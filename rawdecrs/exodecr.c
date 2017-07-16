/*
 * Copyright (c) 2005 Magnus Lind.
 *
 * This software is provided 'as-is', without any express or implied warranty.
 * In no event will the authors be held liable for any damages arising from
 * the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 *   1. The origin of this software must not be misrepresented * you must not
 *   claim that you wrote the original software. If you use this software in a
 *   product, an acknowledgment in the product documentation would be
 *   appreciated but is not required.
 *
 *   2. Altered source versions must be plainly marked as such, and must not
 *   be misrepresented as being the original software.
 *
 *   3. This notice may not be removed or altered from any distribution.
 *
 *   4. The names of this software and/or it's copyright holders may not be
 *   used to endorse or promote products derived from this software without
 *   specific prior written permission.
 */

#include "exodecr.h"

static unsigned short int base[52];
static char bits[52];
static unsigned short int bit_buffer;

#if 0
static unsigned short int
read_bits(const char **inp, int bit_count)
{
#ifdef BITS_AS_BYTES
    int byte_count = bit_count >> 3;
    bit_count &= 7;
#endif
    unsigned short int bits = 0;
    while(bit_count-- > 0)
    {
        if(bit_buffer == 1)
        {
            bit_buffer = 0x100 | (*--(*inp) & 0xff);
        }
        bits <<= 1;
        bits |= bit_buffer & 0x1;
        bit_buffer >>= 1;
    }
#ifdef BITS_AS_BYTES
    while (byte_count-- > 0)
    {
        bits <<= 8;
        bits |= *--(*inp) & 0xff;
    }
#endif
    return bits;
}
#else

static unsigned char shift_mask[] = {
    0b00000000, 0b10000000, 0b11000000, 0b11100000,
    0b11110000, 0b11111000, 0b11111100, 0b11111110,
    0b01000000, 0b10100000, 0b11010000, 0b11101000,
    0b11110100, 0b11111010, 0b11111101, 0b11111110};


static unsigned short int
read_bits(const char **inp, int bit_count)
{
    unsigned short int bits = shift_mask[bit_count];
    int carry = bit_count == 15;
    for (;;)
    {
        bits <<= 1;
        bits |= carry;
        carry = (bits & 0x100) != 0;
        bits &= 0xFF;

        if (carry == 0)
        {
            break;
        }

        if(bit_buffer == 1)
        {
            bit_buffer = 0x100 | (*--(*inp) & 0xff);
        }
        carry =  bit_buffer & 0x1;
        bit_buffer >>= 1;
    }

    if ((bits & 0x80) != 0)
    {
        bits &= 0x7F;
        bits <<= 8;
        bits |= *--(*inp) & 0xff;
    }

    return bits;
}
#endif

static void
init_table(const char **inp)
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

        b1 = read_bits(inp, 4);
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

    bit_buffer = *--in;

    init_table(&in);
    for(;;)
    {
        literal = read_bits(&in, 1);
        if(literal == 1)
        {
            /* literal byte */
            length = 1;
            goto copy;
        }
        index = 0;
        while(read_bits(&in, 1) == 0)
        {
            ++index;
        }
        if(index == 16)
        {
            break;
        }
        if(index == 17)
        {
            literal = 1;
#ifdef BITS_AS_BYTES
            length = (*--in & 0xFF) << 8;
            length |= *--in & 0xFF;
#else
            length = read_bits(&in, 16);
#endif
            goto copy;
        }
        length = base[index] + read_bits(&in, bits[index]);
        switch(length)
        {
        case 1:
            index = 48 + read_bits(&in, 2);
            break;
        case 2:
            index = 32 + read_bits(&in, 4);
            break;
        default:
            index = 16 + read_bits(&in, 4);
            break;
        }
        offset = base[index] + read_bits(&in, bits[index]);
    copy:
        do
        {
            --out;
            if(literal)
            {
                c = *--in;
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
