/*
 * Copyright (c) 2002, 2003 Magnus Lind.
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
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "log.h"
#include "search.h"
#include "optimal.h"
#include "output.h"
#include "getflag.h"
#include "membuf_io.h"
#include "sfx_config.h"
#include "named_buffer.h"

extern struct membuf prgdecr[];

static
FILE *
open_file(char *name, int *load_addr)
{
    FILE * in;
    int relocated = 0;
    int reloc;
    int load;

    do {
        char *load_str;

        in = fopen(name, "rb");
        if (in != NULL)
        {
            /* we have succeded in opening the file */
            break;
        }

        /* hmm, let's see if the user is trying to relocate it */
        load_str = strrchr(name, ',');
        if (load_str == NULL)
        {
            /* we fail */
            break;
        }

        *load_str = '\0';
        ++load_str;
        relocated = 1;

        /* relocation was requested */
        if (str_to_int(load_str, &reloc) != 0)
        {
            /* we fail */
            LOG(LOG_FATAL,
                (" can't parse load address from \"%s\"\n", load_str));
            exit(-1);
        }

        in = fopen(name, "rb");

    } while (0);
    if (in == NULL)
    {
        LOG(LOG_FATAL,
            (" can't open file \"%s\" for input\n", name));
        exit(-1);
    }

    /* set the load address */
    load = fgetc(in);
    load |= fgetc(in) << 8;

    if(load_addr != NULL)
    {
        *load_addr = load;
        if (relocated)
        {
            *load_addr = reloc;
        }
    }
    return in;
}

static int find_sys(const unsigned char *buf)
{
    int outstart = -1;
    int state = 1;
    int i = 0;
    /* skip línk and line number */
    buf += 4;
    /* exit loop at line end */
    while(i < 1000 && buf[i] != '\0')
    {
        unsigned char *sys_end;
        int c = buf[i];

        switch(state)
        {
            /* look for and consume sys token */
        case 1:
            if(c == 0x9e) state = 2;
            break;
            /* skip spaces and left parenthesis, if any */
        case 2:
            if(strchr(" (", c) != NULL) break;
            state = 3;
            /* convert string number to int */
        case 3:
            outstart = strtol((char*)(buf + i), (void*)&sys_end, 10);
            if((buf + i) == sys_end)
            {
                /* we got nothing */
                outstart = -1;
            }
            state = 4;
            break;
        case 4:
            break;
        }
        ++i;
    }

    LOG(LOG_DEBUG, ("state when leaving: %d.\n", state));
    return outstart;
}

static void load_prg(char *filename, char mem[65536],
                     int *startp, int *endp)
{
    int load;
    int len;
    FILE *in;
    in = open_file(filename, &load);
    len = fread(mem + load, 1, 65536 - load, in);
    fclose(in);

    LOG(LOG_NORMAL,
        (" filename: \"%s\", loading from $%04X to $%04X\n",
         filename, load, load + len));

    if(startp != NULL) *startp = load;
    if(endp != NULL) *endp = load + len;
}

static
int
do_loads(int filec, char *filev[], struct membuf *mem,
         const struct sfx_config *sfx, int *basic_end)
{
    int min_start = 65537;
    int max_end = -1;
    int i;
    char *p;

    membuf_clear(mem);
    membuf_append(mem, NULL, 65536);

    for (i = 0; i < filec; ++i)
    {
        FILE *in;
        int start, end;

        load_prg(filev[i], membuf_get(mem), &start, &end);

        /* if any file loads to the basic start, then communicate its end */
        if(sfx != NULL && basic_end != NULL &&
           start <= sfx->basic_start && end > sfx->basic_start)
        {
            *basic_end = end;
        }

        if (start < min_start)
        {
            min_start = start;
        }
        if (end > max_end)
        {
            max_end = end;
        }
    }
    /* move memory to beginning of buffer */
    membuf_truncate(mem, max_end);
    membuf_flip(mem, min_start);

    LOG(LOG_NORMAL, (" crunching from $%04X to $%04X ", min_start, max_end));
    return min_start;
}

