#ifndef ALREADY_INCLUDED_MATCH_H
#define ALREADY_INCLUDED_MATCH_H
/*
 * match.h
 *
 */

struct _match {
     unsigned short int offset;
     unsigned short int len;
  struct _match *next;
};

typedef struct _match match[1];
typedef struct _match *matchp;
typedef const struct _match *const_matchp;

struct _pre_calc {
     struct _match_node *single;
     struct _match_node *tuple;
     struct _match *cache;
};

typedef struct _pre_calc pre_calc[1];

struct _match_ctx {
     pre_calc info[65536];
     unsigned short int rle[65536];
     unsigned short int rle_r[65536];
     const unsigned char *buf;
     int len;
};

typedef struct _match_ctx match_ctx[1];
typedef struct _match_ctx *match_ctxp;

void
match_ctx_init(match_ctx ctx, /* IN/OUT */
	       const unsigned char *buf, /* IN */
	       int buf_len); /* IN */

void
match_ctx_free(match_ctx ctx); /* IN/OUT */

/* this needs to be called with the indexes in
 * reverse order */
const_matchp
matches_get(match_ctx ctx, /* IN/OUT */
	    unsigned short int index); /* IN */

void
match_delete(match_ctx ctx, /* IN/OUT */
	     matchp mp); /* IN */

struct _matchp_cache_enum {
     match_ctxp ctx;
     const_matchp next;
     int pos;
};

typedef struct _matchp_cache_enum matchp_cache_enum[1];
typedef struct _matchp_cache_enum *matchp_cache_enump;

void
matchp_cache_get_enum(match_ctx ctx, /* IN */
		      matchp_cache_enum mpce); /* IN/OUT */

typedef const_matchp matchp_enum_get_next_f(void *matchp_enum); /* IN/OUT */

const_matchp
matchp_cache_enum_get_next(void *matchp_cache_enum);

#endif
