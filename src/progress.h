#ifndef ALREADY_INCLUDED_PROGRESS
#define ALREADY_INCLUDED_PROGRESS

struct progress
{
    char *msg;
    float factor;
    int offset;
    int last;
};

void progress_init(struct progress p[1], char *msg, int start, int end);

void progress_bump(struct progress p[1], int pos);

void progress_free(struct progress p[1]);

#endif