static
void print_license()
{
    LOG(LOG_BRIEF,
        ("----------------------------------------------------------------------------\n"
         "Exomizer v2.0 Copyright (c) 2005 Magnus Lind. (magli143@comhem.se)\n"
         "----------------------------------------------------------------------------\n"));
    LOG(LOG_BRIEF,
        ("This software is provided 'as-is', without any express or implied warranty.\n"
         "In no event will the authors be held liable for any damages arising from\n"
         "the use of this software.\n"
         "Permission is granted to anyone to use this software, alter it and re-\n"
         "distribute it freely for any non-commercial, non-profit purpose subject to\n"
         "the following restrictions:\n\n"));
    LOG(LOG_BRIEF,
        ("   1.  The origin of this software must not be misrepresented; you must not\n"
         "   claim that you wrote the original software. If you use this software in a\n"
         "   product, an acknowledgment in the product documentation would be\n"
         "   appreciated but is not required.\n"
         "   2. Altered source versions must be plainly marked as such, and must not\n"
         "   be misrepresented as being the original software.\n"
         "   3. This notice may not be removed or altered from any distribution.\n"));
    LOG(LOG_BRIEF,
        ("   4. The names of this software and/or it's copyright holders may not be\n"
         "   used to endorse or promote products derived from this software without\n"
         "   specific prior written permission.\n"
         "----------------------------------------------------------------------------\n"
         "The files processed and/or generated by using this software are not covered\n"
         "nor infected by this license in any way.\n"));
}

static
void print_usage(const char *appl, enum log_level level)
{
    const char *applp;

    /* strip pathprefix from appl */
    applp = strrchr(appl, '\\');
    if (applp != NULL)
    {
        appl = applp + 1;
    }                           /* done */
    applp = strrchr(appl, '/');
    if (applp != NULL)
    {
        appl = applp + 1;
    }
    /* done */
    LOG(level, ("usage: %s [option]... infile[,<address>]...\n", appl));
    LOG(level,
        ("  -r           writes the outfile backwards without load address, this is\n"
         "               suitable for files that are to be decompressed directly\n"
         "               from disk, can't be combined with -l or -s\n"
         "  -l <address> adds load address to the outfile, using \"auto\" as <address>\n"
         "               will enable load address auto detection, can't be combined\n"
         "               with -r or -s, default is no load address\n"
         "  -s <address> adds basic-line and decruncher that exits with a jmp <address>,\n"));
    LOG(level,
        ("               using \"sys\" as <address> will enable basic-sys address auto\n"
         "               detection, using \"basic\" as <address> will run basic.can't be combined with -r or -l\n"
         "  -o <outname> sets the outfile name, default is \"a.prg\"\n"
         "  -q           enable quiet mode, display output is reduced to one line\n"
         "  -t           the decruncher target, must be 20, 23, 52, 55, 4, 64 or 128,\n"
         "               must be combined with -s\n"));
    LOG(level,
        ("  -m <offset>  limits the maximum offset size\n"
         "  -p <passes>  limits the maximum number of optimization passes\n"
         "  -4           alias for -t4 (deprecated)\n"
         "  -n           does nothing (deprecated)\n"
         "               combined with -s\n"
         "  --           treat all args to the right as non-options\n"
         "  -?           displays this help screen\n"
         "  -v           displays version and the usage license\n"
         " All infiles are merged into the outfile. They are loaded in the order\n"
         " they are given on the command-line, from left to right.\n"));
}

