/*
 * Copyright (c) 2002 Magnus Lind.
 *
 * This software is provided 'as-is', without any express or implied warranty.
 * In no event will the authors be held liable for any damages arising from
 * the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 *   1. The origin of this software must not be misrepresented; you must not
 *   claim that you wrote the original software. If you use this software in a
 *   product, an acknowledgment in the product documentation would be
 *   appreciated but is not required.
 *
 *   2. Altered source versions must be plainly marked as such, and must not
 *   be misrepresented as being the original software.
 *
 *   3. This notice may not be removed or altered from any source distribution.
 *
 *
 * optimal_dep.c, a part of the exomizer v1.0beta release
 *
 */
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <setjmp.h>

#define STAT2

struct _dep_table {
    unsigned char table_bit[3];
    unsigned char table_off[3];
    unsigned char table_bi[100];
    unsigned char table_lo[100];
    unsigned char table_hi[100];
};

typedef struct _dep_table dep_table[1];
typedef struct _dep_table *dep_tablep;

struct _dep_ctx {
    int reverse;
    int inpos;
    int inend;
    unsigned char inbuf[65536];
    unsigned char outbuf[65536];
    unsigned short int outpos;
    unsigned short int outend;
    unsigned int bitbuf;
    jmp_buf done;
    /* dep_table */
    dep_table t;
};

typedef struct _dep_ctx dep_ctx[1];
typedef struct _dep_ctx *dep_ctxp;

#if STAT
static FILE *stat = NULL;
#endif
static int bytes_read;
static int bytes_written;
static int literal_counter;
static int sequence_counter;

static int max_1len_o;
static int max_2len_o;
static int max_3len_o;

int
get_byte(dep_ctxp ctx)
{
    int c;
    if(!ctx->reverse)
    {
        if(ctx->inpos == 0)
        {
            fprintf(stderr, "unexpected end of file\n");
            longjmp(ctx->done, 1);
        }
        --ctx->inpos;
        if(ctx->outpos <= ctx->inpos + ctx->inend)
        {
            printf("reading clobbered data at $%04X, "
                   "outpos at $%04X\n", ctx->inpos + ctx->inend, ctx->outpos);
        }
        c = ctx->inbuf[ctx->inpos];
    }
    else
    {
        if(ctx->inpos == ctx->inend)
        {
            fprintf(stderr, "unexpected end of file\n");
            longjmp(ctx->done, 1);
        }
        c = ctx->inbuf[ctx->inpos++];
    }

    ++bytes_read;

    return c;
}

int
get_bits(dep_ctxp ctx, int count)
{
    int val;

    val = 0;

    /*printf("get_bits: count = %d", count);*/
    while(count-- > 0) {
        if((ctx->bitbuf & 0x1FF) == 1) {
            ctx->bitbuf = get_byte(ctx) | 0x100;
        }
        val <<= 1;
        val |= ctx->bitbuf & 0x1;
        ctx->bitbuf >>= 1;
        /*printf("bit read %d\n", val &1);*/
    }
    /*printf(" val = %d\n", val);*/
    return val;
}

int
get_gamma_code(dep_ctxp ctx)
{
    int gamma_code;
    /* get bitnum index */
    gamma_code = 0;
    while(get_bits(ctx, 1) == 0)
    {
        ++gamma_code;
    }
    return gamma_code;
}

int
get_cooked_code_phase2(dep_ctxp ctx, int index)
{
    int base;
    dep_tablep tp;
    tp = ctx->t;

    base = tp->table_lo[index] | (tp->table_hi[index] << 8);
    return base + get_bits(ctx, tp->table_bi[index]);
}

void
dep_loop(dep_ctxp ctx)
{
    int val;
    int i;
    int len;
    int offset;
    int src = 0;

    for(;;)
    {
        int literal = 0;
        if(get_bits(ctx, 1))
        {
            ++literal_counter;
            /* literal */
            len = 1;
#ifdef STAT2
            printf("[%d] literal\n", ctx->outpos - 1065);
#endif
            literal = 1;
            goto literal;
        }

        ++sequence_counter;
        /*printf("sekvens1\n");*/
        val = get_gamma_code(ctx);
        if(val >= 17)
        {
            /* done */
            longjmp(ctx->done, 1);
        }
        if(val == 16)
        {
            len = get_bits(ctx, 16);
            literal = 1;
#ifdef STAT2
            printf("[%d] literal copy len %d\n", ctx->outpos - 1065, len);
#endif
            goto literal;
        }

        /*printf("sekvens2\n");*/
        len = get_cooked_code_phase2(ctx, val);

        i = (len > 3 ? 3 : len) - 1;

        val = ctx->t->table_off[i] + get_bits(ctx, ctx->t->table_bit[i]);

        offset = get_cooked_code_phase2(ctx, val);
        switch(len)
        {
        case 1:
            if(offset > max_1len_o) max_1len_o = offset;
            break;
        case 2:
            if(offset > max_2len_o) max_2len_o = offset;
            break;
        case 3:
            if(offset > max_3len_o) max_3len_o = offset;
            break;
        }

#ifdef STAT2
        printf("[%d] sekvens offset = %d, len = %d\n",
               ctx->outpos - 1065, offset, len);
#endif

        src = ctx->outpos + offset;

    literal:
        do {
            if(literal)
            {
                val = get_byte(ctx);
            }
            else
            {
                val = ctx->outbuf[--src];
            }
            /*printf("val = %d '%c'\n", val, isprint(val)?val:'.');*/
            ctx->outbuf[--(ctx->outpos)] = val;
            ++bytes_written;
        } while (--len > 0);
#if STAT
        fprintf(stat, "%d\t%d\t%d\t%d\t%d\n",
                bytes_written, bytes_read, bytes_written - bytes_read,
                literal_counter, sequence_counter);
#endif
    }
}

