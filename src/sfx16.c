/*
 * Copyright (c) 2002 Magnus Lind.
 *
 * This software is provided 'as-is', without any express or implied warranty.
 * In no event will the authors be held liable for any damages arising from
 * the use of this software.
 *
 * Permission is granted to anyone to use this software, alter it and re-
 * distribute it freely for any non-commercial, non-profit purpose subject to
 * the following restrictions:
 *
 *   1. The origin of this software must not be misrepresented; you must not
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
 *
 * This file is a part of the Exomizer v1.1 release
 *
 */

#include <stdlib.h>
#include "sfx.h"
#include "log.h"
#include "output.h"

#define DECOMP_EFFECT_ADDR 0x0fe7
#define DECOMP_MIN_ADDR 0x0c00
#define DECOMP_SAFETY_SOFT 48
#define DECOMP_SAFETY_HARD 24
#define DECOMP_LEN 0xba

static const unsigned char stage1[] = {
    0x01, 0x10, 0x0B, 0x10, 0xD2, 0x07, 0x9E, 0x34,
    0x31, 0x30, 0x39, 0x00, 0x00, 0x00, 0xA0, 0x00,
    0x78, 0x8D, 0x3F, 0xFF, 0xA2, DECOMP_LEN + 3, 0xBD, 0x00,
    0x00, 0x9D, 0xFC, 0x00, 0xCA, 0xD0, 0xF7, 0x4C,
    0x00, 0x00
};
#define STAGE1_COPY_SRC 23
#define STAGE1_JMP_STAGE2 32

#define STAGE1_BEGIN 0x0fff
#define STAGE1_END   0x1021

static unsigned char stage2[] = {
    0xE8, 0xA9, 0x00, 0x85, 0xFC, 0x85, 0xFB, 0xE0,
    0x01, 0x90, 0x27, 0xA5, 0xFD, 0x4A, 0xD0, 0x17,
    0xAD, 0x22, 0x01, 0xD0, 0x09, 0xBD, 0xFD, 0x00,
    0x8D, 0xE7, 0x0B, 0xCE, 0x23, 0x01, 0xCE, 0x22,
    0x01, 0xAD, 0x1B, 0x08, 0x90, 0x1C, 0x6A, 0x26,
    0xFC, 0x26, 0xFB, 0xCA, 0xD0, 0xDF, 0x85, 0xFD,
    0xA5, 0xFC, 0x60, 0x8D, 0x3E, 0xFF, 0x58, 0x4C,
    0x00, 0xC6, 0xCA, 0xC6, 0xFF, 0xC6, 0xAF, 0x88,
    0xB1, 0xAE, 0x91, 0xFE, 0x98, 0xD0, 0xF8, 0x8A,
    0xD0, 0xF0, 0x20, 0x00, 0x01, 0xF0, 0x0A, 0xA5,
    0xFE, 0xD0, 0x02, 0xC6, 0xFF, 0xC6, 0xFE, 0x90,
    0xB7, 0xC8, 0x20, 0x00, 0x01, 0xF0, 0xFA, 0xC0,
    0x11, 0xB0, 0xD0, 0xBE, 0x33, 0x03, 0x20, 0x01,
    0x01, 0x79, 0x67, 0x03, 0x85, 0xA7, 0xA5, 0xFB,
    0x79, 0x9B, 0x03, 0x48, 0xD0, 0x06, 0xA4, 0xA7,
    0xC0, 0x04, 0x90, 0x02, 0xA0, 0x03, 0xBE, 0xB3,
    0x01, 0x20, 0x01, 0x01, 0x79, 0xB6, 0x01, 0xA8,
    0x38, 0xA5, 0xFE, 0xE5, 0xA7, 0x85, 0xFE, 0xB0,
    0x02, 0xC6, 0xFF, 0xBE, 0x34, 0x03, 0x20, 0x01,
    0x01, 0x79, 0x68, 0x03, 0x90, 0x03, 0xE6, 0xFB,
    0x18, 0x65, 0xFE, 0x85, 0xAE, 0xA5, 0xFB, 0x79,
    0x9C, 0x03, 0x65, 0xFF, 0x85, 0xAF, 0xA4, 0xA7,
    0x68, 0xAA, 0x90, 0x90, 0x02, 0x04, 0x04, 0x30,
    0x20, 0x10, 0xE8, 0x98, 0x29, 0x0F, 0xF0, 0x13,
    0x8A, 0x4A, 0xA6, 0xFC, 0x2A, 0x26, 0xFB, 0xCA,
    0x10, 0xFA, 0x79, 0x67, 0x03, 0xAA, 0xA5, 0xFB,
    0x79, 0x9B, 0x03, 0x99, 0x9C, 0x03, 0x8A, 0x99,
    0x68, 0x03, 0xA2, 0x04, 0x20, 0x01, 0x01, 0x99,
    0x34, 0x03, 0xC8, 0xC0, 0x34, 0xD0, 0xD3, 0xA0,
    0x00, 0x8C, 0xE7, 0x07, 0x4C, 0x4A, 0x01
};
#define STAGE2_GET_BYTE     34
#define STAGE2_START        56
#define STAGE2_COPY_LEN_LO 232

