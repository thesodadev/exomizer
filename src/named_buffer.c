#include "named_buffer.h"
#include "log.h"
#include "vec.h"
#include "membuf_io.h"

#include <stdlib.h>

struct sbe
{
    const char *name;
    struct membuf mb[1];
};

static struct vec s_sbe_table[1];

static int sbe_cmp(const void *a, const void *b)
{
    struct sbe *sbe_a;
    struct sbe *sbe_b;
    int val;

    sbe_a = (struct sbe*)a;
    sbe_b = (struct sbe*)b;

    val = strcmp(sbe_a->name, sbe_b->name);

    return val;
}

void sbe_free(struct sbe *e)
{
    membuf_free(e->mb);
}

void named_buffer_init()
{
    vec_init(s_sbe_table, sizeof(struct sbe));
}

void named_buffer_free()
{
    typedef void cb_free(void *a);

    vec_free(s_sbe_table, (cb_free*)sbe_free);
}

struct membuf *new_named_buffer(const char *name)
{
    int pos;
    struct sbe e[1];
    struct sbe *ep;
    struct membuf *mp;

    /* name is already strdup:ped */
    e->name = name;
    pos = vec_find(s_sbe_table, sbe_cmp, e);
    if(pos >= 0)
    {
        /* found */
        LOG(LOG_ERROR, ("buffer already exists.\n"));
        exit(-1);
    }
    membuf_init(e->mb);
    ep = vec_insert(s_sbe_table, -(pos + 2), e);
    mp = ep->mb;

    return mp;
}

struct membuf *get_named_buffer(const char *name)
{
    int pos;
    struct sbe e[1];
    struct sbe *ep;
    struct membuf *mp;

    /* name is already strdup:ped */
    e->name = name;
    pos = vec_find(s_sbe_table, sbe_cmp, e);
    if(pos >= 0)
    {
        /* found */
        ep = vec_get(s_sbe_table, pos);
    }
    else
    {
        membuf_init(e->mb);
        read_file(name, e->mb);
        ep = vec_insert(s_sbe_table, -(pos + 2), e);
    }
    mp = ep->mb;

    return mp;
}