static
void
table_init(dep_ctxp ctx, dep_tablep tp) /* IN/OUT */
{
    int i;
    unsigned int a = 0;
    unsigned int b = 0;

    tp->table_bit[0] = 2;
    tp->table_bit[1] = 4;
    tp->table_bit[2] = 4;

    tp->table_off[0] = 48;
    tp->table_off[1] = 32;
    tp->table_off[2] = 16;

    for(i = 0; i < 52; ++i)
    {
        if(i & 0xF)
        {
            a += 1 << b;
        } else
        {
            a = 1;
        }

        tp->table_lo[i] = a & 0xFF;
        tp->table_hi[i] = a >> 8;

        b = get_bits(ctx, 4);

        tp->table_bi[i] = b;

    }
}

void
table_dump(dep_tablep tp)
{
    int base;
    int i, j;

    printf("table_bit[3] = {%d, %d, %d}\n",
           tp->table_bit[0], tp->table_bit[1], tp->table_bit[2]);

    printf("table_off[3] = {%d, %d, %d}\n",
           tp->table_off[0], tp->table_off[1], tp->table_off[2]);

    /* lengths */
    printf("length: ");
    for(i = 0; i < 16; ++i)
    {
        base = tp->table_lo[i] | (tp->table_hi[i] << 8);
        printf("%d[1«%d], ", base, tp->table_bi[i]);
    }

    for(j = 0; j < 3; ++j)
    {
        int start;
        int end;
        printf("\noffset[%d]: ", j);
        start = tp->table_off[j];
        end = start + (1 << tp->table_bit[j]);
        for(i = start; i < end; ++i)
        {
            base = tp->table_lo[i] | (tp->table_hi[i] << 8);
            printf("%d[1«%d], ", base, tp->table_bi[i]);
        }
    }
    printf("\n");

    for(i = 0; i < 16; ++i)
    {
        printf("%d", tp->table_bi[i]);
    }
    for(j = 0; j < 3; ++j)
    {
        int start;
        int end;
        printf(",");
        start = tp->table_off[j];
        end = start + (1 << tp->table_bit[j]);
        for(i = start; i < end; ++i)
        {
            printf("%d", tp->table_bi[i]);
        }
    }
    printf("\n");

}

void
dep_ctx_init(dep_ctxp ctx, FILE *in, int reverse)
{
    ctx->reverse = reverse;

    if(!reverse)
    {
        int start;
        start = fgetc(in);
        start |= fgetc(in) << 8;
        ctx->inend = start;
        ctx->inpos = fread(ctx->inbuf, 1, sizeof(ctx->inbuf), in);
    }
    else
    {
        ctx->inend = fread(ctx->inbuf, 1, sizeof(ctx->inbuf), in);
        ctx->inpos = 0;
    }

    ctx->outpos = 65535;

    /* init tables and shit */
    ctx->outend = get_byte(ctx) << 8;
    ctx->outend |= get_byte(ctx);

    printf("outend starts at $%04X\n", ctx->outend);

    /* init bitbuf */
    ctx->bitbuf = get_byte(ctx);
    /*printf("bitbuf 0x%02X\n", ctx->bitbuf);*/

    ctx->outpos = ctx->outend;

    table_init(ctx, ctx->t);
    table_dump(ctx->t);
}

void
dep_ctx_exit(dep_ctxp ctx, FILE *out)
{
    /* output start lo */
    fputc(ctx->outpos & 0xFF, out);
    /* output start hi */
    fputc((ctx->outpos >> 8) & 0xFF, out);

    /* output data */
    fwrite(ctx->outbuf + ctx->outpos, 1, ctx->outend - ctx->outpos, out);
}

int
main(int argc, char *argv[])
{
    dep_ctx ctx;
    FILE *in = NULL;
    FILE *out = NULL;

    switch(argc) {
    case 2:
        out = stdout;
        break;
    case 3:
        out = fopen(argv[2], "wb");
        if(out == NULL) {
            printf("%s: cant open \"%s\" for output\n", argv[0], argv[2]);
        }
        break;
    default:
        printf("usage: %s infile [outfile]\n", argv[0]);
        exit(1);
    }

    in = fopen(argv[1], "rb");
    if(in == NULL) {
        printf("%s: cant open \"%s\" for input\n", argv[0], argv[1]);
        exit(1);
    }

    dep_ctx_init(ctx, in, 0);

    printf("ending adress is $%04X\n", ctx->outpos);

#if STAT
    stat = fopen("stat.txt", "w");
    fprintf(stat, "written\tread\tw-r\tliteral\tsequence\n");
#endif
    bytes_read = 0;
    bytes_written = 0;
    literal_counter = 0;
    sequence_counter = 0;

    max_1len_o = 0;
    max_2len_o = 0;
    max_3len_o = 0;

    if(setjmp(ctx->done) == 0)
    {
        dep_loop(ctx);
    }

    printf("max1 %d, max2 %d, max3 %d\n", max_1len_o, max_2len_o, max_3len_o);

#if STAT
    fclose(stat);
#endif

    printf("starting adress is $%04X\n", ctx->outpos);

    dep_ctx_exit(ctx, out);

    fclose(in);
    fclose(out);

    return 0;
}
