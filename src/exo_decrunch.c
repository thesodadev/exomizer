
#define EXO_STATUS_DONE     0
#define EXO_STATUS_ERROR   -1
#define EXO_STATUS_INBUF   -2
#define EXO_STATUS_OUTBUF  -3

struct exo_interval
{
    unsigned short offset;
    char bits;
}

struct exo_buffer
{
    int size;
    unsigned char *data;
}

struct exo_buffers
{
    struct exo_buffer *inbuf[1];
    struct exo_buffer *outbuf[1];
    struct exo_buffer *window[1];
};

struct exo_decr
{
    int state;
    int len;
    int offset;
    unsigned int bitbuf;

    int inbuf_pos;
    int outbuf_pos;
    int window_pos;

    struct exo_buffers *bufs;

    struct exo_interval lengths[16];
    struct exo_interval offsets3[16];
    struct exo_interval offsets2[16];
    struct exo_interval offsets1[4];
};

#define EXO_STATE_INIT      1
#define EXO_STATE_DECRUNCH0 2
#define EXO_STATE_DECRUNCH1 3
#define EXO_STATE_DECRUNCH2 4
#define EXO_STATE_DECRUNCH3 5

#define GET_BITS(BIT_COUNT, VAR_NAME) \
    { VAR_NAME = get_bits(BIT_COUNT); if(VAR_NAME < 0) goto exit; }


void exo_decr_init(struct exo_decr *ctx, struct exo_buffers *bufs;)
{
    ctx->state = INIT_L;
    ctx->len = 0;

    ctx->inbuf_pos = 0;
    ctx->outbuf_pos = 0;
    ctx->window_pos = 0;

    ctx->bufs = bufs;
}

static int bitstream_get(struct exo_decr *ctx)
{
}

static void bitstream_clear_undo(struct exo_decr *ctx)
{
}

int exo_decrunch(struct exo_decr *ctx)
{
    int ret = 0;
    int count;
    int bits;
    int offset;
    switch(ctx->state)
    {
    case EXO_STATE_INIT:
        while(ctx->len < 56)
        {
            offset = 1;
            GET_BITS(4, bits);
            if(ctx->len > 0)
            {
                offset = ctx->lengths[ctx->len - 1].offset;
            }
            ctx->lengths[ctx->len].bits = bits;
            ctx->lengths[ctx->len].offset = offset;
            ++ctx->len;
            bitstream_clear_undo(ctx);
        }

        ctx->state = EXO_STATE_DECRUNCH;
    case EXO_STATE_DECRUNCH0:
    restart:
        /* literal1 or sequence? */
        GET_BITS(1, bits);
        if(bits == 1) goto literal1;

        /* sequence, get gamma count */
        count = 0;
        while(bits == 0)
        {
            ++count;
            GET_BITS(1, bits);
        }

        bitstream_clear_undo(ctx);
        ctx->state = EXO_STATE_DECRUNCH1;
    case EXO_STATE_DECRUNCH1:
        /* get sequence length */
        GET_BITS(ctx->lengths[count].bits, bits);
        ctx->length = ctx->lengths[count].offset + bits;

        bitstream_clear_undo(ctx);
        ctx->state = EXO_STATE_DECRUNCH2;
    case EXO_STATE_DECRUNCH2:
        /* get sequence offset */
        switch(ctx->length)
        {
        case 1:
            GET_BITS(2, count);
            GET_BITS(ctx->offset1[count].bits, bits);
            ctx->offset = ctx->offset1[count].offset + bits;
            break;
        case 2:
            GET_BITS(4, count);
            GET_BITS(ctx->offset2[count].bits, bits);
            ctx->offset = ctx->offset2[count].offset + bits;
            break;
        default:
            GET_BITS(4, count);
            GET_BITS(ctx->offset3[count].bits, bits);
            ctx->offset = ctx->offset3[count].offset + bits;
            break;
        }
        bitstream_clear_undo(ctx);
        ctx->state = EXO_STATE_DECRUNCH3;
    case EXO_STATE_DECRUNCH3:
        /* copy the sequence */
        if(offset > offset_size)
        {
            ctx->state = EXO_STATE_ERROR;
            ret = EXO_STATUS_ERROR_WINDOW_TOO_SMALL;
            goto exit:
        }
        offset = ctx->window_pos - ctx->offset;
        if(offset < 0) offset += ctx->window_size;

        while(ctx->len > 0)
        {
            bits = ctx->window[offset++];
        literal:
            byte_output(ctx, bits);

            --ctx->len;
        }
        bitstream_clear_undo(ctx);
        ctx->state = EXO_STATE_DECRUNCH0;
        goto restart;
    }
 exit:
}
