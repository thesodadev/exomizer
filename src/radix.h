#ifndef ALREADY_INCLUDED_RADIX_H
#define ALREADY_INCLUDED_RADIX_H
/*
 * radix.h
 *
 */

typedef struct _radix_node *radix_nodep;

struct _radix_root {
     int depth;
     radix_nodep root;
};

typedef struct _radix_root radix_root[1];
typedef struct _radix_root *radix_rootp;


typedef void free_callback(void *data, void *priv);

/* *f will be called even for null pointers */
void
radix_tree_free(radix_root rr, /* IN */
		free_callback *f, /* IN */
		void *priv); /* IN */

void
radix_tree_init(radix_root rr); /* IN */

void
radix_node_set(radix_root rr, /* IN */
	       unsigned int index, /* IN */
	       void *data); /* IN */

void *
radix_node_get(radix_root rr, /* IN */
	       unsigned int index); /* IN */

#endif

