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
 * radix.c, a part of the exomizer v1.0beta2 release
 *
 */

#include "radix.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define RADIX_TREE_NODE_RADIX 11U
#define RADIX_TREE_NODE_MASK  ((1U << RADIX_TREE_NODE_RADIX) - 1U)

struct _radix_node {
     struct _radix_node *rn;
};

void
radix_tree_init(radix_root rr) /* IN */
{
     rr->depth = 0;
     rr->root = NULL;
}

static
void
radix_tree_free_helper(int depth,
		       radix_nodep rnp,
		       free_callback *f, /* IN */
		       void *priv) /* IN */
{
     int i;
     do {
	  if(depth == 0)
	  {
	       /* do something to the data pointer? */
	       if(f != NULL)
	       {
		    f(rnp, priv);
	       }
	       break;
	  }
	  if(rnp == NULL)
	  {
	       /* tree not grown here */
	       break;
	  }

	  for(i = RADIX_TREE_NODE_MASK; i >= 0; --i)
	  {
	       radix_tree_free_helper(depth - 1, rnp[i].rn, f, priv);
	       rnp[i].rn = NULL;
	  }
	  free(rnp);
     } while (0);
}

void
radix_tree_free(radix_root rr, /* IN */
		free_callback *f, /* IN */
		void *priv) /* IN */
{
     radix_tree_free_helper(rr->depth, rr->root, f, priv);
     rr->depth = 0;
     rr->root = NULL;
}

void
radix_node_set(radix_rootp rrp, /* IN */
	       unsigned int index, /* IN */
	       void *data) /* IN */
{
     radix_nodep rnp;
     radix_nodep *rnpp;
     unsigned int mask;
     int depth;

     mask = ~0U << (RADIX_TREE_NODE_RADIX * rrp->depth);
     while(index & mask)
     {
	  /*fprintf(stderr, "calloc called\n");*/
	  /* not deep enough, let's deepen the tree */
	  rnp = calloc(1 << RADIX_TREE_NODE_RADIX, sizeof(void *));
	  rnp[0].rn = rrp->root;
	  rrp->root = rnp;
	  rrp->depth += 1;

	  mask = ~0U << (RADIX_TREE_NODE_RADIX * rrp->depth);
     }

     /* go down */
     rnpp = &rrp->root;
     for(depth = rrp->depth - 1; depth >= 0; --depth)
     {
	  unsigned int node_index;

	  if(*rnpp == NULL)
	  {
	       /*fprintf(stderr, "calloc called\n");*/
	       /* tree is not grown in this interval */
	       *rnpp = calloc(1 << RADIX_TREE_NODE_RADIX, sizeof(void *));
	       
	  }
	  node_index = ((index >> (RADIX_TREE_NODE_RADIX * depth)) &
			 RADIX_TREE_NODE_MASK);
	  
	  rnpp = &((*rnpp)[node_index].rn);
     }
     *rnpp = data;
}

void *
radix_node_get(radix_root rr, /* IN */
	       unsigned int index) /* IN */
{
     radix_nodep rnp;
     unsigned short int depth;

     /* go down */
     rnp = rr->root;
     for(depth = rr->depth - 1; depth < 0xffff; --depth)
     {
	  unsigned short int node_index;

	  if(rnp == NULL)
	  {
	       /* tree is not grown in this interval */
	       break;
	  }
	  node_index = ((index >> (RADIX_TREE_NODE_RADIX * depth)) &
			 RADIX_TREE_NODE_MASK);
	  
	  rnp = rnp[node_index].rn;
     }
     return rnp;
}
