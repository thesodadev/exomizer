#ifndef ALREADY_INCLUDED_MATCH_H
#define ALREADY_INCLUDED_MATCH_H
/*
 * Copyright (c) 2002 Magnus Lind.
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
 * This file is a part of the Exomizer v1.1 release
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

void match_ctx_init(match_ctx ctx,      /* IN/OUT */
                    const unsigned char *buf,   /* IN */
                    int buf_len);       /* IN */

void match_ctx_free(match_ctx ctx);     /* IN/OUT */

/* this needs to be called with the indexes in
 * reverse order */
const_matchp matches_get(match_ctx ctx, /* IN/OUT */
                         unsigned short int index);     /* IN */

void match_delete(match_ctx ctx,        /* IN/OUT */
                  matchp mp);   /* IN */

struct _matchp_cache_enum {
    match_ctxp ctx;
    const_matchp next;
    int pos;
};

typedef struct _matchp_cache_enum matchp_cache_enum[1];
typedef struct _matchp_cache_enum *matchp_cache_enump;

void matchp_cache_get_enum(match_ctx ctx,       /* IN */
                           matchp_cache_enum mpce);     /* IN/OUT */

typedef const_matchp matchp_enum_get_next_f(void *matchp_enum); /* IN/OUT */

const_matchp matchp_cache_enum_get_next(void *matchp_cache_enum);       /* IN */

#endif
