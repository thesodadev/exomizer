#ifndef ALREADY_INCLUDED_OUTPUT_H
#define ALREADY_INCLUDED_OUTPUT_H
/*
 * output.h
 *
 */

#include <stdio.h>

struct _output_ctx {
     unsigned int bitbuf;
     unsigned short int pos;
     unsigned short int start;
     unsigned char flags;
     unsigned char buf[65536];
};

typedef struct _output_ctx output_ctx[1];
typedef struct _output_ctx *output_ctxp;

void
output_ctx_init(output_ctx ctx); /* IN/OUT */

void
output_ctx_set_start(output_ctx ctx, /* IN/OUT */
                     unsigned int pos); /* IN */

void
output_ctx_set_reverse(output_ctx ctx); /* IN/OUT */

unsigned int
output_ctx_close(output_ctx ctx, /* IN */
		 FILE *out); /* OUT */

unsigned int
output_get_pos(output_ctx ctx); /* IN */

void
output_set_pos(output_ctx ctx, /* IN */
               unsigned int pos); /* IN */

void
output_copy_bytes(output_ctx ctx, /* IN */
                  unsigned int src_pos, /* IN */
                  unsigned int len); /* IN */

void
output_byte(output_ctx ctx, /* IN/OUT */
	    unsigned char byte); /* IN */

void
output_word(output_ctx ctx, /* IN/OUT */
	    unsigned short int word); /* IN */

void
output_bits_flush(output_ctx ctx); /* IN/OUT */

void
output_bits(output_ctx ctx, /* IN/OUT */
	    int count, /* IN */
	    int val); /* IN */

void
output_gamma_code(output_ctx ctx, /* IN/OUT */
		  int code); /* IN */
#endif
