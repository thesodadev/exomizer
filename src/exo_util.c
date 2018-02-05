/*
 * Copyright (c) 2008 Magnus Lind.
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

#include "exo_util.h"
#include "log.h"
#include "stdlib.h"
#include "string.h"

int find_sys(const unsigned char *buf, int sys_token, int *stub_lenp)
{
    int outstart = -1;
    int state = 1;

    /* skip link and line number */
    int i = 4;
    /* exit loop at line end */
    while(i < 1000 && buf[i] != '\0')
    {
        unsigned char *sys_end;
        int c = buf[i];
        switch(state)
        {
            /* look for and consume sys token */
        case 1:
            if((sys_token == -1 &&
                (c == 0x9e /* cbm */ ||
                 c == 0x8c /* apple 2*/ ||
                 c == 0xbf /* oric 1*/)) ||
               c == sys_token)
            {
                state = 2;
            }
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
    if (stub_lenp != NULL)
    {
        /* skip zero byte + next zero link word */
        *stub_lenp = i + 3;
    }

    LOG(LOG_DEBUG, ("state when leaving: %d.\n", state));
    return outstart;
}

static int get_byte(FILE *in)
{
    int byte = fgetc(in);
    if(byte == EOF)
    {
        LOG(LOG_ERROR, ("Error: unexpected end of xex-file."));
        fclose(in);
        exit(1);
    }
    return byte;
}

static int get_le_word(FILE *in)
{
    int word = get_byte(in);
    word |= get_byte(in) << 8;
    return word;
}

static int get_be_word(FILE *in)
{
    int word = get_byte(in) << 8;
    word |= get_byte(in);
    return word;
}

/**
 * if the file is detected to be xex then load_addr will be set to -1
 * if the file is detected to be oric tap then load_addr will be set to -2
 * if the name contains no len info then *lenp will be set to -1. If the name
 * contains negative len then *lenp will be set to len - 1.
 * if the name contains no offset info then *offsetp will be set to 0.
 * if the file is detected as a prg then the prg header will be read.
 */
static
FILE *
open_file(char *name, int *load_addr, int *offsetp, int *lenp)
{
    FILE *in;
    int is_plain = 0;
    int is_relocated = 0;

    int tries;
    char *tries_arr[3];
    for (tries = 0;; ++tries)
    {
        char *load_str;
        char *at_str;

        in = fopen(name, "rb");
        if (in != NULL || is_plain == 1 || tries == 3)
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
            is_plain = 1;
            load_str = at_str;
        }

        if (load_str == NULL)
        {
            /* nope, */
            break;
        }

        *load_str = '\0';
        is_relocated = 1;
        ++load_str;

        /* relocation was requested */
        tries_arr[tries] = load_str;
    }
    if (in == NULL)
    {
        LOG(LOG_ERROR,
            (" can't open file \"%s\" for input\n", name));
        exit(1);
    }

    {
        int load = -3;
        int offset = 0;
        int len = -1;
        if (--tries >= 0)
        {
            char *p = tries_arr[tries];
            if (p[0] != '\0' && str_to_int(p, &load) != 0)
            {
                /* we fail */
                LOG(LOG_ERROR, (" can't parse load address from \"%s\"\n", p));
                exit(1);
            }
        }
        if (--tries >= 0)
        {
            char *p = tries_arr[tries];
            if (p[0] != '\0' && str_to_int(p, &offset) != 0)
            {
                /* we fail */
                LOG(LOG_ERROR, (" can't parse offset from \"%s\"\n", p));
                exit(1);
            }
        }
        if (--tries >= 0)
        {
            int val;
            char *p = tries_arr[tries];
            if (p[0] != '\0')
            {
                if (str_to_int(p, &val) != 0)
                {
                    /* we fail */
                    LOG(LOG_ERROR, (" can't parse length from \"%s\"\n", p));
                    exit(1);
                }
                if (val < 0)
                {
                    /* make room for -1 to mean no explicit len */
                    val -= 1;
                }
                len = val;
            }
        }

        if(!is_plain)
        {
            /* read the prg load address */
            int prg_load = get_le_word(in);
            if(!is_relocated || load == -3)
            {
                load = prg_load;
                /* unrelocated prg loading to $ffff is xex */
                if(prg_load == 0xffff)
                {
                    /* differentiate this from relocated $ffff files so it is
                     * possible to override the xex auto-detection. */
                    load = -1;
                }
                /* unrelocated prg loading to $1616 is Oric tap */
                else if(prg_load == 0x1616)
                {
                    load = -2;
                }
            }
        }

        if(load_addr != NULL)
        {
            *load_addr = load;
        }
        if(offsetp != NULL)
        {
            *offsetp = offset;
        }
        if(lenp != NULL)
        {
            *lenp = len;
        }
    }
    return in;
}

