#ifndef INCLUDED_DECR_H
#define INCLUDED_DECR_H

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

#include "int.h"

/**
 * The value of bank_mode is interpreted as follows:
 *
 * 0: no RAM ROM banking is available. bank_ram, bank_rom and
 * bank_addr are ignored. There's a 3kB memory hole at [$0400-$0fff]
 * that reads back the page number. (256 * 4, 256 * 5, 256 * 6, ...)
 * The bank_ram and bank_rom contains the start and end addresses of
 * another ram area.
 *
 * 1: no RAM ROM banking is available. bank_ram, bank_rom and
 * bank_addr are ignored.
 *
 * 2: bank_ram, bank_rom contains memory locations that changes the
 * bank mode when written to. the ram only mode still has acces to
 * the io registers. bank_addr is ignored.
 *
 * 3: bank_ram, bank_rom contains values that are to be stored into
 * the bank_addr memory location in order to switch bank mode. The ram
 * only config makes the io-registers unavailable. The effect_color
 * and the border_color needs the io-registers to be banked in.
 *
 **/
struct sfx_config
{
    u16 id;
    u16 basic_start;
    u16 basic_picky;
    u16 table_addr;
    u16 end_of_mem_ram;
    u16 end_of_mem_rom;
    u16 effect_char;
    u16 effect_color;
    u16 effect_border;
    u16 bank_mode;
    u16 bank_ram;
    u16 bank_rom;
    u16 bank_addr;
    const char *model;
};

void sfx_info_dump(int level);

const struct sfx_config *get_sfx_config(int config_id);

#endif
