/*
 * sfx.c
 *
 */
#include "output.h"
#include <stdlib.h>

#define DECOMP_SAFETY 48
#define DECOMP_LEN 0xbb

static const unsigned char stage1[] = {
     0x01, 0x08, 0x0B, 0x08, 0xD2, 0x07, 0x9E, 0x32, 
     0x30, 0x35, 0x39, 0x00, 0xA0, 0x00, 0x78, 0xE6, 
     0x01, 0xA2, DECOMP_LEN + 3, 0xBD, 0x00, 0x00, 0x9D, 0xFC, 
     0x00, 0xCA, 0xD0, 0xF7, 0x4C, 0x00, 0x00
};
#define STAGE1_COPY_SRC 20
#define STAGE1_JMP_STAGE2 29

#define STAGE1_BEGIN 0x07ff
#define STAGE1_END   0x081e

static unsigned char stage2[] = {
     0xE8, 0xA9, 0x00, 0x85, 0xFA, 0x85, 0xFB, 0xE0, 
     0x01, 0x90, 0x28, 0xA5, 0xFD, 0x4A, 0xD0, 0x18, 
     0xAD, 0x23, 0x01, 0xD0, 0x0A, 0xC6, 0x01, 0xEE, 
     0xE7, 0xDB, 0xE6, 0x01, 0xCE, 0x24, 0x01, 0xCE, 
     0x23, 0x01, 0xAD, 0x1B, 0x08, 0x90, 0x1B, 0x6A, 
     0x26, 0xFA, 0x26, 0xFB, 0xCA, 0xD0, 0xDE, 0x85, 
     0xFD, 0xA5, 0xFA, 0x60, 0xC6, 0x01, 0x58, 0x4C, 
     0x00, 0xC6, 0xCA, 0xC6, 0xFF, 0xC6, 0xAF, 0x88, 
     0xB1, 0xAE, 0x91, 0xFE, 0x98, 0xD0, 0xF8, 0x8A, 
     0xD0, 0xF0, 0x20, 0x00, 0x01, 0xF0, 0x0A, 0xA5, 
     0xFE, 0xD0, 0x02, 0xC6, 0xFF, 0xC6, 0xFE, 0x90, 
     0xB7, 0xC8, 0x20, 0x00, 0x01, 0xF0, 0xFA, 0xC0, 
     0x11, 0xB0, 0xD1, 0xBE, 0x33, 0x03, 0x20, 0x01, 
     0x01, 0x79, 0x67, 0x03, 0x85, 0xFC, 0xAA, 0xA5, 
     0xFB, 0x79, 0x9B, 0x03, 0x48, 0xD0, 0x04, 0xE0, 
     0x04, 0x90, 0x02, 0xA2, 0x03, 0x8A, 0xA8, 0xBE, 
     0xB4, 0x01, 0x20, 0x01, 0x01, 0x79, 0xB7, 0x01, 
     0xA8, 0x38, 0xA5, 0xFE, 0xE5, 0xFC, 0x85, 0xFE, 
     0xB0, 0x02, 0xC6, 0xFF, 0xBE, 0x34, 0x03, 0x20, 
     0x01, 0x01, 0x79, 0x68, 0x03, 0x90, 0x03, 0xE6, 
     0xFB, 0x18, 0x65, 0xFE, 0x85, 0xAE, 0xA5, 0xFB, 
     0x79, 0x9C, 0x03, 0x65, 0xFF, 0x85, 0xAF, 0xA4, 
     0xFC, 0x68, 0xAA, 0x90, 0x8F, 0x02, 0x04, 0x04, 
     0x30, 0x20, 0x10, 0xE8, 0x98, 0x29, 0x0F, 0xF0, 
     0x0B, 0xA5, 0xFA, 0x79, 0x67, 0x03, 0xAA, 0xA5, 
     0xFB, 0x79, 0x9B, 0x03, 0x99, 0x9C, 0x03, 0x8A, 
     0x99, 0x68, 0x03, 0xA2, 0x04, 0x20, 0x01, 0x01, 
     0x99, 0x34, 0x03, 0x86, 0xFA, 0xAA, 0xE8, 0x38, 
     0x26, 0xFA, 0x26, 0xFB, 0xCA, 0xD0, 0xF9, 0xC8, 
     0xC0, 0x34, 0xD0, 0xCF, 0xA0, 0x00, 0x4C, 0x4A,
     0x01
};
#define STAGE2_GET_BYTE     35
#define STAGE2_START        56
#define STAGE2_SHORTCUT    238
#define STAGE2_COPY_LEN_LO 237

