#ifndef ALREADY_INCLUDED_CHUNKPOOL_H
#define ALREADY_INCLUDED_CHUNKPOOL_H

struct chunkpool {
    int chunk_size;
    int chunk;
    int chunk_pos;
    int chunk_max;
    void *chunks[32];
};

void
chunkpool_init(struct chunkpool *ctx, int size);

void
chunkpool_free(struct chunkpool *ctx);

void *
chunkpool_malloc(struct chunkpool *ctx);

void *
chunkpool_calloc(struct chunkpool *ctx);

#endif
