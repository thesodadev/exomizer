#ifndef ALREADY_INCLUDED_EXODEC
#define ALREADY_INCLUDED_EXODEC

#include "membuf.h"
#include <setjmp.h>

struct dec_table
{
    unsigned char table_bit[3];
    unsigned char table_off[3];
    unsigned char table_bi[100];
    unsigned char table_lo[100];
    unsigned char table_hi[100];
};

struct dec_ctx
{
    int inpos;
    int inend;
    unsigned char *inbuf;
    struct membuf *outbuf;
    unsigned int bitbuf;
    jmp_buf done;
    /* dep_table */
    struct dec_table t[1];
    int bits_read;
};

/* returns the encoding */
char *
dec_ctx_init(struct dec_ctx ctx[1], struct membuf *inbuf, struct membuf *outbuf);

void
dec_ctx_free(struct dec_ctx ctx[1]);

void dec_ctx_decrunch(struct dec_ctx ctx[1]);

#endif
