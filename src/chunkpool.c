
#include <stdlib.h>
#include "chunkpool.h"
#include "log.h"

void
chunkpool_init(struct chunkpool *ctx, int size)
{
    ctx->chunk_size = size;
    ctx->chunk = -1;
    ctx->chunk_pos = 0;
    ctx->chunk_max = (0x1fffff / size) * size;
}

void
chunkpool_free(struct chunkpool *ctx)
{
    while(ctx->chunk >= 0)
    {
	free(ctx->chunks[ctx->chunk]);
	ctx->chunk -= 1;
    }
    ctx->chunk_pos = 0;
}

void *
chunkpool_malloc(struct chunkpool *ctx)
{
    void *p;
    if(ctx->chunk_pos == 0)
    {
	void *m;
	if(ctx->chunk == 31)
	{
	    LOG(LOG_ERROR, ("out of chunks in file %s, line %d\n",
			    __FILE__, __LINE__));
	    LOG(LOG_BRIEF, ("chunk_size %d\n", ctx->chunk_size));
	    LOG(LOG_BRIEF, ("chunk_max %d\n", ctx->chunk_max));
	    LOG(LOG_BRIEF, ("chunk %d\n", ctx->chunk));
	    exit(1);
	}
	m = malloc(ctx->chunk_max);
	if (m == NULL)
	{
	    LOG(LOG_ERROR, ("out of memory error in file %s, line %d\n",
			    __FILE__, __LINE__));
	    exit(1);
	}
	ctx->chunk += 1;
	ctx->chunks[ctx->chunk] = m;
    }
    p = (char*)ctx->chunks[ctx->chunk] + ctx->chunk_pos;
    ctx->chunk_pos += ctx->chunk_size;
    if(ctx->chunk_pos >= ctx->chunk_max)
    {
	ctx->chunk_pos = 0;
    }
    return p;
}

void *
chunkpool_calloc(struct chunkpool *ctx)
{
    void *p = chunkpool_malloc(ctx);
    memset(p, 0, ctx->chunk_size);
    return p;
}