static void load_xex(unsigned char mem[65536], FILE *in,
                     struct load_info *info)
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

        start = get_le_word(in);
        if(start == 0xffff)
        {
            /* allowed optional header */
        initial_state:
            start = get_le_word(in);
        }
        end = get_le_word(in);
        if(start > 0xffff || end > 0xffff || end < start)
        {
            LOG(LOG_ERROR, ("Error: corrupt data in xex-file."));
            fclose(in);
            exit(1);
        }
        if(start == 0x2e2 && end == 0x2e3)
        {
            /* init vector */
            jsr = get_le_word(in);
            LOG(LOG_VERBOSE, ("Found xex initad $%04X.\n", jsr));
            continue;
        }
        if(start == 0x2e0 && end == 0x2e1)
        {
            /* run vector */
            run = get_le_word(in);
            LOG(LOG_VERBOSE, ("Found xex runad $%04X.\n", run));
            continue;
        }
        ++end;
        jsr = -1;
        if(start < min) min = start;
        if(end > max) max = end;

        len = fread(mem + start, 1, end - start, in);
        if(len != end - start)
        {
            LOG(LOG_ERROR, ("Error: unexpected end of xex-file.\n"));
            fclose(in);
            exit(1);
        }
        LOG(LOG_VERBOSE, (" xex chunk loading from $%04X to $%04X\n",
                          start, end));
    }

    if(run == -1 && jsr != -1) run = jsr;

    info->start = min;
    info->end = max;
    info->basic_var_start = -1;
    info->run = -1;
    if(run != -1)
    {
        info->run = run;
    }
}

static void load_oric_tap(unsigned char mem[65536], FILE *in,
                          struct load_info *info)
{
    int c;
    int autostart;
    int start, end, len;

    /* read oric tap header */

    /* next byte must be 0x16 as we have already read two and must
     * have at least three */
    if(get_byte(in) != 0x16)
    {
        LOG(LOG_ERROR, ("Error: fewer than three lead-in bytes ($16) "
                        "in Oric tap-file header.\n"));
        fclose(in);
        exit(1);
    }
    /* optionally more 0x16 bytes */
    while((c = get_byte(in)) == 0x16);
    /* next byte must be 0x24 */
    if(c != 0x24)
    {
        LOG(LOG_ERROR, ("Error: bad sync byte after lead-in in Oric tap-file "
                        "header, got $%02X but expected $24\n", c));
        fclose(in);
        exit(1);
    }

    /* now we are in sync, lets be lenient */
    get_byte(in); /* should be 0x0 */
    get_byte(in); /* should be 0x0 */
    get_byte(in); /* should be 0x0 or 0x80 */
    autostart = (get_byte(in) != 0);  /* should be 0x0, 0x80 or 0xc7 */
    end = get_be_word(in) + 1; /* the header end address is inclusive */
    start = get_be_word(in);
    get_byte(in); /* should be 0x0 */
    /* read optional file name */
    while(get_byte(in) != 0x0);

    /* read the data */
    len = fread(mem + start, 1, end - start, in);
    if(len != end - start)
    {
        LOG(LOG_BRIEF, ("Warning: Oric tap-file contains %d byte(s) data "
                        "less than expected.\n", end - start - len));
        end = start + len;
    }
    LOG(LOG_VERBOSE, (" Oric tap-file loading from $%04X to $%04X\n",
                      start, end));

