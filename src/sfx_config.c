/*
 * Copyright (c) 2004 Magnus Lind.
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

#include "sfx_config.h"
#include "log.h"
#include <stdlib.h>

static struct sfx_config sfx_configs[] = {
    {20,  0x1001, 0,      0x0334, 0x1fff, 0x1fff, 0x1ff9, 0x97f9, 0x900f,
     0,   0,      0,      0,      "vic20"},
    {23,  0x0401, 0,      0x0334, 0x1fff, 0x1fff, 0x1ff9, 0x97f9, 0x900f,
     1,   0,      0,      0,      "vic20+3kB"},
    {52,  0x1201, 0,      0x0334, 0x7fff, 0x7fff, 0x11f9, 0x95f9, 0x900f,
     0,   0,      0,      0,      "vic20+32kB"},
    {55,  0x1201, 0,      0x0334, 0x7fff, 0x7fff, 0x11f9, 0x95f9, 0x900f,
     1,   0xa000, 0xbfff, 0,      "vic20+3kB+32kB"},
    {4,   0x1001, 1,      0x0334, 0xfcff, 0x3fff, 0x0fe7, 0x0be7, 0xff19,
     2,   0xff3f, 0xff3e, 0,      "c16/plus4"},
    {64,  0x0801, 0,      0x0334, 0xffff, 0x9fff, 0x07e7, 0xdbe7, 0xd020,
     3,     0x38,   0x37, 0x0001, "c64"},
    {128, 0x1c01, 1,      0x0b00, 0xffef, 0x3fff, 0x07e7, 0xdbe7, 0xd020,
     3,     0x3f,   0x00, 0xfff0, "c128"},
    {0}
};

void sfx_info_dump(int level)
{
    const struct sfx_config *sfx;
    const char *sep = "";
    int i = 0;
    for(;;)
    {
        sfx = &sfx_configs[i];
        if(sfx->id == 0)
        {
            sfx = NULL;
            break;
        }
        LOG(level, ("%s%d (%s)", sep, sfx->id, sfx->model));
        sep = ", ";
        ++i;
    }
    LOG(level, ("\n"));
}

const struct sfx_config *get_sfx_config(int config_id)
{
    const struct sfx_config *sfx;
    int i = 0;
    for(;;)
    {
        sfx = &sfx_configs[i];
        if(sfx->id == 0)
        {
            sfx = NULL;
            break;
        }
        if(sfx->id == config_id)
        {
            break;
        }
        ++i;
    }
    return sfx;
}
