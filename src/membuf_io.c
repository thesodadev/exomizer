#include "membuf_io.h"
#include "log.h"
#include <stdio.h>
#include <stdlib.h>

void read_file(const char *name, struct membuf *buf)
{
    char block[1024];
    FILE *in;
    int len;

    in = fopen(name, "rb");
    if(in == NULL)
    {
        LOG(LOG_ERROR, ("Can't open file \"%s\" for input.\n", name));
        exit(-1);
    }
    do
    {
        len = fread(block, 1, 1024, in);
        membuf_append(buf, block, len);
    }
    while(len == 1024);
    LOG(LOG_DEBUG, ("read %d bytes from file\n", len));
    fclose(in);
}

void write_file(const char *name, struct membuf *buf)
{
    FILE *out;
    out = fopen(name, "wb");
    if(out == NULL)
    {
        LOG(LOG_ERROR, ("Can't open file \"%s\" for output.\n", name));
        exit(-1);
    }
    fwrite(membuf_get(buf), 1, membuf_memlen(buf), out);
    fclose(out);
}
