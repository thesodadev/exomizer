#ifndef MEMBUF_IO_ALREADY_INCLUDED
#define MEMBUF_IO_ALREADY_INCLUDED

#include "membuf.h"
void read_file(const char *name, struct membuf *buf);
void write_file(const char *name, struct membuf *buf);

#endif
