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
 *   3. This notice may not be removed or altered from any source distribution.
 *
 *   4. The names of this software and/or it's copyright holders may not be
 *   used to endorse or promote products derived from this software without
 *   specific prior written permission.
 *
 * main.c, a part of the exomizer v1.0beta2 release
 *
 */

#include "search.h"
#include "optimal.h"
#include "output.h"
#include "sfx.h"
#include "getflag.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

void do_load(int filec, char *filev[], 
             unsigned char *buf, int *load, int *len)
{
     int min_load = -1;
     int max_end = -1;
     int i;
     FILE *in;

     for(i = 0; i < filec; ++i)
     {
          int tmp_load, tmp_end;
          in = fopen(filev[i], "rb");
          if(in == NULL) {
               fprintf(stderr,
                       " can't open file \"%s\" for input\n",
                       filev[i]);
               exit(1);
          }
          tmp_load = fgetc(in);
          tmp_load |= fgetc(in) << 8;
          
          tmp_end = tmp_load + fread(buf + tmp_load, 1, 65536 - tmp_load, in);

          fclose(in);

          printf(" filename: \"%s\", loading from $%04X to $%02X\n",
                 filev[i], tmp_load, tmp_end);

          if(min_load == -1 ||
             tmp_load < min_load)
          {
               min_load = tmp_load;
          }
          if(max_end == -1 ||
               tmp_end > max_end)
          {
               max_end = tmp_end;
          }
     }
     /* move memory to beginning of buffer */
     printf(" total area covering $%04X to $%02X (~%d '.')\n",
            min_load, max_end, (max_end - min_load) >> 8);

     *load = min_load;
     *len = max_end - min_load;
}

float simple1(int arg, void *priv)
{
     return 9.0;
}

float simpleN(int arg, void *priv)
{
     return (float)arg;
}
float simple0(int arg, void *priv)
{
     return 0.0f;
}

float elias_gamma(int arg, void *priv)
{
     int bits;
     bits = 0;
     if(arg == 0)
     {
	  return 999999999.0f;
     }

     do {
	  ++bits;
     }
     while(arg > ((1 << bits) - 1));

     bits = (bits << 1) - 1;
     return (float)bits;
}

float elias_gamma_sign(int arg, void *priv)
{
     int bits;
     bits = 0;
     if(arg < 1)
     {
	  arg = -arg + 1;
     }
     do {
	  ++bits;
     }
     while(arg > ((1 << bits) - 1));

     ++bits;

     bits = (bits << 1) - 1;
     return (float)bits;
}

float elias_delta(int arg, void *priv)
{
     int bits;
     bits = 0;
     if(arg == 0)
     {
	  return 999999999.0f;
     }

     do {
	  ++bits;
     }
     while(arg > ((1 << bits) - 1));

     bits = (int)elias_gamma(bits, NULL) + bits;
     return (float)bits;
}

float golomb(int arg, void *priv)
{
     int n;

     n = (int)priv;
     return (float)(((arg - 1)>> n) + 1 + n);
}

float hybrid(int arg, void *priv)
{
     int n;

     n = (int)priv;
     return (float)(n + elias_gamma((arg >> n) + 1, NULL));
}

float
rle_encode(encode_match_data emd, /* IN */
	   matchp_enum_get_next_f *get_next_f, /* IN */
	   void *matchp_enum) /* IN */

{
     encode_match_privp data;

     const_matchp mp;
     float sum;

     data = emd->priv;
     sum = 0.0;

     while((mp = get_next_f(matchp_enum)) != NULL)
     {
	  float a;
	  a = -data->len_f(mp->len, data->len_f_priv, NULL);
	  a += mp->len;

	  sum += a;
     }
     return sum;
}

float prefix_encode(const_matchp mp, encode_match_data emd)
{
     float bits;

     encode_match_privp data;
     
     data = emd->priv;

     bits = 1.0;
     if(mp->offset > 0)
     {
	  bits += data->offset_f(mp->offset, data->offset_f_priv, emd->out);
	  bits += data->len_f(mp->len, data->len_f_priv, emd->out);
	  /*if(mp->len > 256)
	  {
	       bits += 1000000.0;
	  }*/
	  /*if(mp->offset > 4096)
	  {
   	       bits += 1000000.0;
	       }*/
	  if(mp->offset == 1)
	  {
	       data->rle_num += 1;
	       data->rle_bits += bits;
	  } else
	  {
	       data->seq_num += 1;
	       data->seq_bits += bits;
	  }
     } else
     {
	  bits += 8.0;
	  data->lit_num += 1;
	  data->lit_bits += bits;
     }
     return bits;
}

