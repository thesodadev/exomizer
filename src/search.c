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
#include "search.h"

void search_node_free(search_nodep snp) /* IN */
{
    /* emty now since snp:s are stored in an array */
}

void search_node_dump(search_nodep snp) /* IN */
{
    while (snp != NULL)
    {
        LOG(LOG_NORMAL, ("index %d, mp ", snp->index));
        if (snp->match == NULL)
        {
            LOG(LOG_NORMAL, ("(NULL)"));
        } else
        {
            LOG(LOG_NORMAL,
                ("(of %d, le %d)", snp->match->offset, snp->match->len));
        }
        LOG(LOG_NORMAL,
            (", score %0.1f, total %0.1f\n",
             snp->match_score, snp->total_score));

        snp = snp->prev;
    }
}

search_nodep search_buffer(match_ctx ctx,       /* IN */
                           encode_match_f * f,  /* IN */
                           encode_match_data emd)       /* IN */
{
    static search_node snp_arr[65536];
    const_matchp mp;
    search_nodep snp;
    search_nodep best_copy_snp;
    int best_copy_len;

    search_nodep best_rle_snp;

    int len = ctx->len;

    memset(snp_arr, 0, sizeof(snp_arr));

    snp = snp_arr[len];
    snp->index = len;
    snp->match->offset = 0;
    snp->match->len = 0;
    snp->match_score = 0;
    snp->total_score = 0;
    snp->prev = NULL;

    best_copy_snp = snp;
    best_copy_len = 0.0;

    best_rle_snp = NULL;

    /* think twice about changing this code,
     * it works the way it is. The last time
     * I examined this code I was certain it was
     * broken and broke it myself, trying to fix it. */
    while (len >= 0 &&
           (mp = matches_get(ctx, (unsigned short) (len - 1))) != NULL)
    {
#if 1
        /* check if we can do even better with copy */
        snp = snp_arr[len];
        if(best_copy_snp->total_score+best_copy_len*8.0 -
           snp->total_score > 0.0)
        {
            /* found a better copy endpoint */
#if 0
            LOG(LOG_NORMAL,
                ("best copy start moved to index %d\n", snp->index));
#endif
            best_copy_snp = snp;
            best_copy_len = 0.0;
        } else
        {
            float copy_score = best_copy_len*8.0 + (1.0 + 17.0 + 16.0);
            float total_copy_score = best_copy_snp->total_score + copy_score;

#if 0
                LOG(LOG_NORMAL,
                    ("total score %0.1f, copy total score %0.1f\n",
                     snp->total_score, total_copy_score));
#endif
            if(snp->total_score > total_copy_score)
            {
                match local_mp;
                /*here it is good to just copy instead of crunch */
#if 0
                LOG(LOG_NORMAL,
                    ("copy index %d, len %d, total %0.1f, copy %0.1f\n",
                     snp->index, best_copy_len,
                     snp->total_score, total_copy_score));
#endif

                local_mp->len = best_copy_len;
                local_mp->offset = 0;
                snp->total_score = total_copy_score;
                snp->match_score = copy_score;
                snp->prev = best_copy_snp;
                *snp->match = *local_mp;
            }
        }
#endif
        /* end of copy optimization */

        /* check if we can do rle */
        snp = snp_arr[len];
        if(best_rle_snp == NULL ||
           snp->index + ctx->rle_r[snp->index] < best_rle_snp->index)
        {
            /* best_rle_snp can't be reached by rle from snp, reset it*/
            if(ctx->rle[snp->index] > 0)
            {
                best_rle_snp = snp;
#if 0
                LOG(LOG_NORMAL, ("resetting best_rle at index %d, len %d\n",
                                 snp->index, ctx->rle[snp->index]));
#endif
            }
            else
            {
                best_rle_snp = NULL;
            }
        }
        else if(ctx->rle[snp->index] > 0 &&
                snp->index + ctx->rle_r[snp->index] >= best_rle_snp->index)
        {
            float best_rle_score;
            float total_best_rle_score;
            float snp_rle_score;
            float total_snp_rle_score;
            match rle_mp;

#if 0
            LOG(LOG_NORMAL, ("challenger len %d, index %d, "
                             "ruling len %d, index %d\n",
                             ctx->rle_r[snp->index], snp->index,
                             ctx->rle_r[best_rle_snp->index],
                             best_rle_snp->index));
#endif

            /* snp and best_rle_snp is the same rle area,
             * let's see which is best */
#undef NEW_STYLE
#ifdef NEW_STYLE
            rle_mp->len = best_rle_snp->index - snp->index;
#else
            rle_mp->len = ctx->rle[best_rle_snp->index];
#endif
            rle_mp->offset = 1;
            best_rle_score = f(rle_mp, emd);
            total_best_rle_score = best_rle_snp->total_score +
                best_rle_score;

#ifdef NEW_STYLE
            snp_rle_score = 0.0;
#else
            rle_mp->len = ctx->rle[snp->index];
            rle_mp->offset = 1;
            snp_rle_score = f(rle_mp, emd);
#endif
            total_snp_rle_score = snp->total_score + snp_rle_score;

            if(total_snp_rle_score <= total_best_rle_score)
            {
                /* yes, the snp is a better rle than best_rle_snp */
#if 0
                LOG(LOG_NORMAL, ("prospect len %d, index %d, (%0.1f+%0.1f) "
                                 "ruling len %d, index %d (%0.1f+%0.1f)\n",
                                 ctx->rle[snp->index], snp->index,
                                 snp->total_score, snp_rle_score,
                                 ctx->rle[best_rle_snp->index],
                                 best_rle_snp->index,
                                 best_rle_snp->total_score, best_rle_score));
                best_rle_snp = snp;
                LOG(LOG_NORMAL, ("setting current best_rle: "
                                 "index %d, len %d\n",
                                 snp->index, rle_mp->len));
#endif
            }
        }
        if(best_rle_snp != NULL && best_rle_snp != snp)
        {
            float rle_score;
            float total_rle_score;
            /* check if rle is better */
            match local_mp;
            local_mp->len = best_rle_snp->index - snp->index;
            local_mp->offset = 1;
            rle_score = f(local_mp, emd);
            total_rle_score = best_rle_snp->total_score + rle_score;
#if 0
            LOG(LOG_NORMAL, ("comparing index %d (%0.1f) with "
                             "rle index %d, len %d, total score %0.1f %0.1f\n",
                             snp->index, snp->total_score,
                             best_rle_snp->index, local_mp->len,
                             best_rle_snp->total_score, rle_score));
#endif
            if(snp->total_score > total_rle_score)
            {
                /*here it is good to do rle instead of crunch */
#if 0
                LOG(LOG_NORMAL,
                    ("rle index %d, len %d, total %0.1f, rle %0.1f\n",
                     snp->index, local_mp->len,
                     snp->total_score, total_rle_score));
#endif
                snp->total_score = total_rle_score;
                snp->match_score = rle_score;
                snp->prev = best_rle_snp;
                *snp->match = *local_mp;
            }
        }
        /* end of rle optimization */

#if 0
        LOG(LOG_NORMAL,
            ("matches for index %d with total score %0.1f\n",
             len - 1, snp->total_score));
#endif

        while (mp != NULL)
        {
            float score;
            float prev_score;
            float total_score;


            prev_score = snp_arr[len]->total_score;

            score = f(mp, emd);
            total_score = prev_score + score;
#if 0
            /* log no literals */
            if(mp->offset != 0);
            {
                LOG(LOG_NORMAL,
                    ("%d: offset %d, len %d, score %0.1f, tot %0.1f\n",
                     len, mp->offset, mp->len, score, total_score));
            }
#endif
            snp = snp_arr[len - mp->len];
            if ((total_score < 1000000.0) &&
                (snp->match->len == 0 ||
                 total_score < snp->total_score))
            {
                snp->index = len - mp->len;
                *snp->match = *mp;
                snp->match_score = score;
                snp->total_score = total_score;
                snp->prev = snp_arr[len];
            }
            mp = mp->next;
        }

        /* slow way to get to the next node for cur */
        do
        {
            --len;
            ++best_copy_len;
            if (!(len & 0xFF))
            {
                LOG(LOG_NORMAL, ("."));
            }
        }
        while (snp_arr[len]->match == NULL);
    }
    LOG(LOG_NORMAL, ("\n"));

    return snp_arr[0];
}

