#
# Makefile for perf
#
WFLAGS = -std=c89 -Wall -Wstrict-prototypes -D_XOPEN_SOURCE=600 -pedantic
CFLAGS = $(WFLAGS) -O3 -ffast-math -fomit-frame-pointer
LDFLAGS = -s

#CFLAGS = -g -mtune=i686 $(WFLAGS)
#LDFLAGS = -g -mtune=i686

DECR_OBJS = ../exodecrunch.os
CRUNCHER = ../../src/exomizer mem -q $<@0x1010 -o $@

TEST_OBJS = ../testrun.o ../../src/6502emu.o ../../src/exo_util.o ../../src/log.o ../../src/areatrace.o ../../src/vec.o ../../src/membuf_io.o ../../src/membuf.o

OBJS = ../main.os $(DECR_OBJS)


.PHONY: build %.test %.stat
.PRECIOUS: %.cru %.prg $(TEST_OBJS) %.cru.out
#.INTERMEDIATE: %.cru.out

build: data.test

%.stat:	test.prg ../testrun
	@$(MAKE) $(sort $(patsubst %.raw,%.cru,$(wildcard $**.raw)))
	@../testrun test.prg $(sort $(patsubst %.raw,%.cru,$(wildcard $**.raw)))

../testrun: testrun.o $(TEST_OBJS)
	@echo "Linking $@"
	@$(CC) $(LDFLAGS) -o $@ testrun.o $(TEST_OBJS)

%.test: %.cru.out %.raw
	cmp $< $*.raw

%.cru.out: test.prg %.cru ../testrun
	../testrun $< $*.cru

%.prg: $(OBJS) ../c64.cfg
	ld65 $(OBJS) -o $@ -C../c64.cfg

%.os: %.s
	ca65 $< -o $@

clean:
	$(RM) *.out *.cru *.prg

%.cru: %.raw
	$(CRUNCHER)

%.o:	%.c
	@echo "Compiling $<"
	@$(CC) -c $(CFLAGS) $(CPPFLAGS) -o $@ $<

%:	%.o
	@$(CC) $(LDFLAGS) $< -o $@

# cancel built in rule that disturb things
%.out: %