void dump_encode(encode_int_f *f, void *priv)
{ 
     int i;
     for(i = 0; i < 32; ++i) {
	  printf("%0.1f, ", f(i, priv, NULL));
     }
     printf ("\n");    
}

void stats_clear(encode_match_priv m)
{
     m->lit_num = m->seq_num = m->rle_num = 0;
     m->lit_bits = m->seq_bits = m->rle_bits = 0.0;
}

void stats_dump(encode_match_priv m)
{
     printf("lit_num %d, lit_bits %0.1f, seq_num %d, "
	    "seq_bits %0.1f, rle_num %d, rle_bits %0.1f\n",
	    m->lit_num, m->lit_bits, m->seq_num,
	    m->seq_bits, m->rle_num, m->rle_bits);
}

int
generate_output(match_ctx ctx,
		search_nodep snp,
		encode_match_f *f,
		encode_match_data emd,
		int load,
                int len,
                int start,
		FILE *of)
{
     int pos;
     output_ctx out;
     output_ctxp old;

     output_ctx_init(out);

     old = emd->out;
     emd->out = out;

     if(start >= 0 && start < 0x10000)
     {
          /* -s given */
          sfx1_set_new_load(out, (unsigned short int)load);
     } else if(start >= 0x10000 && start < 0x20000)
     {
          /* -l given */
          output_word(out, (unsigned short int)(start - 0x10000));
     } else if(start == -1)
     {
          /* -r given */
          output_ctx_set_reverse(out);
     }

     pos = output_get_pos(out);

     /*printf("pos $%04X\n", out->pos);*/
     output_gamma_code(out, 17);

     /*printf("pos $%04X\n", out->pos);*/
     /*printf("------------\n");*/
     while(snp != NULL)
     {
	  const_matchp mp;

	  mp = snp->match;
	  if(mp != NULL && mp->len > 0)
	  {
	       if(mp->offset == 0)
	       {
		    /* literal */
		    output_byte(out, ctx->buf[snp->index]);
		    output_bits(out, 1, 1);
	       } else
	       {
		    f(mp, emd);
		    output_bits(out, 1, 0);
	       }
	  }
	  /*printf("------------\n");*/
	  snp = snp->prev;
     }
     
     /*printf("pos $%04X\n", out->pos);*/
     /* output header here */
     optimal_out(out, emd);
     /*printf("pos $%04X\n", out->pos);*/

     output_bits_flush(out);

     output_word(out, (unsigned short int)(load + len));
     /*printf("pos $%04X\n", out->pos);*/

     printf("  length of compressed data (including header): %d\n",
            output_get_pos(out) - pos); 

     if(start >= 0)
     {
          sfx2_add_stages(out, (unsigned short int)start);
     }

     len = output_ctx_close(out, of);
     emd->out = old;
	  
     return len;
}

search_nodep
do_compress(match_ctx ctx,
            encode_match_data emd)
{
     matchp_cache_enum mpce;
     matchp_snp_enum snpe;
     search_nodep snp;
     search_nodep best_snp;
     int pass;
     float size;
     float old_size;

     pass = 1;
     printf(" pass %d: optimizing ...\n", pass);
     matchp_cache_get_enum(ctx, mpce);
     optimal_optimize(emd, matchp_cache_enum_get_next, mpce);

     best_snp = NULL;
     old_size = 1000000.0;

     for(;;)
     {
	  snp = search_buffer(ctx, 
			      optimal_encode,
			      emd);
	  if(snp == NULL)
	  {
	       fprintf(stderr, "error: search_buffer() returned NULL\n");
	       exit(-1);
	  }
	  
	  size = snp->total_score;
	  printf("  size %0.1f bits ~%d bytes\n",
		 size, (((int)size) + 7) >> 3);
	  
	  if(size >= old_size)
	  {
	       search_node_free(snp);
	       break;
	  }
	  ++pass;

	  if(best_snp != NULL)
	  {
	       search_node_free(best_snp);
	  }
	  best_snp = snp;
	  old_size = size;

          optimal_free(emd);
          optimal_init(emd);

	  printf(" pass %d: optimizing ... \n", pass);
	  matchp_snp_get_enum(snp, snpe);
	  optimal_optimize(emd, matchp_snp_enum_get_next, snpe);
     }
#if 0
     optimal_dump(emd);
#endif
     return best_snp;
 }

