/*
 * Copyright (c) 2002 - 2005 Magnus Lind.
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
#include "exo_helper.h"
#include "parse.h"
#include "named_buffer.h"

extern struct membuf sfxdecr[];

#define DEFAULT_OUTFILE "a.out"

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

static int get_word(FILE *in)
{
    int word = fgetc(in);
    word |= fgetc(in) << 8;
    if(word < 0)
    {
        LOG(LOG_ERROR, ("Error: unexpected end of xex-file."));
        fclose(in);
        exit(-1);
    }
    return word;
}

static
FILE *
open_file(char *name, int *load_addr)
{
    FILE * in;
    int is_prg = 1;
    int is_relocated = 0;
    int load;

    do
    {
        char *load_str;
        char *at_str;

        in = fopen(name, "rb");
        if (in != NULL)
        {
            /* We have succeded in opening the file.
             * There's no address suffix. */
            break;
        }

        /* hmm, let's see if the user is trying to relocate it */
        load_str = strrchr(name, ',');
        at_str = strrchr(name, '@');
        if(at_str != NULL && (load_str == NULL || at_str > load_str))
        {
            is_prg = 0;
            load_str = at_str;
        }

        if (load_str == NULL)
        {
            /* nope, */
            break;
        }

        *load_str = '\0';
        ++load_str;
        is_relocated = 1;

        /* relocation was requested */
        if (str_to_int(load_str, &load) != 0)
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

    if(is_prg)
    {
        /* read the prg load address */
        int prg_load = get_word(in);
        if(!is_relocated)
        {
            load = prg_load;
            /* unrelocated prg loading to $ffff is xex */
            if(prg_load == 0xffff)
            {
                /* differentiate this from relocated $ffff files so it is
                 * possible to override the xex auto-detection. */
                load = -1;
            }
        }
    }

    if(load_addr != NULL)
    {
        *load_addr = load;
    }
    return in;
}

static void load_xex(unsigned char mem[65536], FILE *in,
                     int *startp, int *endp, int *runp)
{
    int run = -1;
    int jsr = -1;
    int min = 65536, max = 0;

    goto initial_state;
    for(;;)
    {
        int start, end, len;

        start = fgetc(in);
        if(start == EOF) break;
        ungetc(start, in);

        start = get_word(in);
        if(start == 0xffff)
        {
            /* allowed optional header */
        initial_state:
            start = get_word(in);
        }
        end = get_word(in);
        if(start > 0xffff || end > 0xffff || end < start)
        {
            LOG(LOG_ERROR, ("Error: corrupt data in xex-file."));
            fclose(in);
            exit(-1);
        }
        if(start == 0x2e2 && end == 0x2e3)
        {
            /* init vector */
            jsr = get_word(in);
            continue;
        }
        if(start == 0x2e0 && end == 0x2e1)
        {
            /* run vector */
            run = get_word(in);
            LOG(LOG_DEBUG, ("Found xex runad %04X.\n", run));
            continue;
        }
        ++end;
        jsr = -1;
        if(start < min) min = start;
        if(end > max) max = end;

        len = fread(mem + start, 1, end - start, in);
        if(len != end - start)
        {
            LOG(LOG_ERROR, ("Error: unexpected end of xex-file."));
            fclose(in);
            exit(-1);
        }
        LOG(LOG_VERBOSE, (" xex chunk loading from $%04X to $%04X\n",
                          start, end));
    }

    if(run == -1 && jsr != -1) run = jsr;

    if(startp != NULL) *startp = min;
    if(endp != NULL) *endp = max;
    if(run != -1 && runp != NULL) *runp = run;
}

static void load_located(char *filename, unsigned char mem[65536],
                         int *startp, int *endp, int *runp)
{
    int sp, ep;
    int load;
    FILE *in;

    in = open_file(filename, &load);
    if(load == -1)
    {
        /* file is an xex file */
        load_xex(mem, in, &sp, &ep, runp);
    }
    else
    {
        int len;
        load &= 0xffff;
        len = fread(mem + load, 1, 65536 - load, in);
        sp = load;
        ep = load + len;
    }
    fclose(in);

    LOG(LOG_NORMAL,
        (" filename: \"%s\", loading from $%04X to $%04X\n",
         filename, sp, ep));

    if(startp != NULL) *startp = sp;
    if(endp != NULL) *endp = ep;
}