static unsigned char stage3[] = {
     0xA2, 0x00, 0xB0, 0x0E, 0xCA, 0xCE, 0x1A, 0x09, 
     0xCE, 0x1D, 0x09, 0x88, 0xB9, 0x00, 0x00, 0x99, 
     0x00, 0x00, 0x98, 0xD0, 0xF6, 0x8A, 0xD0, 0xEC, 
     0x4C, 0x4A, 0x01
};
#define STAGE3_COPY_LEN_HI  1
#define STAGE3_COPY_DEC_SRC_HI 6
#define STAGE3_COPY_DEC_DEST_HI  9
#define STAGE3_COPY_SRC    13
#define STAGE3_COPY_DEST   16

static unsigned short int L_copy_len;

void
sfx1_set_new_load(output_ctx out, /* IN/OUT */
                  unsigned short int load) /* IN */
{
     unsigned short int new_load;
     if(load < 0x0400)
     {
          printf("error: cant handle load adresses < $0400\n");
          exit(1);
     }

     output_ctx_set_start(out, STAGE1_BEGIN);
     
     /* set L_load to just after stage1 code */
     new_load = STAGE1_END;     

     /* do we have enough decompression buffer safety? */
     if(load - DECOMP_SAFETY < STAGE1_END)
     {
          /* no, we need to transfer bytes */
          new_load = load - DECOMP_SAFETY;
     }
     output_set_pos(out, new_load);

     L_copy_len = STAGE1_END - new_load;
#if 0
     printf("copy_len $%04X\n", L_copy_len);
     printf("new_load $%04X\n", new_load);
#endif
}

void
sfx2_add_stages(output_ctx out, /* IN/OUT */
                unsigned short int start) /* IN */
{
     int i;
     int stage2_begin;
     int stage3_begin = 0;
     int stage3_end = 0;
     int stages_end;

     stage2_begin = output_get_pos(out);
     /*printf("stage2_begin $%04X\n", stage2_begin);*/

     for(i = 0; i < sizeof(stage2); ++i)
     {
          output_byte(out, stage2[i]);
     }
     if(L_copy_len > 0)
     {
          /* add stage 3 */

          /* clobber the jmp last in stage 2 */

          stage3_begin = output_get_pos(out) - 3;
          /*printf("stage3_begin $%04X\n", stage3_begin);*/
          output_set_pos(out, stage3_begin);
          
          /* copy stage 3 */
          for(i = 0; i < sizeof(stage3); ++i)
          {
               output_byte(out, stage3[i]);
          }

          stage3_end = output_get_pos(out);

          /* copy the actual bytes */
          output_copy_bytes(out, STAGE1_END - L_copy_len, L_copy_len);
     }
     stages_end = output_get_pos(out);
     /*printf("stages_end $%04X\n", stages_end);*/

     /* add stage 1 */
     output_set_pos(out, STAGE1_BEGIN);
     for(i = 0; i < sizeof(stage1); ++i)
     {
          output_byte(out, stage1[i]);
     }
          
     /* fixup adresses */
     output_set_pos(out, STAGE1_BEGIN + STAGE1_COPY_SRC);
     output_word(out, (unsigned short int)(stage2_begin - 4));

     output_set_pos(out, STAGE1_BEGIN + STAGE1_JMP_STAGE2);
     output_word(out, (unsigned short int)(stage2_begin + DECOMP_LEN));

     output_set_pos(out, stage2_begin + STAGE2_GET_BYTE);
     output_word(out, (unsigned short int)(stage2_begin - 3));
     
     output_set_pos(out, stage2_begin + STAGE2_START);
     output_word(out, (unsigned short int)start);

     if(L_copy_len > 0)
     {
          /* fixup additional stage 3 stuff */
          output_set_pos(out, stage2_begin + STAGE2_COPY_LEN_LO);
          output_byte(out, (unsigned char)(L_copy_len & 0xff));
          
          output_set_pos(out, stage3_begin + STAGE3_COPY_LEN_HI);
          output_byte(out, (unsigned char)((L_copy_len >> 8) & 0xff));

          output_set_pos(out, stage3_begin + STAGE3_COPY_DEC_SRC_HI);
          output_word(out, (unsigned short int)(stage3_begin + STAGE3_COPY_SRC + 1));

          output_set_pos(out, stage3_begin + STAGE3_COPY_DEC_DEST_HI);
          output_word(out, (unsigned short int)(stage3_begin + STAGE3_COPY_DEST + 1));

          output_set_pos(out, stage3_begin + STAGE3_COPY_SRC);
          output_word(out, (unsigned short int)(stage3_end + (L_copy_len & 0xff00)));
          
          output_set_pos(out, stage3_begin + STAGE3_COPY_DEST);
          output_word(out, (unsigned short int)(STAGE1_END - (L_copy_len & 0x00ff)));

     }
     output_set_pos(out, stages_end);
}
