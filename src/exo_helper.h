#ifndef EXO_HELPER_ALREADY_INCLUDED
#define EXO_HELPER_ALREADY_INCLUDED

int
str_to_int(const char *str, int *value);

struct crunch_info
{
    int literal_sequences_used;
    int needed_safety_offset;
};

void crunch_backwards(struct membuf *inbuf,
                      struct membuf *outbuf,
                      const char *exported_encoding,
                      int max_passes,
                      int max_offset,
                      int use_literal_sequences,
                      struct crunch_info *info); /* OUT */

void crunch(struct membuf *inbuf,
            struct membuf *outbuf,
            const char *exported_encoding,
            int max_passes,
            int max_offset,
            int use_literal_sequences,
            struct crunch_info *info); /* OUT */

void decrunch(int level,
              struct membuf *inbuf,
              struct membuf *outbuf);

void decrunch_backwards(int level,
                        struct membuf *inbuf,
                        struct membuf *outbuf);

void reverse_buffer(char *start, int len);
#endif
