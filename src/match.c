/*
 * Copyright (c) 2002, 2003 Magnus Lind.
 *
 * This software is provided 'as-is', without any express or implied warranty.
 * In no event will the authors be held liable for any damages arising from
 * the use of this software.
 *
 * Permission is granted to anyone to use this software, alter it and re-
 * distribute it freely for any non-commercial, non-profit purpose subject to
 * the following restrictions:
 *
 *   1. The origin of this software must not be misrepresented; you must not
 *   claim that you wrote the original software. If you use this software in a
 *   product, an acknowledgment in the product documentation would be
 *   appreciated but is not required.
 *
 *   2. Altered source versions must be plainly marked as such, and must not
 *   be misrepresented as being the original software.
 *
 *   3. This notice may not be removed or altered from any distribution.
 *
 *   4. The names of this software and/or it's copyright holders may not be
 *   used to endorse or promote products derived from this software without
 *   specific prior written permission.
 *
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "log.h"
#include "match.h"
#include "chunkpool.h"
#include "radix.h"


struct match_node {
    int index;
    struct match_node *next;
};

static
const_matchp matches_calc(match_ctx ctx,        /* IN/OUT */
                          unsigned short int index);    /* IN */

matchp match_new(match_ctx ctx, /* IN/OUT */
                 matchp *mpp,
                 unsigned short int len,
                 unsigned short int offset)
{
    matchp m = chunkpool_malloc(ctx->m_pool);
    m->len = len;
    m->offset = offset;

    /* insert new node in list */
    m->next = *mpp;
    *mpp = m;

    return m;
}

void match_ctx_init(match_ctx ctx,      /* IN/OUT */
                    const unsigned char *buf,      /* IN */
                    int buf_len,        /* IN */
                    int max_offset)
{
    struct match_node *np;
    struct chunkpool map_pool[1];

    int c, i;
    int val;

    memset(ctx->info, 0, sizeof(ctx->info));
    memset(ctx->rle, 0, sizeof(ctx->rle));
    memset(ctx->rle_r, 0, sizeof(ctx->rle_r));

    chunkpool_init(ctx->m_pool, sizeof(match));
    chunkpool_init(map_pool, sizeof(match));

    ctx->max_offset = max_offset;

    ctx->buf = buf;
    ctx->len = buf_len;

    val = buf[0];
    for (i = 1; i < buf_len; ++i)
    {
        if (buf[i] == val)
        {
            ctx->rle[i] = ctx->rle[i - 1] + 1;
        } else
        {
            ctx->rle[i] = 0;
        }
        val = buf[i];
    }

    val = buf[buf_len - 1];
    for (i = buf_len - 2; i >= 0; --i)
    {
        if (buf[i] == val)
        {
            ctx->rle_r[i] = ctx->rle_r[i + 1] + 1;
        } else
        {
            ctx->rle_r[i] = 0;
        }
        val = buf[i];
    }

    /* add extra nodes to rle sequences */
    for(c = 0; c < 256; ++c)
    {
        static char rle_map[65536];
        struct match_node *prev_np;
        int rle_len;

        /* for each possible rle char */
        memset(rle_map, 0, sizeof(rle_map));
        prev_np = NULL;
        for (i = 0; i < buf_len; ++i)
        {
            /* must be the correct char */
            if(buf[i] != c)
            {
                continue;
            }

            rle_len = ctx->rle[i];
            if(!rle_map[rle_len] && ctx->rle_r[i] > 1)
            {
                /* no previous lengths and not our primary length*/
                continue;
            }

            np = chunkpool_malloc(ctx->m_pool);
            np->index = i;
            np->next = NULL;
            rle_map[rle_len] = 1;

            LOG(LOG_DUMP, ("0) c = %d, added np idx %d -> %d\n", c, i, 0));

            /* if we have a previous entry, let's chain it together */
            if(prev_np != NULL)
            {
                LOG(LOG_DEBUG, ("1) c = %d, pointed np idx %d -> %d\n",
                                c, prev_np->index, i));
                prev_np->next = np;
            }

            ctx->info[i]->single = np;
            prev_np = np;
        }

        memset(rle_map, 0, sizeof(rle_map));
        prev_np = NULL;
        for (i = buf_len - 1; i >= 0; --i)
        {
            /* must be the correct char */
            if(buf[i] != c)
            {
                continue;
            }

            rle_len = ctx->rle_r[i];
            np = ctx->info[i]->single;
            if(np == NULL)
            {
                if(rle_map[rle_len] && prev_np != NULL && rle_len > 0)
                {
                    np = chunkpool_malloc(ctx->m_pool);
                    np->index = i;
                    np->next = prev_np;
                    ctx->info[i]->single = np;

                    LOG(LOG_DEBUG, ("2) c = %d, added np idx %d -> %d\n",
                                    c, i, prev_np->index));
                }
            }
            else
            {
                prev_np = np;
            }

            if(ctx->rle_r[i] > 0)
            {
                continue;
            }
            rle_len = ctx->rle[i];
            rle_map[rle_len] = 1;
        }
    }

    for (i = buf_len - 1; i >= 0; --i)
    {
        /* let's populate the cache */
        matches_calc(ctx, (unsigned short) i);
        if (!(i & 0xFF))
        {
            LOG(LOG_NORMAL, ("."));
        }

    }

    LOG(LOG_NORMAL, ("\n"));

    chunkpool_free(map_pool);
}