void matchp_snp_get_enum(const_search_nodep snp,        /* IN */
                         matchp_snp_enum snpe)  /* IN/OUT */
{
    snpe->startp = snp;
    snpe->currp = snp;
}

const_matchp matchp_snp_enum_get_next(void *matchp_snp_enum)
{
    matchp_snp_enump snpe;
    const_matchp val;

    snpe = matchp_snp_enum;

    val = NULL;
    while (snpe->currp != NULL && val == NULL)
    {
        val = snpe->currp->match;
        snpe->currp = snpe->currp->prev;
    }

    if (snpe->currp == NULL)
    {
        snpe->currp = snpe->startp;
    }
    return val;
}


void matchp_snp_rle_get_enum(const_search_nodep snp,    /* IN */
                             matchp_snp_rle_enum snpe)  /* IN/OUT */
{
    snpe->startp = snp;
    snpe->currp = snp;
}

const_matchp matchp_snp_rle_enum_get_next(void *matchp_snp_enum)
{
    int c, p;
    matchp val;
    const_matchp mp;
    matchp_snp_rle_enump snpe;

    val = NULL;
    snpe = matchp_snp_enum;
    if (snpe->currp == NULL)
    {
        snpe->currp = snpe->startp;
    } else
    {
        val = snpe->m;
        val->len = 1;
        val->offset = 1;
        mp = snpe->currp->match;
        p = mp ? mp->offset == 0 : -2;

        while ((snpe->currp = snpe->currp->prev) != NULL)
        {
            mp = snpe->currp->match;
            if (mp == NULL || mp->len == 0)
            {
                continue;
            }
            c = p;
            p = mp->offset == 0;
            if (p != c)
            {
                break;
            }
            val->len += 1;
        }
    }
    /*LOG(LOG_DUMP("rle: len = %d\n", val ? val->len : -1)); */
    return val;
}
