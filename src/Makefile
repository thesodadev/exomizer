#
# Makefile for exomizer
#

#CFLAGS = -Wall -O3 -ffast-math -mtune=i686 -fomit-frame-pointer -fgcse -pedantic
#LDFLAGS = -s

CFLAGS = -g -pg -mtune=i686 -Wall
LDFLAGS = -g -pg -mtune=i686

SHARED_OBJS = getflag.o log.o membuf.o
RAW_OBJS = match.o search.o exo_raw.o optimal.o output.o membuf_io.o \
           chunkpool.o radix.o exo_helper.o exodec.o progress.o
EXO_OBJS = match.o search.o exo_main.o optimal.o output.o membuf_io.o \
           chunkpool.o radix.o exo_helper.o exodec.o progress.o asm.tab.o \
           lex.yy.o parse.o expr.o pc.o vec.o named_buffer.o prgdecr.o
BAS_OBJS = bas_main.o bprg_renumber.o bprg_link_patch.o bprg_trampoline.o \
           bprg.o vec.o
ALL_OBJS = $(EXO_OBJS) $(RAW_OBJS) $(BAS_OBJS) $(SHARED_OBJS)

#.SILENT:

.PHONY: build

build: $(MAKEFILE) exomizer exoraw exobasic

exomizer: deps $(EXO_OBJS) $(SHARED_OBJS)
	$(CC) $(LDFLAGS) -o $@ $(EXO_OBJS) $(SHARED_OBJS)

exoraw: deps $(RAW_OBJS) $(SHARED_OBJS)
	$(CC) $(LDFLAGS) -o $@ $(RAW_OBJS) $(SHARED_OBJS)

exobasic: deps $(BAS_OBJS) $(SHARED_OBJS)
	$(CC) $(LDFLAGS) -o $@ $(BAS_OBJS) $(SHARED_OBJS)

clean:
	-$(RM) $(EXO_OBJS) $(RAW_OBJS) $(BAS_OBJS) $(SHARED_OBJS) \
	b2membuf.o b2membuf b2membuf.exe \
	exomizer exoraw exobasic exomizer.exe exoraw.exe exobasic.exe deps

asm.tab.h asm.tab.c: asm.y
	bison -t -d asm.y

lex.yy.c: asm.yy
	flex -B asm.yy

prgdecr.c: prgdecr b2membuf
	./b2membuf prgdecr >prgdecr.c

prgdecr: prgdecr.s exoraw
	./exoraw -q prgdecr.s -o prgdecr

b2membuf: b2membuf.o
-include deps

deps: $(wildcard *.h)
	$(CC) -MM $(wildcard *.c) >$@