static unsigned char stage3s[] = {
    0xB9, 0x00, 0x00, 0x99, 0x00, 0x00, 0x88, 0xD0,
    0xF7, 0x4C, 0x4A, 0x01
};
#define STAGE3S_COPY_SRC    1
#define STAGE3S_COPY_DEST   4

static unsigned char stage3l[] = {
    0xA2, 0x00, 0xB0, 0x0E, 0xCA, 0xCE, 0x1A, 0x09,
    0xCE, 0x1D, 0x09, 0x88, 0xB9, 0x00, 0x00, 0x99,
    0x00, 0x00, 0x98, 0xD0, 0xF6, 0x8A, 0xD0, 0xEC,
    0x4C, 0x4A, 0x01
};
#define STAGE3L_COPY_LEN_HI  1
#define STAGE3L_COPY_DEC_SRC_HI 6
#define STAGE3L_COPY_DEC_DEST_HI  9
#define STAGE3L_COPY_SRC    13
#define STAGE3L_COPY_DEST   16

static unsigned int L_copy_len;
static
void load(output_ctx out,       /* IN/OUT */
          unsigned short int load)      /* IN */
{
    unsigned short int new_load;
    if (load < DECOMP_MIN_ADDR)
    {
        LOG(LOG_ERROR,
	    ("error: cant handle load address < $%04X\n",
	     DECOMP_MIN_ADDR));
        exit(1);
    }

    output_ctx_set_start(out, STAGE1_BEGIN);

    /* set L_load to just after stage1 code */
    new_load = STAGE1_END;

    /* do we have enough decompression buffer safety? */
    if (load - DECOMP_SAFETY_HARD < STAGE1_END)
    {
        /* no, we need to transfer bytes */
        new_load = load - DECOMP_SAFETY_SOFT;
        /* make sure that the depack effect is visible */
	if(new_load > DECOMP_EFFECT_ADDR)
	{
	    new_load = DECOMP_EFFECT_ADDR;
	}
        /* use the hard safety if we are lower than the min addr */
	if(new_load < DECOMP_MIN_ADDR - DECOMP_SAFETY_HARD)
        {
	    new_load = DECOMP_MIN_ADDR - DECOMP_SAFETY_HARD;
        }
    }
    output_set_pos(out, new_load);

    L_copy_len = STAGE1_END - new_load;

    LOG(LOG_DUMP, ("copy_len $%04X\n", L_copy_len));
    LOG(LOG_DUMP, ("new_load $%04X\n", new_load));

}

