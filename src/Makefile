#
# Makefile for exomizer
#

CFLAGS = -Wall -O3 -ffast-math -mcpu=i686 -fomit-frame-pointer -fgcse -pedantic
LDFLAGS = -s
#CFLAGS = -pg
#LDFLAGS = -pg

OBJS = match.o search.o main.o optimal.o output.o radix.o sfx264.o sfx64.o getflag.o log.o chunkpool.c

.PHONY: build

build: exomizer.deps exomizer

exomizer: $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^ -lm

clean:
	$(RM) $(OBJS) exomizer

-include exomizer.deps

SRC_C = $(OBJS:.o=.c)
exomizer.deps: $(wildcard *.h)
	$(CC) -MM $(SRC_C) >$@
