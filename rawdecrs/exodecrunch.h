#ifndef EXO_DECRUNCH_ALREADY_INCLUDED
#define EXO_DECRUNCH_ALREADY_INCLUDED

#define MAX_OFFSET 65535

typedef int exo_read_crunched_byte(void *data);

struct exo_decrunch_ctx *
exo_decrunch_new(unsigned short int max_offset,
                 exo_read_crunched_byte *read_byte,
                 void *read_data);

int
exo_read_decrunched_byte(struct exo_decrunch_ctx *ctx);

void
exo_decrunch_delete(struct exo_decrunch_ctx *ctx);

#endif /* EXO_DECRUNCH_ALREADY_INCLUDED */