void
do_output(match_ctx ctx,
          search_nodep snp,
          encode_match_data emd,
          const char *outname,
          int load,
          int len,
          int start)
{
     FILE *outfile;

     outfile = fopen(outname, "wb");
     if(outfile == NULL) {
          fprintf(stderr, " can't open file \"%s\" for output\n", outname);
          exit(1);
     }
     
     len = generate_output(ctx, snp, optimal_encode, emd,
			   load, len, start, outfile);
     fclose(outfile);

     printf("  filename: \"%s\", total length: %d\n",
	    outname, len);
     
#ifdef LITERAL_TAGGING_RLE
     {
	  encode_match_data emd2;
	  matchp_snp_rle_enum rlee;
	  encode_match_priv optimal_priv2;

	  printf("\nPass 4: Approximate rle-based literal tagging scheme"
		 "\n----------------------------------------------------\n");
	  emd2->priv = optimal_priv2;
	  optimal_init(emd2);
	  
	  matchp_snp_rle_get_enum(best_snp, rlee);
	  optimal_optimize(emd2, matchp_snp_rle_enum_get_next, rlee);
	  
	  size = rle_encode(emd2, matchp_snp_rle_enum_get_next, rlee);
	  
	  printf("  Not yet implemented. Printing an estimate:\n");
       
	  printf("  rle_encode saves %0.1f bits ~%d bytes "
		 "(ignoring header size increase)\n",
		 size, (((int)size) + 7) >> 3);
#if 0     
	  optimal_dump(emd2);
#endif
	  printf(" Approximating tagging scheme, NOT done.\n");
	  
	  optimal_free(emd2);
     }
#endif
     /*printf(" Output file sucessfully generated.\n");*/
}

void
print_license()
{
     printf("This is Exomizer v1.0beta2, Copyright (c) 2002 Magnus Lind.\n"
	    "----------------------------------------------------------------------------\n"
	    "This software is provided 'as-is', without any express or implied warranty.\n"
	    "In no event will the authors be held liable for any damages arising from\n"
	    "the use of this software.\n\n"
	    "Permission is granted to anyone to use this software, alter it and re-\n"
	    "distribute it freely for any non-commercial, non-profit purpose subject to\n"
	    "the following restrictions:\n\n"
	    "   1.  The origin of this software must not be misrepresented; you must not\n"
	    "   claim that you wrote the original software. If you use this software in a\n"
	    "   product, an acknowledgment in the product documentation would be\n"
	    "   appreciated but is not required.\n"
	    "   2. This notice may not be removed or altered from any distribution.\n"
	    "   3. The names of this software and/or it's copyright holders may not be\n"
	    "   used to endorse or promote products derived from this software without\n"
	    "   specific prior written permission.\n\n"
	    "However, files produced by this software are not covered by this license and\n"
	    "may be used for any purpose, including commercial applications.\n"
	    "----------------------------------------------------------------------------\n");
}


void
print_usage(const char *appl)
{
     const char *applp;

     /* strip pathprefix from appl */
     applp = strrchr(appl, '\\');
     if(applp != NULL)
     {
          appl = applp + 1;
     } /* done */
     applp = strrchr(appl, '/');
     if(applp != NULL)
     {
          appl = applp + 1;
     } /* done */

     printf("usage: %s [options] infile(s)\n", appl);
     printf(
 "  -r           writes the outfile backwards without load address, this is\n"
 "               suitable for files that are to be decompressed directly\n"
 "               from disc, can't be combined with -l or -s\n"
 "  -l <address> adds load address to the outfile, can't be combined\n"
 "               with -r or -s, default is no load address\n"
 "  -s <address> adds load adress $0801, basic-line and decruncher, after\n"
 "               decrunching it will jmp to <address>, can't be combined\n"
 "               with -r or -l\n"
 "  -o <outname> sets the outfile name, default is \"a.out\"\n"
 "  --           treat all args to the right as non-options\n"
 "  -?           displays this help screen\n"
 "  -u           displays copyright and the usage license\n"
 " All infiles are merged into the outfile. They are loaded in the order\n"
 " they are given on the command-line, from the left to the right.\n");
}

