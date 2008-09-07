
#include "desfx.h"
#include "6502emu.h"
#include "log.h"

u16 decrunch_sfx(u8 mem[65536], u16 run, u16 *startp, u16 *endp)
{
    struct cpu_ctx r[1];
    u16 start;
    u16 end;
    r->cycles = 0;
    r->mem = mem;
    r->pc = run;
    r->sp = '\xf6';
    r->flags = 0;

    LOG(LOG_DEBUG, ("run %04x\n", run));

    /* setting up decrunch */
    while(r->pc >= 0x0400 || r->sp != 0xf6)
    {
        next_inst(r);
    }
    end = r->mem[0xfe] + (r->mem[0xff] << 8);

    LOG(LOG_DEBUG, ("end %04x @%u\n", end, r->cycles));
    hex_dump(LOG_DEBUG, r->mem + 0x334, 156);

    /* decrunching */
    while(r->pc < 0x400)
    {
        next_inst(r);
    }
    start = r->mem[0xfe] + (r->mem[0xff] << 8);

    LOG(LOG_DEBUG, ("start %04x @%u\n", start, r->cycles));

    if(startp != NULL)
    {
        *startp = start;
    }
    if(endp != NULL)
    {
        *endp = end;
    }
    return r->pc;
}
