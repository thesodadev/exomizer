#ifndef EXO_HELPER_ALREADY_INCLUDED
#define EXO_HELPER_ALREADY_INCLUDED

/*
 * Copyright (c) 2005, 2013, 2015 Magnus Lind.
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

#include "log.h"
#include "membuf.h"
#include "search.h"
#include "optimal.h"
#include "flags.h"

#define CRUNCH_OPTIONS_DEFAULT {NULL, 65535, 65535, 65535, 0, 1, 0, 0, \
                                optimal_encode}

struct common_flags
{
    struct crunch_options *options;
    const char *outfile;
};

#define CRUNCH_FLAGS "cCe:Em:M:p:PA:o:qBv"
#define BASE_FLAGS "o:qBv"

void print_crunch_flags(enum log_level level, const char *default_outfile);

void print_base_flags(enum log_level level, const char *default_outfile);

typedef void print_usage_f(const char *appl, enum log_level level,
                           const char *default_outfile);

void handle_crunch_flags(int flag_char, /* IN */
                         const char *flag_arg, /* IN */
                         print_usage_f *print_usage, /* IN */
                         const char *appl, /* IN */
                         struct common_flags *options); /* OUT */

void handle_base_flags(int flag_char, /* IN */
                       const char *flag_arg, /* IN */
                       print_usage_f *print_usage, /* IN */
                       const char *appl, /* IN */
                       const char **default_outfilep); /* OUT */

    /*
     * bit 0  Controls bit bit orientation: 0=normal (new), 1=reversed (old)
     * bit 1  Sequences with length 1: 0=enable,1=disable
     * bit 2  Sequences with length > 255 where (length & 255) would have been
     *        using its own decrunch table: 0=enable, 1=disable
     * bit 3  Decides if we are to have two lengths (1 and 2) or three lengths
     *        (1, 2 and 3) using dedicated decrunch tables: 0=two, 1=three
     * bit 4  Contols how more than 7 bits are shifted 0=split into a shift of
     *        of less than 8 bits + a byte (new), 1=all bits are shifted (old)
     * bit 5  Align bit stream towards start without flag: 1=enable, 0=disable
     * bit 6  Implicit first literal byte: 0=enable, 1=disable
     */

struct crunch_options
{
    const char *exported_encoding;
    int max_passes;
    int max_len;
    int max_offset;
    int favor_speed;
    int output_header;
    /*
     * bit 0  Controls bit bit orientation: 0=normal (new), 1=reversed (old)
     * bit 3  Decides if we are to have two lengths (1 and 2) or three lengths
     *        (1, 2 and 3) using dedicated decrunch tables: 0=two, 1=three
     * bit 4  Contols how more than 7 bits are shifted 0=split into a shift of
     *        of less than 8 bits + a byte (new), 1=all bits are shifted (old)
     * bit 5  Align bit stream towards start without flag: 1=enable, 0=disable
     * bit 6  Implicit first literal byte: 0=enable, 1=disable
     */
    int flags_proto;
    /*
     * bit 1  Sequences with length 1: 0=enable,1=disable
     * bit 2  Sequences with length > 255 where (length & 255) would have been
     *        using its own decrunch table: 0=enable, 1=disable
     * bit 3  Literal sequences: 0=enable,1=disable
     */
    int flags_avoid;
    encode_match_f *encode;
};

#define STATIC_CRUNCH_INFO_INIT {0, 0}
struct crunch_info
{
    int literal_sequences_used;
    int needed_safety_offset;
};

void print_license(void);

void crunch_backwards(struct membuf *inbuf,
                      struct membuf *outbuf,
                      const struct crunch_options *options, /* IN */
                      struct crunch_info *info); /* OUT */

void crunch(struct membuf *inbuf,
            struct membuf *outbuf,
            const struct crunch_options *options, /* IN */
            struct crunch_info *info); /* OUT */

struct decrunch_options
{
    /* see crunch_options flags field */
    int flags_proto;
    /* 0 backward, 1 forward */
    int direction;
};

void decrunch(int level,
              struct membuf *inbuf,
              struct membuf *outbuf,
              struct decrunch_options *dopts);

void reverse_buffer(char *start, int len);

#endif
