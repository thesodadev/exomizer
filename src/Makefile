#
# Makefile for exomizer
#

CFLAGS = -Wall -O3 -ffast-math -mcpu=i686 -fomit-frame-pointer -fgcse -pedantic
LDFLAGS = -s

#CFLAGS = -g
#LDFLAGS = -g

SHARED_OBJS = getflag.o log.o
EXO_OBJS = match.o search.o exo_main.o optimal.o output.o sfx264.o sfx64.o \
	   sfx64ne.o sfx264ne.o chunkpool.o radix.o
BAS_OBJS = bas_main.o bprg_renumber.o bprg_link_patch.o bprg_trampoline.o \
	   bprg.o vec.o membuf.o
ALL_OBJS = $(EXO_OBJS) $(BAS_OBJS) $(SHARED_OBJS)

.PHONY: build

build: $(MAKEFILE) exomizer exobasic exodec

exomizer: deps $(EXO_OBJS) $(SHARED_OBJS)
	$(CC) $(LDFLAGS) -o $@ $(EXO_OBJS) $(SHARED_OBJS)

exobasic: deps $(BAS_OBJS) $(SHARED_OBJS)
	$(CC) $(LDFLAGS) -o $@ $(BAS_OBJS) $(SHARED_OBJS)

exodec: exodec.o
	$(CC) $(LDFLAGS) -o $@ exodec.o

clean:
	-$(RM) $(EXO_OBJS) $(BAS_OBJS) $(SHARED_OBJS) exodec.o \
	exomizer exobasic exodec exomizer.exe exobasic.exe exodec.exe deps

-include deps

deps: $(wildcard *.h)
	$(CC) -MM $(wildcard *.c) >$@