void match_ctx_free(match_ctx ctx)      /* IN/OUT */
{
    chunkpool_free(ctx->m_pool);
}

void dump_matches(matchp mp)
{
    if (mp == NULL)
    {
        LOG(LOG_DEBUG, (" (NULL)\n"));
    } else
    {
        if(mp->offset > 0)
        {
            LOG(LOG_DEBUG, (" offset %d, len %d\n", mp->offset, mp->len));
        }
        if (mp->next != NULL)
        {
            dump_matches(mp->next);
        }
    }
}

const_matchp matches_get(match_ctx ctx, /* IN/OUT */
                         unsigned short int index)      /* IN */
{
    return ctx->info[index]->cache;

}

/* this needs to be called with the indexes in
 * reverse order */
const_matchp matches_calc(match_ctx ctx,        /* IN/OUT */
                          unsigned short int index)     /* IN */
{
    const unsigned char *buf;

    matchp matches;
    matchp mp;
    struct match_node *np;

    buf = ctx->buf;
    matches = NULL;

    LOG(LOG_DUMP, ("index %d, char '%c', rle %d, rle_r %d\n",
                   index, buf[index], ctx->rle[index],
                   ctx->rle_r[index]));

    /* proces the literal match and add it to matches */
    mp = match_new(ctx, &matches, 1, 0);

    /* get possible match */
    np = ctx->info[index]->single;
    if(np != NULL)
    {
        np = np->next;
    }
    for (; np != NULL; np = np->next)
    {
        int mp_len;
        int our_pos;
        int np_pos;

        /* limit according to max offset */
        if(np->index > index + ctx->max_offset)
        {
            break;
        }

        LOG(LOG_DEBUG, ("find lengths for index %d to index %d\n",
                        index, np->index));

        /* get match len */
        mp_len = mp->offset > 0 ? mp->len : 0;
        LOG(LOG_DEBUG, ("0) comparing current offset length [%d] %d %d\n",
                        index, mp->offset, mp_len));
        our_pos = index - mp_len + 1;
        np_pos = np->index - mp_len + 1;
        /* Compare the first <previous len> bytes backwards. We can
         * skip some comparisons by increasing by the rle count. We
         * don't need to compare the first byte, hence > 1 instead of
         * > 0 */
        while(mp_len > 1 && buf[our_pos] == buf[np_pos])
        {
            int offset1 = ctx->rle_r[our_pos];
            int offset2 = ctx->rle_r[np_pos];
            int offset = offset1 < offset2 ? offset1 : offset2;

            LOG(LOG_DEBUG, ("1) compared sucesssfully [%d] %d %d\n",
                            index, our_pos, np_pos));

            mp_len -= 1 + offset;
            our_pos += 1 + offset;
            np_pos += 1 + offset;
        }
        if(mp_len > 1)
        {
            /* sequence length too short, skip this match */
            continue;
        }

        /* Here we know that the current match is atleast as long as
         * the previuos one. let's compare further. */
        mp_len = mp->offset > 0 ? mp->len : 0;
        our_pos = index - mp_len;
        np_pos = np->index - mp_len;
        while(our_pos >= 0 && buf[our_pos] == buf[np_pos])
        {
            LOG(LOG_DEBUG, ("2) compared sucesssfully [%d] %d %d\n",
                            index, our_pos, np_pos));
            --our_pos;
            --np_pos;
            /* allocate match struct and add it to matches */
            mp = match_new(ctx, &matches, index - our_pos, np->index - index);
        }
        if(our_pos < 0)
        {
            /* we have reached the eof, no better matches can be found */
            break;
        }
    }
    /* add to cache */
    ctx->info[index]->cache = matches;

    LOG(LOG_DEBUG, ("adding matches for index %d to cache\n", index));
    dump_matches(matches);

    return matches;
}

