#ifndef EXO_HELPER_ALREADY_INCLUDED
#define EXO_HELPER_ALREADY_INCLUDED

int
str_to_int(const char *str, int *value);

int crunch_backwards(struct membuf *inbuf,
                     struct membuf *outbuf,
                     const char *exported_encoding,
                     int max_passes,
                     int max_offset);

int crunch(struct membuf *inbuf,
           struct membuf *outbuf,
           const char *exported_encoding,
           int max_passes,
           int max_offset);

void decrunch(int level,
              struct membuf *inbuf,
              struct membuf *outbuf);

void decrunch_backwards(int level,
                        struct membuf *inbuf,
                        struct membuf *outbuf);

void reverse_buffer(char *start, int len);
#endif