int 
main(int argc, char *argv[])
{
     const char *outfile = NULL;
     int reverse = 0;
     int outstart = -1;
     int outload = -1;
     int c, infilec;
     char **infilev;
     int load;
     int len;
     unsigned char mem[65536];
     match_ctx ctx;
     encode_match_data emd;
     encode_match_priv optimal_priv;
     search_nodep snp;

     /*printf("flagind %d\n", flagind);*/
     outload = -1;
     while((c = getflag(argc, argv, "rl:s:o:u")) != -1)
     {
          /*printf(" flagind %d flagopt '%c'\n", flagind, c);*/
          switch (c) {
          case 'r':
               reverse = 1;
               break;
          case 'l':
               if(sscanf(flagarg, "%i", &outload) != 1 ||
                  outload < 0 || outload >= 65536)
               {
                    fprintf(stderr,
                            "error: invalid adress for -l option, "
                            "must be in the range of [0 - 65535]\n");
                    print_usage(argv[0]);
                    exit(1);
               }
               break;
          case 's':
               if(sscanf(flagarg, "%i", &outstart) != 1 ||
                  outstart < 0 || outstart >= 65536)
               {
                    fprintf(stderr,
                            "error: invalid adress for -s option, "
                           "must be in the range of [0 - 65535]\n");
                    print_usage(argv[0]);
                    exit(1);
               }
               break;
          case 'o':
               outfile = flagarg;
               break;
          case 'u':
	       print_license();
               exit(1);               
          default:
               if(c != '?')
               {
                    fprintf(stderr,
                            "error, invalid option \"-%c\"\n", flagflag);
               } else
               print_usage(argv[0]);
               exit(1);               
          }
     }
#if 0
     printf("flagind %d\n", flagind);
     for(c = 0; c < argc; ++c)
     {
          if(c == flagind)
          {
               printf("-----------------------\n");
          }
          printf("argv[%d] = \"%s\"\n", c, argv[c]);
     }
     exit(1);
#endif

     infilev = argv + flagind;
     infilec = argc - flagind;

     if((reverse != 0) + (outload != -1) + (outstart != -1) > 1)
     {
          fprintf(stderr,
                  "error: the -r, -l or -s options can't be combined.\n");
          print_usage(argv[0]);
          exit(1);
     }
     if(infilec == 0)
     {
          fprintf(stderr,
                  "error: no input files to process.\n");
          print_usage(argv[0]);
          exit(1);
     }

     if(outfile == NULL)
     {
          printf("warning: no -o option was given, outfile is \"a.out\".\n");
          outfile = "a.out";
     }

     printf("Mode for target file: ");
     if(reverse)
     {
          printf("decompression from file\n");
          outstart = -1;
     } else if(outstart < 0)
     {
          printf("decompression from memory");
          if(outload != -1)
          {
               printf(", loading to: $%04X", outload);
          }
          printf("\n");
          outstart = outload + 0x10000;
     } else
     {
          printf("self-decompressing executable, start address $%04X\n", outstart);
     }

     printf("\nPhase 1: Instrumenting input files"
	    "\n----------------------------------\n");

     /* zero fill mem */
     memset(mem, 0, sizeof(mem));
     do_load(infilec, infilev, mem, &load, &len);
     match_ctx_init(ctx, mem + load, len);

     printf(" Instrumenting file, done.\n");

     emd->out = NULL;
     emd->priv = optimal_priv;

     optimal_init(emd);
     
     printf("\nPhase 2: Calculating encoding"
	    "\n-----------------------------\n");
     snp = do_compress(ctx, emd);
     printf(" Calulating encoding, done.\n");
     
     printf("\nPass 3: Generating output file"
	    "\n------------------------------\n");
     do_output(ctx, snp, emd, outfile, load, len, outstart);

     printf(" Generating file, done.\n");

     optimal_free(emd);

     search_node_free(snp);
     match_ctx_free(ctx);

     return 0;
}
