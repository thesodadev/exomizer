#
# Makefile for exomizer
#

CFLAGS = -Wall -O3 -ffast-math -mtune=i686 -fomit-frame-pointer -fgcse -pedantic
LDFLAGS = -s

#CFLAGS = -g -pg -mtune=i686
#LDFLAGS = -g -pg -mtune=i686

SHARED_OBJS = getflag.o log.o
EXO_OBJS = match.o search.o exo_main2.o optimal.o output.o membuf_io.o \
	   chunkpool.o radix.o membuf.o exo_helper.o exodec.o progress.o
BAS_OBJS = bas_main.o bprg_renumber.o bprg_link_patch.o bprg_trampoline.o \
	   bprg.o vec.o membuf.o
ALL_OBJS = $(EXO_OBJS) $(BAS_OBJS) $(SHARED_OBJS)

#.SILENT:

.PHONY: build

build: $(MAKEFILE) exomizer exobasic

exomizer: deps $(EXO_OBJS) $(SHARED_OBJS)
	$(CC) $(LDFLAGS) -o $@ $(EXO_OBJS) $(SHARED_OBJS)

exobasic: deps $(BAS_OBJS) $(SHARED_OBJS)
	$(CC) $(LDFLAGS) -o $@ $(BAS_OBJS) $(SHARED_OBJS)

clean:
	-$(RM) $(EXO_OBJS) $(BAS_OBJS) $(SHARED_OBJS) exodec.o \
	exomizer exobasic exomizer.exe exobasic.exe deps

-include deps

deps: $(wildcard *.h)
	$(CC) -MM $(wildcard *.c) >$@
