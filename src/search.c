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
 *   3. This notice may not be removed or altered from any source distribution.
 *
 *   4. The names of this software and/or it's copyright holders may not be
 *   used to endorse or promote products derived from this software without
 *   specific prior written permission.
 *
 * search.c, a part of the exomizer v1.0beta2 release
 *
 */
#include "search.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void
search_node_free(search_nodep snp) /* IN */
{
     /* emty now since snp:s are stored in an array */
}

void
search_node_dump(search_nodep snp) /* IN */
{
     while(snp != NULL)
     {
       	  printf("index %d, mp ", snp->index);
       	  if(snp->match == NULL)
       	  {
       	       printf("(NULL)");
       	  } else
       	  {
       	       printf("(of %d, le %d)",
       		      snp->match->offset,
       		      snp->match->len);
       	  }
       	  printf(", score %0.1f, total %0.1f\n",
       		 snp->match_score,
       		 snp->total_score);
	  
       	  snp = snp->prev;
     }
}

search_nodep
search_buffer(match_ctx ctx, /* IN */
       	      encode_match_f *f, /* IN */
       	      encode_match_data emd) /* IN */
{
     static search_node snp_arr[65536];
     float score_cache[65536];
     const_matchp mp;
     search_nodep snp;
     int i;
     int len = ctx->len;
     
     memset(snp_arr, 0, sizeof(snp_arr));
     
     snp = snp_arr[len];
     snp->index = len;
     snp->match->offset = 0;
     snp->match->len = 0;
     snp->match_score = 0;
     snp->total_score = 0;
     snp->prev = NULL;

     for(i = 0; i < 65536; ++i)
     {
	  score_cache[i] = 0.0;
     }

     /* think twice about changing this code,
      * it works the way it is. The last time
      * I examined this code I was certain it was
      * broken and broke it myself, trying to fix it. */
     while(len >= 0 &&
	   (mp = matches_get(ctx, (unsigned short)(len - 1))) != NULL)
     {
#if 0
	  printf("matches for index %d with total score %0.1f\n",
                 len - 1, snp->total_score);
#endif
	  
	  while(mp != NULL)
	  {
	       match local_mp;
	       unsigned short int i;
	       float score;
	       float prev_score;
	       float total_score;

	       
	       prev_score = snp_arr[len]->total_score;
	       local_mp->offset = mp->offset;

	       for(i = 1; i <= mp->len; ++i)
	       {
		    local_mp->len = i;

		    if(local_mp->offset != 1)
		    {
			 score = f(local_mp, emd);
		    } else 
		    {
			 score = score_cache[local_mp->len];
			 if(score == 0.0)
			 {
			      score = f(local_mp, emd);
			      score_cache[local_mp->len] = score;
			 }
		    }
		    total_score = prev_score + score;
#if 0		    
		    printf("%d: offset %d, len %d, score %0.1f, tot %0.1f\n",
			   len, mp->offset, mp->len, score, total_score);
#endif
		    snp = snp_arr[len - local_mp->len];
		    if((total_score < 1000000.0) &&
		       (snp->match->len == 0 ||
			total_score < snp->total_score))
		    {
			 snp->index = len - local_mp->len;
			 *snp->match = *local_mp;
			 snp->match_score = score;
			 snp->total_score = total_score;
			 snp->prev = snp_arr[len];
		    }
	       }
	       mp = mp->next;
	  }
	  /* slow way to get to the next node for cur */
	  while(snp_arr[--len]->match == NULL)
	  {
	       if(!(len & 0xFF))
	       {
		    fprintf(stdout, ".");
                    fflush(stdout);
	       }
	  }
	  if(!(len & 0xFF))
	  {
	       fprintf(stdout, ".");
               fflush(stdout);
	  }
     }
     fprintf(stdout, "\n");
     
     return snp_arr[0];
}

void
matchp_snp_get_enum(const_search_nodep snp, /* IN */
       		    matchp_snp_enum snpe) /* IN/OUT */
{
     snpe->startp = snp;
     snpe->currp = snp;
}

const_matchp
matchp_snp_enum_get_next(void *matchp_snp_enum)
{
     matchp_snp_enump snpe;
     const_matchp val;
     
     snpe = matchp_snp_enum;
     
     val = NULL;
     while(snpe->currp != NULL && val == NULL)
     {
       	  val = snpe->currp->match;
       	  snpe->currp = snpe->currp->prev;
     }
     
     if(snpe->currp == NULL)
     {
       	  snpe->currp = snpe->startp;
     }
     return val;
}


void
matchp_snp_rle_get_enum(const_search_nodep snp, /* IN */
       			matchp_snp_rle_enum snpe) /* IN/OUT */
{
     snpe->startp = snp;
     snpe->currp = snp;
}

const_matchp
matchp_snp_rle_enum_get_next(void *matchp_snp_enum)
{
     int c, p;
     matchp val;
     const_matchp mp;
     matchp_snp_rle_enump snpe;
     
     val = NULL;
     snpe = matchp_snp_enum;
     if(snpe->currp == NULL)
     {
	  snpe->currp = snpe->startp;
     } else
     {
	  val = snpe->m;
	  val->len = 1;
	  val->offset = 1;
	  mp = snpe->currp->match;
	  p = mp ? mp->offset == 0 : -2;
	  
	  while((snpe->currp = snpe->currp->prev) != NULL)
	  {
	       mp = snpe->currp->match;
	       if(mp == NULL || mp->len == 0)
	       {
		    continue;
	       }
	       c = p;	       
	       p = mp->offset == 0;
	       if(p != c)
	       {
		    break;
	       }
	       val->len += 1;
	  }
     }
     /*printf ("rle: len = %d\n", val ? val->len : -1);*/
     return val;
}
