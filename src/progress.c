
#include "progress.h"
#include "log.h"
#include <string.h>

#define BAR_LENGTH 64

void progress_init(struct progress p[1], char *msg, int start, int end)
{
    if(start > end)
    {
        p->factor = (float)BAR_LENGTH / (end - start);
        p->offset = -start;
    }
    else
    {
        p->factor = (float)BAR_LENGTH / (start - end);
        p->offset = start;
    }
    p->last = -1;
    if(msg == NULL)
    {
        msg = "progressing_";
    }
    p->msg = msg;

    LOG(LOG_DEBUG, ("start %d, end %d, pfactor %f, poffset %d\n",
                    start, end, p->factor, p->offset));
}

void progress_bump(struct progress p[1], int pos)
{
    int fraction = ((pos + p->offset) * p->factor) + 0.5;
    while(fraction > p->last)
    {
        if(p->last == -1)
        {
            LOG(LOG_NORMAL, ("  % *s]\r [", BAR_LENGTH, ""));
        }
        else
        {
            LOG(LOG_NORMAL, ("%c", p->msg[p->last % strlen(p->msg)]));
        }
        p->last += 1;
    }
}

void progress_free(struct progress p[1])
{
}
