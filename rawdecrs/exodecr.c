
#include "exodecr.h"

static unsigned short int base[52];
static char bits[52];
static unsigned short int bit_buffer;
static const char *inp;


static unsigned char
read_byte()
{
    return (unsigned char)(*--inp);
}

static unsigned short int
read_bits(int bit_count)
{
    unsigned short int bits = 0;
    while(bit_count-- > 0)
    {
        if(bit_buffer == 1)
        {
            bit_buffer = 0x100 | read_byte();
        }
        bits <<= 1;
        bits |= bit_buffer & 0x1;
        bit_buffer >>= 1;
    }
    return bits;
}

static void
init_table()
{
    int i;
    unsigned short int b2;

    for(i = 0; i < 52; ++i)
    {
        unsigned short int b1;
        if((i & 15) == 0)
        {
            b2 = 1;
        }
        base[i] = b2;

        b1 = read_bits(4);
        bits[i] = b1;

        b2 += 1 << b1;
    }
}

char *
exo_decrunch(const char *in, char *out)
{
    unsigned short int index;
    unsigned short int length;
    unsigned short int offset;
    char c;
    char literal;

    inp = in;

    bit_buffer = read_byte();

    init_table();
    for(;;)
    {
        literal = read_bits(1);
        if(literal == 1)
        {
            /* literal byte */
            length = 1;
            goto copy;
        }
        index = 0;
        while(read_bits(1) == 0)
        {
            ++index;
        }
        if(index == 16)
        {
            literal = 1;
            length = read_bits(16);
            goto copy;
        }
        if(index == 17)
        {
            break;
        }
        length = base[index] + read_bits(bits[index]);
        switch(length)
        {
        case 1:
            index = 48 + read_bits(2);
            break;
        case 2:
            index = 32 + read_bits(4);
            break;
        default:
            index = 16 + read_bits(4);
            break;
        }
        offset = base[index] + read_bits(bits[index]);
    copy:
        do
        {
            --out;
            if(literal)
            {
                c = read_byte();
            }
            else
            {
                c = out[offset];
            }
            *out = c;
        }
        while(--length > 0);
    }
    return out;
}