static
int
do_load(char *file_name, struct membuf *mem)
{
    int start, end;
    unsigned char *p;

    membuf_clear(mem);
    membuf_append(mem, NULL, 65536);
    p = membuf_get(mem);

    load_located(file_name, p, &start, &end, NULL);

    /* move memory to beginning of buffer */
    membuf_truncate(mem, end);
    membuf_trim(mem, start);

    LOG(LOG_NORMAL, (" crunching from $%04X to $%04X ", start, end));
    return start;
}

static
int
do_loads(int filec, char *filev[], struct membuf *mem,
         int basic_txt_start, int *basic_var_startp, int *runp)
{
    int run = -1;
    int min_start = 65537;
    int max_end = -1;
    int basic_code = 0;
    int i;
    unsigned char *p;

    membuf_clear(mem);
    membuf_append(mem, NULL, 65536);
    p = membuf_get(mem);

    for (i = 0; i < filec; ++i)
    {
        int start, end;

        load_located(filev[i], p, &start, &end, &run);
        if(run != -1 && runp != NULL)
        {
            LOG(LOG_DEBUG, ("Propagating xex runad %04X.\n", run));
            *runp = run;
        }

        /* if any file loads to the basic start */
        if(basic_txt_start >= 0)
        {
            if(start <= basic_txt_start && end > basic_txt_start)
            {
                basic_code = 1;
                if(basic_var_startp != NULL)
                {
                    *basic_var_startp = end;
                }
                else if(run == -1)
                {
                    /* only if we didn't get run address from load_located */
                    int sys_addr = find_sys(p + basic_txt_start);
                    if(runp != NULL)
                    {
                        *runp = sys_addr;
                    }
                }
            }
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

    if(basic_txt_start >= 0 && !basic_code && run == -1)
    {
        /* no program loaded to the basic start */
        LOG(LOG_ERROR, ("\nError: nothing loaded at the start of basic "
                        "text address ($%04X).\n",
                        basic_txt_start));
        exit(-1);
    }

    /* if we have a basic code loaded and we are doing a proper basic start */
    if(basic_code && basic_var_startp != NULL)
    {
        int valuepos = basic_txt_start - 1;
        /* the byte immediatley preceeding the basic start must be 0
         * for basic to function properly. */
        if(min_start > valuepos)
        {
            /* It not covered by the files to crunch. Since the
             * default fill value is 0 we don't need to set it but we
             * need to include that location in the crunch as well. */
            min_start = valuepos;
        }
        else
        {
            int value = p[valuepos];
            /* it has been covered by at least one file. Let's check
             * if it is zero. */
            if(value != 0)
            {
                /* Hm, its not, danger Will Robinson! */
                LOG(LOG_WARNING,
                    ("Warning, basic will probably not work since the value of"
                     " the location \npreceeding the basic start ($%04X)"
                     " is not 0 but %d.\n", valuepos, value));
            }
        }
    }

    /* move memory to beginning of buffer */
    membuf_truncate(mem, max_end);
    membuf_trim(mem, min_start);

    LOG(LOG_NORMAL, (" crunching from $%04X to $%04X ", min_start, max_end));
    return min_start;
}

static
void print_usage(const char *appl, enum log_level level)
{
    /* done */
    LOG(level,
        ("usage: %s level|mem|sfx|raw [option]... infile[,<address>]...\n"
         "  see the individual commands for more help.\n",
         appl));
}

static
void print_level_usage(const char *appl, enum log_level level,
                       const char *default_outfile)
{
    /* done */
    LOG(level,
        ("usage: %s level [option]... infile[,<address>]...\n"
         "  The level command generates outfiles that are intended to be decrunched on\n"
         "  the fly while being read.\n", appl));
    print_shared_flags(level, default_outfile);
    LOG(level,
        (" All infiles are crunched separately and concatenated in the outfile in the\n"
         " order they are given on the command-line.\n"));
}

static
void print_mem_usage(const char *appl, enum log_level level,
                     const char *default_outfile)
{
    /* done */
    LOG(level,
        ("usage: %s mem [option]... infile[,<address>]...\n"
         "  The mem command generates outfiles that are intended to be decrunched from\n"
         "  memory after being loaded or assembled there.\n", appl));
    LOG(level,
        ("  -l <address>  adds load address to the outfile, using \"none\" as <address>\n"
         "                will skip the load address.\n"));
    print_shared_flags(level, default_outfile);
    LOG(level,
        (" All infiles are merged into the outfile. They are loaded in the order\n"
         " they are given on the command-line, from left to right.\n"));
}

static
void print_raw_usage(const char *appl, enum log_level level,
                     const char *default_out_name)
{
    LOG(level, ("usage: %s [option]... infile\n", appl));
    LOG(level,
        ("  -b            crunch/decrunch backwards\n"
         "  -r            write outfile in reverse order\n"
         "  -d            decrunch (instead of crunch)\n"));
    print_shared_flags(level, default_out_name);
}

static
void print_sfx_usage(const char *appl, enum log_level level,
                     const char *default_outfile)
{
    /* done */
    LOG(level,
        ("usage: %s sfx basic[,<start>[,<end>[,<high>]]]|sys[,<start>]|<jmpaddress> [option]... infile[,<address>]...\n"
         "  The sfx command generates outfiles that are intended to decrunch themselves.\n"
         "  The basic start argument will start a basic program.\n"
         "  The sys start argument will auto detect the start address by searching the\n"
         "  basic start for a sys command.\n"
         "  the <jmpaddress> start argument will jmp to the given address.\n", appl));
    LOG(level,
        ("  -t            sets the decruncher target, must be one of 4, 20, 23, 52, 55,\n"
         "                64, 128 or 168, default is 64.\n"
         "  -x<assembler fragment>\n"
         "                User specified effect. Must not modify X reg, Y reg and carry\n"
         "  -D<symbol>=<value>\n"
         "                predefines symbols for the sfx assembler.\n"));
    print_shared_flags(level, default_outfile);
    LOG(level,
        (" All infiles are merged into the outfile. They are loaded in the order\n"
         " they are given on the command-line, from left to right.\n"));
}

void level(const char *appl, int argc, char *argv[])
{
    int literal_sequences_used = 0;
    int max_safety = 0;
    int c;
    int infilec;
    char **infilev;

    struct crunch_options options[1] = { CRUNCH_OPTIONS_DEFAULT };
    struct common_flags flags[1] = {{NULL, DEFAULT_OUTFILE}};

    struct membuf in[1];
    struct membuf out[1];

    flags->options = options;

    LOG(LOG_DUMP, ("flagind %d\n", flagind));
    while ((c = getflag(argc, argv, SHARED_FLAGS)) != -1)
    {
        LOG(LOG_DUMP, (" flagind %d flagopt '%c'\n", flagind, c));
        handle_shared_flags(c, flagarg, print_level_usage, appl, flags);
    }

    membuf_init(in);
    membuf_init(out);

    infilev = argv + flagind;
    infilec = argc - flagind;

    if (infilec == 0)
    {
        LOG(LOG_ERROR, ("Error: no input files to process.\n"));
        print_level_usage(appl, LOG_NORMAL, DEFAULT_OUTFILE);
        exit(-1);
    }

    /* append the files instead of merging them */
    for(c = 0; c < infilec; ++c)
    {
        struct crunch_info info[1];
        int in_load;
        int in_len;
        int out_pos;
        out_pos = membuf_memlen(out);

        in_load = do_load(infilev[c], in);
        in_len = membuf_memlen(in);
        crunch_backwards(in, out, options, info);

        if(info->literal_sequences_used)
        {
            literal_sequences_used = 1;
        }
        if(info->needed_safety_offset > max_safety)
        {
            max_safety = info->needed_safety_offset;
        }

        /* append the starting address of decrunching */
        membuf_append_char(out, (in_load + in_len) & 255);
        membuf_append_char(out, (in_load + in_len) >> 8);

        /* reverse the just appended segment of the out buffer */
        reverse_buffer((char*)membuf_get(out) + out_pos,
                       membuf_memlen(out) - out_pos);
    }

    LOG(LOG_NORMAL, (" Literal sequences are %sused and",
                     literal_sequences_used ? "" : "not "));
    LOG(LOG_NORMAL, (" the largest safety offset is %d.\n",
                     max_safety));

    write_file(flags->outfile, out);

    membuf_free(out);
    membuf_free(in);
}

void mem(const char *appl, int argc, char *argv[])
{
    int load_addr = -1;
    int prepend_load_addr = 1;
    char flags_arr[32];
    int c;
    int infilec;
    char **infilev;

    struct crunch_options options[1] = { CRUNCH_OPTIONS_DEFAULT };
    struct common_flags flags[1] = {{NULL, DEFAULT_OUTFILE}};

    struct membuf in[1];
    struct membuf out[1];

    flags->options = options;

    LOG(LOG_DUMP, ("flagind %d\n", flagind));
    sprintf(flags_arr, "l:%s", SHARED_FLAGS);
    while ((c = getflag(argc, argv, flags_arr)) != -1)
    {
        LOG(LOG_DUMP, (" flagind %d flagopt '%c'\n", flagind, c));
        switch(c)
        {
        case 'l':
            if(strcmp(flagarg, "none") == 0)
            {
                prepend_load_addr = 0;
            }
            else if(str_to_int(flagarg, &load_addr) != 0 ||
                    load_addr < 0 || load_addr >= 65536)
            {
                LOG(LOG_ERROR,
                    ("Error: invalid address for -l option, "
                     "must be in the range of [0 - 0xffff]\n"));
                print_mem_usage(appl, LOG_NORMAL, DEFAULT_OUTFILE);
                exit(-1);
            }
            break;
        default:
            handle_shared_flags(c, flagarg, print_mem_usage, appl, flags);
        }
    }

    membuf_init(in);
    membuf_init(out);

    infilev = argv + flagind;
    infilec = argc - flagind;

    if (infilec == 0)
    {
        LOG(LOG_ERROR, ("Error: no input files to process.\n"));
        print_mem_usage(appl, LOG_NORMAL, DEFAULT_OUTFILE);
        exit(-1);
    }

    {
        struct crunch_info info[1];
        int in_load;
        int in_len;
        int safety;

        in_load = do_loads(infilec, infilev, in, -1, NULL, NULL);
        in_len = membuf_memlen(in);

        /* make room for load addr */
        if(prepend_load_addr)
        {
            membuf_append(out, NULL, 2);
        }

        crunch_backwards(in, out, options, info);
        safety = info->needed_safety_offset;

        /* append the in_loading address of decrunching */
        membuf_append_char(out, (in_load + in_len) & 255);
        membuf_append_char(out, (in_load + in_len) >> 8);

        /* prepend load addr */
        if(prepend_load_addr)
        {
            char *p;
            if(load_addr < 0)
            {
                /* auto load addr specified */
                load_addr = in_load - safety;
            }
            p = membuf_get(out);
            p[0] = load_addr & 255;
            p[1] = load_addr >> 8;
            LOG(LOG_NORMAL, (" The load address is $%04X.\n", load_addr));
        }
        else
        {
            LOG(LOG_NORMAL, (" No load address is used.\n", load_addr));
        }

        LOG(LOG_NORMAL, (" Literal sequences are %sused and",
                         info->literal_sequences_used ? "" : "not "));
        LOG(LOG_NORMAL, (" the safety offset is %d.\n",
                         info->needed_safety_offset));
    }

    write_file(flags->outfile, out);

    membuf_free(out);
    membuf_free(in);
}

struct target_info
{
    int id;
    int basic_txt_start;
    const char *model;
};

static
const struct target_info *
get_target_info(int target)
{
    static const struct target_info targets[] =
        {
            {20,  0x1001, "Vic20"},
            {23,  0x0401, "Vic20+3kB"},
            {52,  0x1201, "Vic20+32kB"},
            {55,  0x1201, "Vic20+3kB+32kB"},
            {4,   0x1001, "C16/plus4"},
            {64,  0x0801, "C64"},
            {128, 0x1c01, "C128"},
            {168, 0x2c00, "Atari 400/800 XL/XE"},
            {0, 0, NULL}
        };
    const struct target_info *targetp;
    for(targetp = targets; targetp->id != 0; ++targetp)
    {
        if(target == targetp->id)
        {
            break;
        }
    }
    if(targetp->id == 0)
    {
        targetp = NULL;
    }
    return targetp;
}

void sfx(const char *appl, int argc, char *argv[])
{
    int in_load;
    int basic_txt_start = -1;
    int basic_var_start = -1;
    int basic_highest_addr = -1;
    int decr_target = 64;
    int sys_addr = -1;
    char flags_arr[32];
    int c;
    int infilec;
    char **infilev;

    struct crunch_info info[1];

    struct crunch_options options[1] = { CRUNCH_OPTIONS_DEFAULT };
    struct common_flags flags[1] = {{NULL, DEFAULT_OUTFILE}};
    const struct target_info *targetp;

    struct membuf buf1[1];

    struct membuf *in;
    struct membuf *out;
    struct membuf *fx = NULL;

    flags->options = options;

    if(argc <= 1)
    {
        LOG(LOG_ERROR, ("Error: no start argument given.\n"));
        print_sfx_usage(appl, LOG_NORMAL, DEFAULT_OUTFILE);
        exit(-1);
    }

    parse_init();

    /* required argument: how to start the crunched program */
    do
    {
        char *p = strtok(argv[1], ",");
        if (strcmp(p, "sys") == 0)
        {
            /* we should look for a basic sys command. */
            sys_addr = -1;
            p = strtok(NULL, ",");
            /* look for an optional basic start address */
            if(p == NULL) break;
            if(str_to_int(p, &basic_txt_start) != 0)
            {
                LOG(LOG_ERROR,
                    ("Error: invalid value for the start of basic text "
                     "address.\n"));
                print_sfx_usage(appl, LOG_NORMAL, DEFAULT_OUTFILE);
                exit(-1);
            }
        }
        else if(strcmp(p, "basic") == 0)
        {
            /* we should start a basic program. */
            sys_addr = -2;
            p = strtok(NULL, ",");
            /* look for an optional basic start address */
            if(p == NULL) break;
            if(str_to_int(p, &basic_txt_start) != 0)
            {
                LOG(LOG_ERROR,
                    ("Error: invalid value for the start of basic text "
                     "address.\n"));
                print_sfx_usage(appl, LOG_NORMAL, DEFAULT_OUTFILE);
                exit(-1);
            }
            p = strtok(NULL, ",");
            /* look for an optional basic end address */
            if(p == NULL) break;
            if(str_to_int(p, &basic_var_start) != 0)
            {
                LOG(LOG_ERROR,
                    ("Error: invalid value for the start of basic "
                     "variables address.\n"));
                print_sfx_usage(appl, LOG_NORMAL, DEFAULT_OUTFILE);
                exit(-1);
            }
            p = strtok(NULL, ",");
            /* look for an optional highest address used by basic */
            if(p == NULL) break;
            if(str_to_int(p, &basic_highest_addr) != 0)
            {
                LOG(LOG_ERROR,
                    ("Error: invalid value for the highest address used "
                     "by basic address.\n"));
                print_sfx_usage(appl, LOG_NORMAL, DEFAULT_OUTFILE);
                exit(-1);
            }
        }
        else if(str_to_int(p, &sys_addr) != 0 ||
                sys_addr < 0 || sys_addr >= 65536)
        {
            /* we got an address we should jmp to. */
            LOG(LOG_ERROR,
                ("Error: invalid address for <start>, "
                 "must be in the range of [0 - 0xffff]\n"));
            print_sfx_usage(appl, LOG_NORMAL, DEFAULT_OUTFILE);
            exit(-1);
        }
    }
    while(0);

    LOG(LOG_DUMP, ("flagind %d\n", flagind));
    sprintf(flags_arr, "D:t:x:%s", SHARED_FLAGS);
    while ((c = getflag(argc, argv, flags_arr)) != -1)
    {
        char *p;
        LOG(LOG_DUMP, (" flagind %d flagopt '%c'\n", flagind, c));
        switch(c)
        {
        case 't':
            if (str_to_int(flagarg, &decr_target) != 0 ||
                get_target_info(decr_target) == NULL)
            {
                LOG(LOG_ERROR,
                    ("error: invalid value, %d, for -t option, "
                     "must be one of 4, 20, 23, 52, 55, 64, 128 or 168.",
                     decr_target));
                print_sfx_usage(appl, LOG_NORMAL, DEFAULT_OUTFILE);
                exit(-1);
            }
            break;
        case 'x':
            fx = new_named_buffer("user_effect");
            membuf_append(fx, flagarg, strlen(flagarg));
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
                    print_sfx_usage(appl, LOG_NORMAL, DEFAULT_OUTFILE);
                    exit(-1);
                }
                /* This is ugly, we really should allocate our own
                 * copy of the symbol string. */
                *p = '\0';
                new_symbol(flagarg, value);
            }
            else
            {
                LOG(LOG_ERROR, ("Error: invalid value for -D "
                                "<symbol>=<value> option.\n"));
                print_sfx_usage(appl, LOG_NORMAL, DEFAULT_OUTFILE);
                exit(-1);
            }
            break;
        default:
            handle_shared_flags(c, flagarg, print_sfx_usage, appl, flags);
        }
    }

    membuf_init(buf1);
    in = buf1;
    out = new_named_buffer("crunched_data");

    infilev = argv + flagind + 1;
    infilec = argc - flagind - 1;

    if (infilec == 0)
    {
        LOG(LOG_ERROR, ("Error: no input files to process.\n"));
        print_sfx_usage(appl, LOG_NORMAL, DEFAULT_OUTFILE);
        exit(-1);
    }

    targetp = get_target_info(decr_target);
    if(basic_txt_start == -1)
    {
        basic_txt_start = targetp->basic_txt_start;
    }

    {
        int in_len;
        int safety;
        int *basic_var_startp;
        int *sys_addrp;
        int basic_start;

        sys_addrp = NULL;
        basic_var_startp = NULL;
        if(sys_addr == -2 && basic_var_start == -1)
        {
            basic_var_startp = &basic_var_start;
        }
        basic_start = -1;
        if(sys_addr < 0)
        {
            basic_start = basic_txt_start;
            if(sys_addr == -1)
            {
                sys_addrp = &sys_addr;
            }
        }

        in_load = do_loads(infilec, infilev, in,
                           basic_start, basic_var_startp, sys_addrp);
        in_len = membuf_memlen(in);

        if(decr_target == 20 || decr_target == 52)
        {
            /* these are vic20 targets with a memory hole from
             * $0400-$1000. Each page is filled with the value of the
             * high-byte of its address. */
            if(in_load >= 0x0400 && in_load + in_len <= 0x1000)
            {
                /* all the loaded data is in the memory hole.*/
                LOG(LOG_ERROR,
                    ("Error: all data loaded to the memory hole.\n"));
                exit(-1);
            }
            else if(in_load >= 0x0400 && in_load < 0x1000 &&
                    in_load + in_len > 0x1000)
            {
                /* The data starts in the memory hole and ends in
                 * RAM. We need to adjust the start. */
                int diff = 0x1000 - in_load;
                in_load += diff;
                in_len -= diff;
                membuf_trim(in, diff);
                LOG(LOG_WARNING,
                    ("Warning, trimming address interval to $%04X-$%04X.\n",
                     in_load, in_load + in_len));
            }
            else if(in_load < 0x0400 &&
                    in_load + in_len >= 0x0400 && in_load + in_len < 0x1000)
            {
                /* The data starts in RAM and ends in the memory
                 * hole. We need to adjust the end. */
                int diff = in_load + in_len - 0x0400;
                in_len -= diff;
                membuf_truncate(in, in_len);
                LOG(LOG_WARNING,
                    ("Warning, trimming address interval to $%04X-$%04X.\n",
                     in_load, in_load + in_len));
            }
            else if(in_load < 0x0400 && in_load + in_len > 0x1000)
            {
                /* The data starts in RAM covers the memory hole and
                 * ends in RAM. */
                int hi, lo;
                char *p;
                p = membuf_get(in);
                for(hi = 0x04; hi < 0x10; hi += 0x01)
                {
                    for(lo = 0; lo < 256; ++lo)
                    {
                        int addr = (hi << 8) | lo;
                        p[addr - in_load] = hi;
                    }
                }
                LOG(LOG_NORMAL, ("Memory hole at interval $0400-$1000 "
                                 "included in crunch..\n"));
            }
        }

        /* make room for load addr */
        membuf_append(out, NULL, 2);

        crunch_backwards(in, out, options, info);
        safety = info->needed_safety_offset;

        /* append the in_loading address of decrunching */
        membuf_append_char(out, (in_load + in_len) & 255);
        membuf_append_char(out, (in_load + in_len) >> 8);

        /* prepend load addr */
        {
            char *p;
            p = membuf_get(out);
            p[0] = (in_load - safety) & 255;
            p[1] = (in_load - safety) >> 8;
        }
    }

    LOG(LOG_NORMAL, (" Target is self-decrunching %s executable", targetp->model));
    if(sys_addr == -1)
    {
        LOG(LOG_ERROR, ("\nError: cant find sys address at basic "
                        "text start ($%04X).\n", basic_txt_start));
        exit(-1);
    }
    if(sys_addr != -2)
    {
        LOG(LOG_NORMAL, (",\n jmp address $%04X.\n", sys_addr));
    }
    else
    {
        LOG(LOG_NORMAL, (",\n basic start ($%04X-$%04X).\n",
                         basic_txt_start, basic_var_start));
    }

    {
        /* add decruncher */
        struct membuf source[1];

        membuf_init(source);
        decrunch(LOG_DEBUG, sfxdecr, source);

        in = out;
        out = buf1;
        membuf_clear(out);

        new_symbol("r_start_addr", sys_addr);
        /*symbol_dump_resolved(LOG_NORMAL, "r_start_addr");*/
        new_symbol("r_target", decr_target);
        /*symbol_dump_resolved(LOG_NORMAL, "r_target");*/

        if(sys_addr == -2)
        {
            /* only set this if its changed from the default. */
            if(basic_txt_start != targetp->basic_txt_start)
            {
                new_symbol("i_basic_txt_start", basic_txt_start);
                symbol_dump_resolved(LOG_DEBUG, "i_basic_txt_start");
            }
            /* only set this if we've been given a value for it. */
            if(basic_var_start != -1)
            {
                new_symbol("i_basic_var_start", basic_var_start);
                symbol_dump_resolved(LOG_DEBUG, "i_basic_var_start");
            }
            /* only set this if we've been given a value for it. */
            if(basic_highest_addr != -1)
            {
                new_symbol("i_basic_highest_addr", basic_highest_addr);
                symbol_dump_resolved(LOG_DEBUG, "i_basic_highest_addr");
            }
        }

        if(fx != NULL)
        {
            new_symbol("i_user_effect", 1);
            symbol_dump_resolved(LOG_DEBUG, "i_user_effect");
        }

        if(info->literal_sequences_used)
        {
            new_symbol("i_literal_sequences_used", 1);
            symbol_dump_resolved(LOG_DEBUG, "i_literal_sequences_used");
        }

        if(assemble(source, out) != 0)
        {
            LOG(LOG_ERROR, ("Parse failure.\n"));
        }

        membuf_free(source);
    }

    write_file(flags->outfile, out);

    membuf_free(buf1);

    parse_free();
}

