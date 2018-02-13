/*
 * Copyright (c) 2007 Magnus Lind.
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

#include "desfx.h"
#include "6502emu.h"
#include "log.h"

struct mem_ctx
{
    u8 *mem;
    u16 start;
    u16 end;
};

static void mem_access_write(struct mem_access *this, u16 address, u8 value)
{
    struct mem_ctx *ctx = this->ctx;

    if ((ctx->mem[1] & 4) == 4 && (ctx->mem[1] & 3) != 0 &&
        address >= 0xd000 && address < 0xe000)
    {
        /* IO-area written and visible */
        return;
    }

    ctx->mem[address] = value;

    if (address < 0x334)
    {
        return;
    }

    if (address < ctx->start - 1 || address > ctx->end)
    {
        ctx->start = address;
        ctx->end = address + 1;
    }
    else if (address + 1 == ctx->start)
    {
        ctx->start = address;
    }
    else if (address == ctx->end)
    {
        ctx->end = address + 1;
    }
}

static u8 mem_access_read(struct mem_access *this, u16 address)
{
    struct mem_ctx *ctx = this->ctx;
    return ctx->mem[address];
}

u16 decrunch_sfx(u8 mem[65536], u16 run, u16 *startp, u16 *endp, u32 *cyclesp)
{
    struct mem_ctx m;
    struct cpu_ctx r;
    r.cycles = 0;
    r.mem.ctx = &m;
    r.mem.read = mem_access_read;
    r.mem.write = mem_access_write;
    r.pc = run;
    r.sp = '\xf6';
    r.flags = 0;

    m.start = 0xffff;
    m.end = 0;
    m.mem = mem;
    m.mem[1] = 0x37;

    LOG(LOG_DEBUG, ("run %04x\n", run));

    /* setting up decrunch */
    while(r.pc >= 0x0400 || r.sp != 0xf6)
    {
        next_inst(&r);
    }

    LOG(LOG_DEBUG, ("end %04x @%u\n", m.end, r.cycles));
    hex_dump(LOG_DEBUG, mem + 0x334, 156);

    /* decrunching */
    while(r.pc < 0x400)
    {
        next_inst(&r);
    }
    LOG(LOG_DEBUG, ("start %04x @%u\n", m.start, r.cycles));

    if(startp != NULL)
    {
        *startp = m.start;
    }
    if(endp != NULL)
    {
        *endp = m.end;
    }
    if(cyclesp != NULL)
    {
        *cyclesp = r.cycles;
    }
    return r.pc;
}