static
int
matchp_keep_this(const_matchp mp)
{
    int val = 1;
    /* if we want to ignore this matchp then return true else false */
    if(mp->len == 1)
    {
        if(mp->offset > 32)
        {
            val = 0;
        }
    }
    else if(mp->len == 2)
    {
        if(mp->offset > 8192)
        {
            val = 1;
        }
    }

    return val;
}

static
void
matchp_cache_peek(struct match_ctx *ctx, int pos,
                  const_matchp *litpp, const_matchp *seqpp)
{
    const_matchp litp, seqp, val;

    seqp = NULL;
    litp = NULL;
    if(pos >= 0)
    {
        val = matches_get(ctx, pos);
        litp = val;
        while(litp->offset != 0)
        {
            litp = litp->next;
        }
        while(val != NULL)
        {
            if(matchp_keep_this(val) && val->offset != 0)
            {
                if(seqp == NULL || val->len > seqp->len)
                {
                    seqp = val;
                }
                if(val->len == 1 &&
                   (litp->offset == 0 || litp->offset > val->offset))
                {
                    litp = val;
                }
            }
            val = val->next;
        }

    }

    if(litpp != NULL) *litpp = litp;
    if(seqpp != NULL) *seqpp = seqp;
}

void matchp_cache_get_enum(match_ctx ctx,       /* IN */
                           matchp_cache_enum mpce)      /* IN/OUT */
{
    mpce->ctx = ctx;
    mpce->pos = ctx->len - 1;
    /*mpce->next = NULL;*/ /* two iterations */
    mpce->next = (void*)mpce; /* just one */
}

const_matchp matchp_cache_enum_get_next(void *matchp_cache_enum)
{
    const_matchp val, lit, seq;
    matchp_cache_enump mpce;

    mpce = matchp_cache_enum;

 restart:
    matchp_cache_peek(mpce->ctx, mpce->pos, &lit, &seq);
    val = lit;
    if(lit == NULL)
    {
        /* the end, restart and return NULL */
        mpce->pos = mpce->ctx->len - 1;
        if(mpce->next == NULL)
        {
            mpce->next = (void*)mpce;
            goto restart;
        }
        else
        {
            mpce->next = NULL;
        }
    }
    else
    {
        if(seq != NULL)
        {
            const_matchp next;
            matchp_cache_peek(mpce->ctx, mpce->pos - 1, NULL, &next);
            if(next == NULL ||
               (next->len + (mpce->next != NULL && next->len < 3) <= seq->len))
            {
                /* nope, next is not better, use this sequence */
                val = seq;
            }
        }
    }
    if(val != NULL)
    {
        LOG(LOG_DUMP, ("Using len %d, offset, %d\n", val->len, val->offset));
        mpce->pos -= val->len;
    }
    return val;
}