static
void stages(output_ctx out,     /* IN/OUT */
            unsigned short int start)   /* IN */
{
    int i;
    int stage2_begin;
    int stage3_begin = 0;
    int stage3_end = 0;
    int stages_end;

    stage2_begin = output_get_pos(out);
    /*LOG(LOG_DUMP, ("stage2_begin $%04X\n", stage2_begin)); */

    for (i = 0; i < sizeof(stage2); ++i)
    {
        output_byte(out, stage2[i]);
    }
    if (L_copy_len > 0)
    {
        /* add stage 3 */
        LOG(LOG_DUMP,
            ("adding extra copy stage, copy_len %d\n", L_copy_len));

        /* clobber the jmp last in stage 2 */

        stage3_begin = output_get_pos(out) - 6;
        /*LOG(LOG_DUMP, ("stage3_begin $%04X\n", stage3_begin)); */
        output_set_pos(out, stage3_begin);

        /* copy stage 3 */
        if (L_copy_len > 256)
        {
            for (i = 0; i < sizeof(stage3l); ++i)
            {
                output_byte(out, stage3l[i]);
            }
        } else
        {
            for (i = 0; i < sizeof(stage3s); ++i)
            {
                output_byte(out, stage3s[i]);
            }
        }

        stage3_end = output_get_pos(out);

        /* copy the actual bytes */
        output_copy_bytes(out, STAGE1_END - L_copy_len, L_copy_len);
    }

    stages_end = output_get_pos(out);
    /*LOG(LOG_DUMP, ("stages_end $%04X\n", stages_end)); */

    /* add stage 1 */
    output_set_pos(out, STAGE1_BEGIN);
    for (i = 0; i < sizeof(stage1); ++i)
    {
        output_byte(out, stage1[i]);
    }

    /* fixup addresses */
    output_set_pos(out, STAGE1_BEGIN + STAGE1_COPY_SRC);
    output_word(out, (unsigned short int) (stage2_begin - 4));

    output_set_pos(out, STAGE1_BEGIN + STAGE1_JMP_STAGE2);
    output_word(out, (unsigned short int) (stage2_begin + DECOMP_LEN));

    output_set_pos(out, stage2_begin + STAGE2_GET_BYTE);
    output_word(out, (unsigned short int) (stage2_begin - 3));

    output_set_pos(out, stage2_begin + STAGE2_START);
    output_word(out, (unsigned short int) start);

    if (L_copy_len > 256)
    {
        /* fixup additional stage 3 stuff */
        output_set_pos(out, stage2_begin + STAGE2_COPY_LEN_LO);
        output_byte(out, (unsigned char) (L_copy_len & 0xff));

        output_set_pos(out, stage3_begin + STAGE3L_COPY_LEN_HI);
        output_byte(out, (unsigned char) ((L_copy_len >> 8) & 0xff));

        output_set_pos(out, stage3_begin + STAGE3L_COPY_DEC_SRC_HI);
        output_word(out,
                    (unsigned short int) (stage3_begin + STAGE3L_COPY_SRC +
                                          1));

        output_set_pos(out, stage3_begin + STAGE3L_COPY_DEC_DEST_HI);
        output_word(out,
                    (unsigned short int) (stage3_begin +
                                          STAGE3L_COPY_DEST + 1));

        output_set_pos(out, stage3_begin + STAGE3L_COPY_SRC);
        output_word(out,
                    (unsigned short int) (stage3_end +
                                          (L_copy_len & 0xff00)));

        output_set_pos(out, stage3_begin + STAGE3L_COPY_DEST);
        output_word(out,
                    (unsigned short int) (STAGE1_END -
                                          (L_copy_len & 0x00ff)));

    } else if (L_copy_len > 0)
    {
        int adjust = (L_copy_len != 256);
        /* fixup additional stage 3 stuff */
        output_set_pos(out, stage2_begin + STAGE2_COPY_LEN_LO);
        output_byte(out, (unsigned char) L_copy_len);

        output_set_pos(out, stage3_begin + STAGE3S_COPY_SRC);
        output_word(out, (unsigned short int) (stage3_end - adjust));

        output_set_pos(out, stage3_begin + STAGE3S_COPY_DEST);
        output_word(out,
                    (unsigned short int) (STAGE1_END - L_copy_len -
                                          adjust));

    }

    /* set the pos behind everything */
    output_set_pos(out, stages_end);
}
struct sfx_decruncher sfx_c16plus4[1] = { {&load, &stages, "c16/+4"} };
