/*
 * Copyright (c) 2002 - 2008 Magnus Lind.
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

#include "../src/log.h"
#include "../src/exo_util.h"
#include "../src/6502emu.h"
#include "../src/areatrace.h"
#include "../src/int.h"
#include "../src/membuf_io.h"

#include <stdlib.h>

struct mem_ctx
{
    u8 *mem;
    struct areatrace at;
};

static void mem_access_write(struct mem_access *this, u16 address, u8 value)
{
    struct mem_ctx *ctx = this->ctx;
    ctx->mem[address] = value;
    areatrace_access(&ctx->at, address);
}

static u8 mem_access_read(struct mem_access *this, u16 address)
{
    struct mem_ctx *ctx = this->ctx;
    return ctx->mem[address];
}

void save_single(const char *in_name, struct membuf *mem, int start, int end)
{
    struct membuf mb_name;
    const char *out_name;

    membuf_init(&mb_name);

    membuf_printf(&mb_name, "%s.out", in_name);
    out_name = (const char *)membuf_get(&mb_name);

    membuf_truncate(mem, end);
    membuf_trim(mem, start);

    write_file(out_name, mem);

    membuf_free(&mb_name);
}

void test_single(const char *name)
{
    struct cpu_ctx r;
    struct load_info info[1];
    struct membuf mem_mb;
    u8 *mem;
    struct mem_ctx mem_ctx;
    int start;
    int end;

    membuf_init(&mem_mb);
    membuf_atleast(&mem_mb, 65536);
    mem = membuf_get(&mem_mb);
    mem_ctx.mem = mem;

    areatrace_init(&mem_ctx.at);

    info->basic_txt_start = 0x0801;
    load_located(name, mem, info);

    /* no start address from load*/
    if(info->run == -1)
    {
        /* look for sys line */
        info->run = find_sys(mem + info->basic_txt_start, 0x9e, NULL);
    }
    if(info->run == -1)
    {
        LOG(LOG_ERROR, ("Error, can't find entry point.\n"));
        exit(-1);
    }

    LOG(LOG_DEBUG, ("run %04x\n", info->run));

    r.cycles = 0;
    r.mem.ctx = &mem_ctx;
    r.mem.read = mem_access_read;
    r.mem.write = mem_access_write;
    r.pc = info->run;
    r.sp = '\xff';
    r.flags = 0;

    /* setting up decrunch */
    while(r.sp >= 0x10)
    {
        next_inst(&r);
    }
    LOG(LOG_BRIEF, ("decrunch took %u cycles.\n", r.cycles));

    /* save traced area */
    areatrace_merge_overlapping(&mem_ctx.at);
    areatrace_get_largest(&mem_ctx.at, &start, &end);

    LOG(LOG_BRIEF, ("saving from $%04X to $%04X.\n", start, end));

    save_single(name, &mem_mb, start, end);

    areatrace_free(&mem_ctx.at);
}

int main(int argc, char *argv[])
{
    int i;

    /* init logging */
    LOG_INIT_CONSOLE(LOG_BRIEF);

    for (i = 1; i < argc; ++i)
    {
        test_single(argv[i]);
    }

    return 0;
}
