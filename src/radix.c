/*
 * radix.c
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