int
main(int argc, char *argv[])
{
    const char *outfile = "a.prg";
    const char *exported_encoding = NULL;
    int reverse = 0;
    int outstart = -1;
    int decruncher = 0;
    int decr_target = 0;
    int decr_effect_off = 0;
    int symbol_arg = 0;
    int basic_end = -1;
    int loadaddr = 0;
    int outload = -1;
    int c, infilec;
    char *p;
    char **infilev;
    int start;
    int safety;
    int len;
    int max_offset = 65536;
    int max_passes = 65536;
    const struct sfx_config *sfx;

    struct membuf buf1[1];
    struct membuf *in;
    struct membuf *out;

    /* init logging */
    LOG_INIT_CONSOLE(LOG_NORMAL);

    sfx = get_sfx_config(64);
    parse_init();

    LOG(LOG_DUMP, ("flagind %d\n", flagind));

    while ((c = getflag(argc, argv, "m:t:q4D:rnl:s:o:vp:e:")) != -1)
    {
        LOG(LOG_DUMP, (" flagind %d flagopt '%c'\n", flagind, c));
        switch (c)
        {
        case 'q':
            LOG_SET_LEVEL(LOG_BRIEF);
            break;
        case 'r':
            reverse = 1;
            break;
        case 'm':
            if (str_to_int(flagarg, &max_offset) != 0 ||
                max_offset < 0 || max_offset >= 65536)
            {
                LOG(LOG_ERROR,
                    ("Error: invalid offset for -m option, "
                     "must be in the range of [0 - 0xffff]\n"));
                print_usage(argv[0], LOG_NORMAL);
                exit(-1);
            }
            break;
        case 'l':
            loadaddr = 1;
            if (strcmp(flagarg, "auto") != 0 && strcmp(flagarg, "AUTO") != 0 &&
                (str_to_int(flagarg, &outload) != 0 ||
                 outload < 0 || outload >= 65536))
            {
                LOG(LOG_ERROR,
                    ("Error: invalid address for -l option, "
                     "must be in the range of [0 - 0xffff]\n"));
                print_usage(argv[0], LOG_NORMAL);
                exit(-1);
            }
            break;
        case 'p':
            if (str_to_int(flagarg, &max_passes) != 0 ||
                max_passes < 1 || max_passes >= 65536)
            {
                LOG(LOG_ERROR,
                    ("Error: invalid value for -p option, "
                     "must be in the range of [1 - 0xffff]\n"));
                print_usage(argv[0], LOG_NORMAL);
                exit(-1);
            }
            break;
        case 's':
            decruncher = 1;
            if (strcmp(flagarg, "sys") != 0 && strcmp(flagarg, "SYS") != 0 &&
                (str_to_int(flagarg, &outstart) != 0 ||
                 outstart < 0 || outstart >= 65536))
            {
                LOG(LOG_ERROR,
                    ("Error: invalid address for -s option, "
                     "must be in the range of [0 - 0xffff]\n"));
                print_usage(argv[0], LOG_NORMAL);
                exit(-1);
            }
            break;
        case 'o':
            outfile = flagarg;
            break;
        case 'n':
            decr_effect_off = 1;
            break;
        case 'v':
            print_license();
            exit(0);
        case '4':
            decr_target = 4;
            sfx = get_sfx_config(decr_target);
            break;
        case 't':
            if (str_to_int(flagarg, &decr_target) != 0 ||
                (sfx = get_sfx_config(decr_target)) == NULL)
            {
                LOG(LOG_ERROR,
                    ("error: invalid value for -t option, "
                     "must be one of "));
                sfx_info_dump(LOG_ERROR);
                print_usage(argv[0], LOG_NORMAL);
                exit(-1);
            }
            break;
        case 'e':
            exported_encoding = flagarg;
            break;
        case 'D':
            p = strrchr(flagarg, '=');
            if(p != NULL)
            {
                int value;
                if(str_to_int(p + 1, &value) != 0)
                {
                    LOG(LOG_ERROR, ("Error: invalid value for -D "
                                    "<symbol>[=<value>] option.\n"));
                    print_usage(argv[0], LOG_NORMAL);
                    exit(-1);
                }
                /* This is ugly, we really should allocate our own
                 * copy of the symbol string. */
                *p = '\0';
                new_symbol(flagarg, value);
                symbol_arg = 1;
            }
            else
            {
                LOG(LOG_ERROR, ("Error: invalid value for -D "
                                "<symbol>=<value> option.\n"));
                print_usage(argv[0], LOG_NORMAL);
                exit(-1);
            }
            break;
        default:
            if (flagflag != '?')
            {
                LOG(LOG_ERROR,
                    ("Error: invalid option \"-%c\"", flagflag));
                if (flagarg != NULL)
                {
                    LOG(LOG_ERROR, (" with argument \"%s\"", flagarg));
                }
                LOG(LOG_ERROR, ("\n"));
            }
            print_usage(argv[0], LOG_BRIEF);
            exit(0);
        }
    }

    /* assert option combinations */
    if (reverse + loadaddr + decruncher > 1)
    {
        LOG(LOG_ERROR,
            ("Error: the -r, -l or -s options can't be combined.\n"));
        print_usage(argv[0], LOG_NORMAL);
        exit(-1);
    }
    if (symbol_arg != 0 && !decruncher)
    {
        LOG(LOG_ERROR,
            ("Error: the -D<symbol>=<value> option must be combined "
             "with -s.\n"));
        print_usage(argv[0], LOG_NORMAL);
        exit(-1);
    }
    if (decr_target != 0 && !decruncher)
    {
        LOG(LOG_ERROR,
            ("Error: the -t<target> option must be combined with -s.\n"));
        print_usage(argv[0], LOG_NORMAL);
        exit(-1);
    }
    if (decr_effect_off && !decruncher)
    {
        LOG(LOG_ERROR,
            ("Error: the -n option must be combined with -s.\n"));
        print_usage(argv[0], LOG_NORMAL);
        exit(-1);
    }
    /* end of option combination assertions */

#if 0
    LOG(LOG_DEBUG, ("flagind %d\n", flagind));
    for (c = 0; c < argc; ++c)
    {
        if (c == flagind)
        {
            LOG(LOG_DEBUG, ("-----------------------\n"));
        }
        LOG(LOG_DEBUG, ("argv[%d] = \"%s\"\n", c, argv[c]));
    }
    exit(1);
#endif

    membuf_init(buf1);
    in = buf1;
    out = new_named_buffer("crunched_data");

    infilev = argv + flagind;
    infilec = argc - flagind;

    if (infilec == 0)
    {
        LOG(LOG_ERROR, ("Error: no input files to process.\n"));
        print_usage(argv[0], LOG_NORMAL);
        exit(-1);
    }

    start = do_loads(infilec, infilev, in, sfx, &basic_end);

    if(decruncher)
    {
        LOG(LOG_NORMAL, ("self-decrunching %s executable", sfx->model));
        if(outstart < 0)
        {
            if(basic_end < 0)
            {
                /* no program loaded to the basic start */
                LOG(LOG_ERROR, ("\nError: nothing loaded at the basic "
                                "start address ($%04X).\n",
                                sfx->basic_start));
                exit(-1);
            }
            outstart = find_sys((char*) membuf_get(in) +
                                sfx->basic_start - start);
            if(outstart < 0)
            {
                LOG(LOG_ERROR, ("\nError: cant find sys address.\n"));
                exit(-1);
            }
        }
        LOG(LOG_NORMAL, (", jmp address $%04X\n", outstart));
    }

    if(loadaddr || decruncher)
    {
        membuf_append(out, NULL, 2);
    }

    safety = crunch_backwards(in, out, exported_encoding,
                              max_passes, max_offset);

    LOG(LOG_NORMAL, ("safety is %d\n", safety));

    p = membuf_get(out);
    if(loadaddr || decruncher)
    {
        if(outload < 0)
        {
            /* now we know the value of the safety */
            outload = start - safety;
        }
        p[0] = outload & 255;
        p[1] = outload >> 8;
    }
    start += membuf_memlen(in);
    membuf_append_char(out, start & 255);
    membuf_append_char(out, start >> 8);

    if(reverse)
    {
        reverse_buffer(p, membuf_memlen(out));
    }
    if(decruncher)
    {
        /* add decruncher */
        struct membuf source[1];

        membuf_init(source);

        decrunch(LOG_DEBUG, prgdecr, source);

        in = out;
        out = buf1;
        membuf_clear(out);

        new_symbol("i_start_addr", outstart);
        /*symbol_dump_resolved(LOG_NORMAL, "i_start_addr");*/
        new_symbol("i_target", decr_target);
        /*symbol_dump_resolved(LOG_NORMAL, "i_target");*/

        /* new_symbol("i_ram_on_exit", 1);
           symbol_dump_resolved(LOG_NORMAL, "i_ram_on_exit"); */

        /* new_symbol("i_config_effect", 1);
           symbol_dump_resolved(LOG_NORMAL, "i_config_effect"); */

        if(assemble(source, out) != 0)
        {
            LOG(LOG_ERROR, ("Parse failure.\n"));
        }

        membuf_free(source);
    }

    write_file(outfile, out);

    membuf_free(buf1);

    parse_free();

    LOG_FREE;

    return 0;
}
