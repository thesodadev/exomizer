#
# Makefile for exomizer
#

CFLAGS = -Wall -O3 -ffast-math -mcpu=i686 -fomit-frame-pointer -fgcse
LDFLAGS = -s

OBJS = match.o search.o main.o optimal.o output.o radix.o sfx.o getflag.o log.o

.PHONY: build

build: exomizer.deps exomizer

exomizer: $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^ -lm

clean:
	rm -f $(OBJS) exomizer

-include exomizer.deps

SRC_C = $(OBJS:.o=.c)
exomizer.deps: $(wildcard *.h)
	$(CC) -MM $(SRC_C) >$@
