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

typedef struct match_node match_node[1];
typedef struct match_node *match_nodep;

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

#if 0
    while(*mpp != NULL &&
          ((*mpp)->len > len ||
           ((*mpp)->len == len && (*mpp)->offset < offset)))
    {
        mpp = &((*mpp)->next);
    }
#endif
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
    match_nodep np;
    match_nodep *npp;
    static struct match_node *single_map[256];
    static struct match_node *tuple_map[65536];

    struct chunkpool map_pool[1];

    int i;
    int val;
    memset(single_map, 0, sizeof(single_map));
    memset(tuple_map, 0, sizeof(tuple_map));

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

    for (i = buf_len - 1; i >= 0; --i)
    {
        LOG(LOG_DUMP,
            ("buf[%i] = '%c', rle %d\n", i, buf[i], ctx->rle[i]));

        /* allocate single */
        npp = single_map + buf[i];

        np = *npp;
        if(np != NULL)
        {
            ctx->info[i]->single = chunkpool_malloc(ctx->m_pool);
            *(ctx->info[i]->single) = *np;
        }
        else
        {
            ctx->info[i]->single = NULL;
            np = chunkpool_malloc(map_pool);
        }
        np->next = ctx->info[i]->single;
        np->index = i;
        *npp = np;

        LOG(LOG_DUMP, ("allocated np for index %d\n", i));

        /* skip rle */
    }

    for (i = buf_len - 1; i >= 1; --i)
    {
        /* allocate tuple */
        npp = tuple_map + ((buf[i - 1] << 8) | buf[i]);

        np = *npp;
        if(np != NULL)
        {
            ctx->info[i]->tuple = chunkpool_malloc(ctx->m_pool);
            *(ctx->info[i]->tuple) = *np;
        }
        else
        {
            ctx->info[i]->tuple = NULL;
            np = chunkpool_malloc(map_pool);
        }
        np->next = ctx->info[i]->tuple;
        np->index = i;
        *npp = np;
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
        /* proces the literal match and add it to matches */
        mp = match_new(ctx, &matches, 1, 0);

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

            if(single_np->index > index + ctx->max_offset)
            {
                break;
            }

            /* allocate a match node and add it to matches */
            if (single_np->index - index == 1)
            {
                mp = match_new(ctx, &matches, ctx->rle[index] + 1,
                               single_np->index - index);
            } else
            {
                mp = match_new(ctx, &matches, rle_len + 1,
                               single_np->index - index);
            }

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

            if(tuple_np->index > index + ctx->max_offset)
            {
                break;
            }

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
                /* allocate match struct and add it to matches */
                mp = match_new(ctx, &matches, len, tuple_np->index - index);
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

#if 0
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
                                 (mpce->next->len > 7) ||
                                 (val->len == 1 && (val->offset == 0 || val->offset > 32))))
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
#else

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
#if 1
                if(val->len == 1 &&
                   (litp->offset == 0 || litp->offset > val->offset))
                {
                    litp = val;
                }
#endif
            }
            val = val->next;
        }

    }
#if 0
    if(litp == NULL)
        LOG(LOG_NORMAL, ("litp(NULL)"));
    else
        LOG(LOG_NORMAL, ("litp(%d,%d)", litp->len, litp->offset));

    if(seqp == NULL)
        LOG(LOG_NORMAL, ("seqp(NULL)"));
    else
        LOG(LOG_NORMAL, ("seqp(%d,%d)", seqp->len, seqp->offset));
#endif


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
#endif
