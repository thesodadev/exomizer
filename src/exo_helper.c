
#include "log.h"
#include "output.h"
#include "membuf.h"
#include "match.h"
#include "search.h"
#include "optimal.h"
#include "exodec.h"
#include "exo_helper.h"
#include <stdlib.h>


/* returns 0 if ok, 1 otherwise */
int
str_to_int(const char *str, int *value)
{
    int status = 0;
    do {
        char *str_end;
        long lval;

        /* base 0 is auto detect */
        int base = 0;

        if (*str == '\0')
        {
            /* no string to parse */
            status = 1;
            break;
        }

        if (*str == '$')
        {
            /* a $ prefix specifies base 16 */
            ++str;
            base = 16;
        }

        lval = strtol(str, &str_end, base);

        if(*str_end != '\0')
        {
            /* there is garbage in the string */
            status = 1;
            break;
        }

        if(value != NULL)
        {
            /* all is well, set the out parameter */
            *value = (int)lval;
        }
    } while(0);

    return status;
}

int do_output(match_ctx ctx,
              search_nodep snp,
              encode_match_data emd,
              encode_match_f * f,
              struct membuf *outbuf,
              int *literal_sequences_used)
{
    int pos;
    int pos_diff;
    int max_diff;
    int diff;
    int copy_used = 0;
    output_ctxp old;
    output_ctx out;

    output_ctx_init(out, outbuf);
    old = emd->out;
    emd->out = out;

    pos = output_get_pos(out);

    pos_diff = pos;
    max_diff = 0;

    LOG(LOG_DUMP, ("pos $%04X\n", out->pos));
    output_gamma_code(out, 17);
    output_bits(out, 1, 0); /* 1 bit out */

    diff = output_get_pos(out) - pos_diff;
    if(diff > max_diff)
    {
        max_diff = diff;
    }

    LOG(LOG_DUMP, ("pos $%04X\n", out->pos));
    LOG(LOG_DUMP, ("------------\n"));

    while (snp != NULL)
    {
        const_matchp mp;

        mp = snp->match;
        if (mp != NULL && mp->len > 0)
        {
            if (mp->offset == 0)
            {
                if(mp->len == 1)
                {
                    /* literal */
                    output_byte(out, ctx->buf[snp->index]);
                    output_bits(out, 1, 1);
                } else
                {
                    int i;
                    for(i = 0; i < mp->len; ++i)
                    {
                        output_byte(out, ctx->buf[snp->index + i]);
                    }
                    output_bits(out, 16, mp->len);
                    output_gamma_code(out, 16);
                    output_bits(out, 1, 0);
                    copy_used = 1;
                }
            } else
            {
                f(mp, emd);
                output_bits(out, 1, 0);
            }

            pos_diff += mp->len;
            diff = output_get_pos(out) - pos_diff;
            if(diff > max_diff)
            {
                max_diff = diff;
            }
        }
        LOG(LOG_DUMP, ("------------\n"));
        snp = snp->prev;
    }

    LOG(LOG_DUMP, ("pos $%04X\n", out->pos));
    /* output header here */
    optimal_out(out, emd);
    LOG(LOG_DUMP, ("pos $%04X\n", out->pos));

    output_bits_flush(out);

    emd->out = old;

    if(literal_sequences_used != NULL)
    {
        *literal_sequences_used = copy_used;
    }

    return max_diff;
}

search_nodep
do_compress(match_ctx ctx, encode_match_data emd,
            const char *exported_encoding,
            int max_passes,
            int use_literal_sequences)
{
    matchp_cache_enum mpce;
    matchp_snp_enum snpe;
    search_nodep snp;
    search_nodep best_snp;
    int pass;
    float size;
    float old_size;

    pass = 1;
    LOG(LOG_NORMAL, (" pass %d: optimizing ..\n", pass));

    if(exported_encoding != NULL)
    {
        optimal_encoding_import(emd, exported_encoding);
    }
    else
    {
        matchp_cache_get_enum(ctx, mpce);
        optimal_optimize(emd, matchp_cache_enum_get_next, mpce);
    }

    best_snp = NULL;
    old_size = 100000000.0;

    for (;;)
    {
        snp = search_buffer(ctx, optimal_encode, emd,
                            use_literal_sequences);
        if (snp == NULL)
        {
            LOG(LOG_ERROR, ("error: search_buffer() returned NULL\n"));
            exit(-1);
        }

        size = snp->total_score;
        LOG(LOG_NORMAL, ("  size %0.1f bits ~%d bytes\n",
                         size, (((int) size) + 7) >> 3));

        if (size >= old_size)
        {
            search_node_free(snp);
            break;
        }

        if (best_snp != NULL)
        {
            search_node_free(best_snp);
        }
        best_snp = snp;
        old_size = size;
        ++pass;

        if(pass > max_passes)
        {
            break;
        }

        optimal_free(emd);
        optimal_init(emd);

        LOG(LOG_NORMAL, (" pass %d: optimizing ..\n", pass));

        matchp_snp_get_enum(snp, snpe);
        optimal_optimize(emd, matchp_snp_enum_get_next, snpe);
    }

    return best_snp;
}