void raw(const char *appl, int argc, char *argv[])
{
    char flags_arr[32];
    int decrunch_mode = 0;
    int backwards_mode = 0;
    int reverse_mode = 0;
    int c, infilec;
    char **infilev;

    static struct crunch_options options[1] = { CRUNCH_OPTIONS_DEFAULT };
    struct common_flags flags[1] = { {options, DEFAULT_OUTFILE} };

    struct membuf inbuf[1];
    struct membuf outbuf[1];

    LOG(LOG_DUMP, ("flagind %d\n", flagind));
    sprintf(flags_arr, "bdr%s", SHARED_FLAGS);
    while ((c = getflag(argc, argv, flags_arr)) != -1)
    {
        LOG(LOG_DUMP, (" flagind %d flagopt '%c'\n", flagind, c));
        switch (c)
        {
        case 'b':
            backwards_mode = 1;
            break;
        case 'r':
            reverse_mode = 1;
            break;
        case 'd':
            decrunch_mode = 1;
            break;
        default:
            handle_shared_flags(c, flagarg, print_raw_usage, appl, flags);
        }
    }

    infilev = argv + flagind;
    infilec = argc - flagind;

    if (infilec != 1)
    {
        LOG(LOG_ERROR, ("Error: exactly one input file must be given.\n"));
        print_raw_usage(appl, LOG_NORMAL, DEFAULT_OUTFILE);
        exit(-1);
    }

    membuf_init(inbuf);
    membuf_init(outbuf);

    read_file(infilev[0], inbuf);

    if(decrunch_mode)
    {
        int seems_backward = 0;
        int seems_forward = 0;
        unsigned char *p;

        p = membuf_get(inbuf);
        if(p[0] == 0x80 && p[1] == 0x0)
        {
            seems_backward = 1;
        }
        p += membuf_memlen(inbuf);
        if(p[-1] == 0x80 && p[-2] == 0x0)
        {
            seems_forward = 1;
        }

        /* do we know what way it was crunched? */
        if((seems_backward ^ seems_forward) != 0)
        {
            /* yes, override option. */
            backwards_mode = seems_backward;
        }

        if(backwards_mode)
        {
            LOG(LOG_NORMAL, ("Decrunching infile \"%s\" to outfile \"%s\" "
                             " backwards.\n", infilev[0], flags->outfile));
            decrunch_backwards(LOG_NORMAL, inbuf, outbuf);
        }
        else
        {
            LOG(LOG_NORMAL, ("Decrunching infile \"%s\" to outfile \"%s\".\n",
                             infilev[0], flags->outfile));
            decrunch(LOG_NORMAL, inbuf, outbuf);
        }
    }
    else
    {
        struct crunch_info info[1];
        if(backwards_mode)
        {
            LOG(LOG_NORMAL, ("Crunching infile \"%s\" to outfile \"%s\" "
                             "backwards.\n", infilev[0], flags->outfile));
            crunch_backwards(inbuf, outbuf, options, info);
        }
        else
        {
            LOG(LOG_NORMAL, ("Crunching infile \"%s\" to outfile \"%s\".\n",
                             infilev[0], flags->outfile));
            crunch(inbuf, outbuf, options, info);
        }

        LOG(LOG_NORMAL, (" Literal sequences are %sused and",
                         info->literal_sequences_used ? "" : "not "));
        LOG(LOG_NORMAL, (" the safety offset is %d.\n",
                         info->needed_safety_offset));

    }

    if(reverse_mode)
    {
        reverse_buffer(membuf_get(outbuf), membuf_memlen(outbuf));
    }

    write_file(flags->outfile, outbuf);

    membuf_free(outbuf);
    membuf_free(inbuf);
}

int
main(int argc, char *argv[])
{
    const char *appl;

    /* init logging */
    LOG_INIT_CONSOLE(LOG_NORMAL);

    appl = fixup_appl(argv[0]);
    if(argc < 2)
    {
        /* missing required command */
        LOG(LOG_ERROR,
            ("Error: required command is missing, please use level, "
             "mem or sfx.\n"));
        print_usage(appl, LOG_ERROR);
        exit(-1);
    }
    ++argv;
    --argc;
    if(strcmp(argv[0], "level") == 0)
    {
        level(appl, argc, argv);
    }
    else if(strcmp(argv[0], "mem") == 0)
    {
        mem(appl, argc, argv);
    }
    else if(strcmp(argv[0], "sfx") == 0)
    {
        sfx(appl, argc, argv);
    }
    else if(strcmp(argv[0], "raw") == 0)
    {
        raw(appl, argc, argv);
    }
    else
    {
        /* unknown command */
        LOG(LOG_ERROR,
            ("Error: unrecognised command, please use level, mem or sfx.\n"));
        print_usage(appl, LOG_ERROR);
        exit(-1);
    }

    LOG_FREE;

    return 0;
}