    /* fill in the fields */
    info->start = start;
    info->end = end;
    info->run = -1;
    info->basic_var_start = -1;
    if(autostart)
    {
        info->run = start;
    }
    if(info->basic_txt_start >= start &&
       info->basic_txt_start < end)
    {
        info->basic_var_start = end - 1;
    }
}

/* (len == -1) => no length was given
 * (len >= 0) => given length == len
 * (len < -1) => given length == len + 1 */
static void load_prg(unsigned char mem[65536], FILE *in,
                     int offset, int len,
                     struct load_info *info)
{
    int header_offset;
    int file_len;

    header_offset = ftell(in);
    /* get the real length of the file and validate the offset*/
    if(fseek(in, 0, SEEK_END))
    {
        LOG(LOG_ERROR, ("Error: can't seek to EOF.\n"));
        fclose(in);
        exit(1);
    }
    file_len = ftell(in) - header_offset;
    if(offset < 0)
    {
        offset += file_len;
    }
    if(fseek(in, offset + header_offset, SEEK_SET))
    {
        LOG(LOG_ERROR, ("Error: can't seek to offset %d.\n", offset));
        fclose(in);
        exit(1);
    }
    if(len < 0)
    {
        len += file_len - offset + 1; /* + 1 convert back to given length */
    }
    if(len < 0)
    {
        LOG(LOG_ERROR, ("Error: can't read %d bytes from offset %d.\n",
                        len, offset));
        fclose(in);
        exit(1);
    }
    if (len == 0 || len > 65536 - info->start)
    {
        /* limit len to available buffer space */
        len = 65536 - info->start;
    }

    len = fread(mem + info->start, 1, len, in);

    info->end = info->start + len;
    info->basic_var_start = -1;
    info->run = -1;
    if(info->basic_txt_start >= info->start &&
       info->basic_txt_start < info->end)
    {
        info->basic_var_start = info->end;
    }
}

void load_located(char *filename, unsigned char mem[65536],
                  struct load_info *info)
{
    int load, offset, len;
    FILE *in;

    in = open_file(filename, &load, &offset, &len);
    if(load == -1)
    {
        /* file is an xex file */
        load_xex(mem, in, info);
    }
    else if(load == -2)
    {
        /* file is an oric tap file */
        load_oric_tap(mem, in, info);
    }
    else
    {
        /* file is a located plain file or a prg file */
        info->start = load;
        load_prg(mem, in, offset, len, info);
    }
    fclose(in);

    LOG(LOG_BRIEF,
        (" Reading \"%s\", loading from $%04X to $%04X.\n",
         filename, info->start, info->end));
}

/* returns 0 if ok, 1 otherwise */
int str_to_int(const char *str, int *value)
{
    int status = 0;
    do {
        char *str_end;
        long lval;

        /* base 0 is auto detect */
        int base = 0;

        if (*str == '\0')
        {
            /* no string to parse */
            status = 1;
            break;
        }

        if (*str == '$')
        {
            /* a $ prefix specifies base 16 */
            ++str;
            base = 16;
        }

        lval = strtol(str, &str_end, base);

        if(*str_end != '\0')
        {
            /* there is garbage in the string */
            status = 1;
            break;
        }

        if(value != NULL)
        {
            /* all is well, set the out parameter */
            *value = (int)lval;
        }
    } while(0);

    return status;
}

const char *fixup_appl(char *appl)
{
    char *applp;

    /* strip pathprefix from appl */
    applp = strrchr(appl, '\\');
    if (applp != NULL)
    {
        appl = applp + 1;
    }
    applp = strrchr(appl, '/');
    if (applp != NULL)
    {
        appl = applp + 1;
    }
    /* strip possible exe suffix */
    applp = appl + strlen(appl) - 4;
    if(strcmp(applp, ".exe") == 0 || strcmp(applp, ".EXE") == 0)
    {
        *applp = '\0';
    }
    return appl;
}
