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
 * match.c, a part of the exomizer v1.0 release
 *
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "log.h"
#include "match.h"
#include "radix.h"


struct _match_node {
    int index;
    struct _match_node *next;
};

typedef struct _match_node match_node[1];
typedef struct _match_node *match_nodep;

static
const_matchp matches_calc(match_ctx ctx,        /* IN/OUT */
                          unsigned short int index);    /* IN */

static
void *match_alloc(int size)
{
    void *p;

    p = malloc(size);
    if (p == NULL)
    {
        LOG(LOG_ERROR,
            ("out of memory, file %s, line %d\n", __FILE__, __LINE__));
        exit(1);
    }
    return p;
}

matchp match_new(match_ctx ctx) /* IN/OUT */
{
    return match_alloc(sizeof(match));
}

void match_ctx_init(match_ctx ctx,      /* IN/OUT */
                    const unsigned char *buf, int buf_len)      /* IN */
{
    match_nodep np;
    match_nodep *npp;
    struct _match_node *single_map[256];
    struct _match_node *tuple_map[65536];

    int i;
    int val;
    memset(single_map, 0, sizeof(single_map));
    memset(tuple_map, 0, sizeof(tuple_map));

    memset(ctx->info, 0, sizeof(ctx->info));
    memset(ctx->rle, 0, sizeof(ctx->rle));
    memset(ctx->rle_r, 0, sizeof(ctx->rle_r));

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

    for (i = buf_len - 1; i >= 0; --i)
    {
        LOG(LOG_DUMP,
            ("buf[%i] = '%c', rle %d\n", i, buf[i], ctx->rle[i]));

        /* allocate single */
        npp = single_map + buf[i];

        np = match_alloc(sizeof(match_node));
        np->next = *npp;
        np->index = i;
        *npp = np;

        ctx->info[i]->single = np->next;

        LOG(LOG_DUMP, ("allocated np for index %d\n", i));

        /* skip rle */
    }

    for (i = buf_len - 1; i >= 1; --i)
    {
        /* allocate tuple */
        npp = tuple_map + ((buf[i - 1] << 8) | buf[i]);

        np = match_alloc(sizeof(match_node));
        np->next = *npp;
        np->index = i;
        *npp = np;

        ctx->info[i]->tuple = np->next;
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

    for (i = 0; i < 256; ++i)
    {
        if (single_map[i] == NULL)
        {
            continue;
        }
        free(single_map[i]);
    }
    for (i = 0; i < 65536; ++i)
    {
        if (tuple_map[i] == NULL)
        {
            continue;
        }
        free(tuple_map[i]);
    }
}

void match_ctx_free(match_ctx ctx)      /* IN/OUT */
{
    matchp mp, mp1;
    int i;
    for (i = 0; i < 65536; ++i)
    {
        free(ctx->info[i]->tuple);
        free(ctx->info[i]->single);
        mp = ctx->info[i]->cache;
        while (mp != NULL)
        {
            mp1 = mp;
            mp = mp->next;
            free(mp1);
        }
    }
}

void dump_matches(matchp mp)
{
    if (mp == NULL)
    {
        LOG(LOG_NORMAL, (" (NULL)\n"));
    } else
    {
        LOG(LOG_NORMAL, (" offset %d, len %d\n", mp->offset, mp->len));
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
    unsigned short int rle_len;

    matchp matches;
    matchp mp;
    match_nodep single_np;
    match_nodep tuple_np;

    buf = ctx->buf;
    matches = NULL;
    do
    {
#if 0
        LOG(LOG_DUMP, ("index %d, char '%c', rle %d, rle_r %d\n",
                       index, buf[index], ctx->rle[index],
                       ctx->rle_r[index]));
#endif
        /* proces the literal match */
        mp = match_new(ctx);
        mp->next = matches;
        matches = mp;

        mp->offset = 0;
        mp->len = 1;

        /* get possible match */
        single_np = ctx->info[index]->single;

        /* think twice before touching this,
         * I thought about this code for several days 
         * when i changed it last and it works now. */
        rle_len = 0;
        do
        {
            /* find correct match */
            while ((single_np != NULL) &&
                   (ctx->rle[single_np->index] < rle_len))
            {
                single_np = single_np->next;
            }
            /* break if no single matches left */
            if (single_np == NULL)
            {
                break;
            }

            /* allocate a match pointer */
            mp = match_new(ctx);
            mp->next = matches;
            matches = mp;

            if (single_np->index - index == 1)
            {
                mp->len = ctx->rle[index] + 1;
            } else
            {
                mp->len = rle_len + 1;
            }
            mp->offset = single_np->index - index;

            ++rle_len;

            /* reiterate only if we are in the highest
             * index position of an rle with len > 1 */
        }
        while ((ctx->rle_r[index] == 0) &&
               (ctx->rle[index] > 0) && (rle_len < ctx->rle[index]));

#if 0
        LOG(LOG_DUMP,
            ("single matches for index %d char '%c'\n", index,
             buf[index]));
        dump_matches(matches);
#endif
        ctx->info[index]->cache = matches;
        /* update index */
        if (index < 1)
        {
            break;
        }

        /* get tuple */
        tuple_np = ctx->info[index]->tuple;

        for (; tuple_np != NULL; tuple_np = tuple_np->next)
        {
            unsigned short int len;

            if (ctx->rle[index] != ctx->rle[tuple_np->index])
            {
                continue;
            }

            LOG(LOG_DUMP, ("index %d, tuple_np->index %d\n",
                           index, tuple_np->index));

            /* get match len */
            len = 2;
            while ((index - len >= 0) &&
                   (buf[index - len] == buf[tuple_np->index - len]))
            {
                int a, b;
                a = ctx->rle[index - len];
                b = ctx->rle[tuple_np->index - len];
                len += (a < b ? a : b);
                ++len;
            }

            /* are we bigger than the previous best? */
            if (len > mp->len)
            {
                /* allocate match struct */
                mp = match_new(ctx);
                mp->next = matches;
                matches = mp;

                mp->offset = tuple_np->index - index;
                mp->len = len;
            }
        }
        /* add to cache */
        ctx->info[index]->cache = matches;
    }
    while (0);
#if 0
    LOG(LOG_DUMP, ("adding matches for index %d to cache\n", index));
    dump_matches(matches);
#endif
    return matches;
}

void matchp_cache_get_enum(match_ctx ctx,       /* IN */
                           matchp_cache_enum mpce)      /* IN/OUT */
{
    mpce->ctx = ctx;
    mpce->pos = 0;
    mpce->next = NULL;
}

const_matchp matchp_cache_enum_get_next(void *matchp_cache_enum)
{
    const_matchp val;
    matchp_cache_enump mpce;

    mpce = matchp_cache_enum;

    val = NULL;
    while (mpce->pos < 65536)
    {
        val = mpce->next;
        if (val != NULL)
        {
            break;
        }
        val = mpce->ctx->info[(mpce->pos)++]->cache;
        if (val != NULL)
        {
            LOG(LOG_DUMP, ("index %d\n", mpce->pos - 1));
            mpce->next = val->next;
            break;
        }
    }
    mpce->next = NULL;
    if (val != NULL)
    {
        LOG(LOG_DUMP,
            (" offset %d, len %d, val $%02X\n",
             val->offset, val->len, mpce->ctx->buf[mpce->pos - 1]));

        mpce->next = val->next;
#if 1
        /* heuristics to skip some matches in
           order to improve initial statistics */
        while (mpce->next && (
                                 /* allow only one entry of each len */
                                 (mpce->next->len == val->len) ||
                                 /* allow only 1 entry with len > 2 */
                                 (mpce->next->len > 7)))
        {
            mpce->next = mpce->next->next;
        }
#endif
    }
    if (val == NULL && mpce->pos >= 65536)
    {
        mpce->pos = 0;
    }
    return val;
}
