#ifndef EXO_HELPER_ALREADY_INCLUDED
#define EXO_HELPER_ALREADY_INCLUDED

int
str_to_int(const char *str, int *value);

#define CRUNCH_OPTIONS_DEFAULT {NULL, 65535, 65535, 1}

struct common_flags
{
    struct crunch_options *options;
    const char *outfile;
};

#define SHARED_FLAGS "ce:m:o:p:qv"

void print_shared_flags(enum log_level level);

typedef void print_usage_f(const char *appl, enum log_level level);

void handle_shared_flags(int flag_char, /* IN */
                         const char *flag_arg, /* IN */
                         print_usage_f *print_usage, /* IN */
                         const char *appl, /* IN */
                         struct common_flags *options); /* OUT */

const char *fixup_appl(char *appl);

struct crunch_options
{
    const char *exported_encoding;
    int max_passes;
    int max_offset;
    int use_literal_sequences;
};

struct crunch_info
{
    int literal_sequences_used;
    int needed_safety_offset;
};

void crunch_backwards(struct membuf *inbuf,
                      struct membuf *outbuf,
                      struct crunch_options *options, /* IN */
                      struct crunch_info *info); /* OUT */

void crunch(struct membuf *inbuf,
            struct membuf *outbuf,
            struct crunch_options *options, /* IN */
            struct crunch_info *info); /* OUT */

void decrunch(int level,
              struct membuf *inbuf,
              struct membuf *outbuf);

void decrunch_backwards(int level,
                        struct membuf *inbuf,
                        struct membuf *outbuf);

void reverse_buffer(char *start, int len);
#endif
