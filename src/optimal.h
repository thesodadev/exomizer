#ifndef ALREADY_INCLUDED_OPTIMAL_H
#define ALREADY_INCLUDED_OPTIMAL_H
/*
 * optimal.h
 *
 */
#include "search.h"
#include "output.h"

float
optimal_encode(const_matchp mp, /* IN */
	       encode_match_data emp); /* IN */

void
optimal_init(encode_match_data emp); /* IN/OUT */

void
optimal_free(encode_match_data emd); /* IN */

void
optimal_optimize(encode_match_data emd, /* IN/OUT */
		 matchp_enum_get_next_f *f, /* IN */
		 void *priv); /* IN */

void
optimal_dump(encode_match_data emp); /* IN */

void
optimal_out(output_ctx out, encode_match_data emd);

#endif