void crunch_backwards(struct membuf *inbuf,
                      struct membuf *outbuf,
                      const char *exported_encoding,
                      int max_passes,
                      int max_offset,
                      int use_literal_sequences,
                      struct crunch_info *info)
{
    static match_ctx ctx;
    encode_match_data emd;
    search_nodep snp;
    int outlen;
    int safety;
    int copy_used;

    outlen = membuf_memlen(outbuf);
    emd->out = NULL;
    optimal_init(emd);

    LOG(LOG_NORMAL,
        ("\nPhase 1: Instrumenting file"
         "\n-----------------------------\n"));
    LOG(LOG_NORMAL, (" Length of indata: %d bytes.\n", membuf_memlen(inbuf)));

    match_ctx_init(ctx, inbuf, max_offset);

    LOG(LOG_NORMAL, (" Instrumenting file, done.\n"));

    emd->out = NULL;
    optimal_init(emd);

    LOG(LOG_NORMAL,
        ("\nPhase 2: Calculating encoding"
         "\n-----------------------------\n"));
    snp = do_compress(ctx, emd, exported_encoding,
                      max_passes, use_literal_sequences);
    LOG(LOG_NORMAL, (" Calculating encoding, done.\n"));

    LOG(LOG_NORMAL,
        ("\nPhase 3: Generating output file"
         "\n------------------------------\n"));
    LOG(LOG_NORMAL, (" Encoding: %s\n", optimal_encoding_export(emd)));
    safety = do_output(ctx, snp, emd, optimal_encode, outbuf, &copy_used);
    LOG(LOG_NORMAL, (" Length of crunched data: %d bytes.\n",
                     membuf_memlen(outbuf) - outlen));

    optimal_free(emd);
    search_node_free(snp);
    match_ctx_free(ctx);

    if(info != NULL)
    {
        info->literal_sequences_used = copy_used;
        info->needed_safety_offset = safety;
    }
}

void reverse_buffer(char *start, int len)
{
    char *end = start + len - 1;
    char tmp;

    while (start < end)
    {
        tmp = *start;
        *start = *end;
        *end = tmp;

        ++start;
        --end;
    }
}

void crunch(struct membuf *inbuf,
            struct membuf *outbuf,
            const char *exported_encoding,
            int max_passes,
            int max_offset,
            int use_literal_sequences,
            struct crunch_info *info)
{
    int outpos;
    reverse_buffer(membuf_get(inbuf), membuf_memlen(inbuf));
    outpos = membuf_memlen(outbuf);

    crunch_backwards(inbuf, outbuf, exported_encoding,
                     max_passes, max_offset,
                     use_literal_sequences, info);

    reverse_buffer(membuf_get(inbuf), membuf_memlen(inbuf));
    reverse_buffer((char*)membuf_get(outbuf) + outpos, membuf_memlen(outbuf));
}

void decrunch(int level,
              struct membuf *inbuf,
              struct membuf *outbuf)
{
    struct dec_ctx ctx[1];
    char *enc;
    enc = dec_ctx_init(ctx, inbuf, outbuf);

    LOG(level, (" Encoding: %s\n", enc));

    dec_ctx_decrunch(ctx);
    dec_ctx_free(ctx);
}

void decrunch_backwards(int level,
                        struct membuf *inbuf,
                        struct membuf *outbuf)
{
    int outpos;
    reverse_buffer(membuf_get(inbuf), membuf_memlen(inbuf));
    outpos = membuf_memlen(outbuf);

    decrunch(level, inbuf, outbuf);

    reverse_buffer(membuf_get(inbuf), membuf_memlen(inbuf));
    reverse_buffer((char*)membuf_get(outbuf) + outpos, membuf_memlen(outbuf));
}
