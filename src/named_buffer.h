#ifndef NAMED_BUFFER_INCLUDED
#define NAMED_BUFFER_INCLUDED

#include "membuf.h"

void named_buffer_init();
void named_buffer_free();

struct membuf *new_named_buffer(const char *name);
struct membuf *get_named_buffer(const char *name);

#endif
