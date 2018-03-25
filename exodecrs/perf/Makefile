#
# Makefile for perf
#
WFLAGS = -std=c89 -Wall -Wstrict-prototypes -D_XOPEN_SOURCE=600 -pedantic
CFLAGS = $(WFLAGS) -O3 -ffast-math -fomit-frame-pointer
LDFLAGS = -s

#CFLAGS = -g -mtune=i686 $(WFLAGS)
#LDFLAGS = -g -mtune=i686

TEST_OBJS = ../testrun.o ../../src/6502emu.o ../../src/exo_util.o ../../src/log.o ../../src/areatrace.o ../../src/vec.o ../../src/membuf_io.o ../../src/membuf.o

SOURCES = ../main.os ../exodecrunch.os

.PHONY: build %.test %.stat
.PRECIOUS: %.exo %.prg $(TEST_OBJS)
.INTERMEDIATE: %.exo.out

build: data.test

%.stat:	test.prg ../testrun
	@$(MAKE) $(sort $(patsubst %.raw,%.exo,$(wildcard $**.raw)))
	@../testrun test.prg $(sort $(patsubst %.raw,%.exo,$(wildcard $**.raw)))

../testrun: $(TEST_OBJS)
	@echo "Linking $@"
	@$(CC) $(LDFLAGS) -o $@ $(TEST_OBJS)

%.test: %.exo.out %.raw
	cmp $< $*.raw

%.exo.out: test.prg %.exo ../testrun
	../testrun $< $*.exo

%.prg: $(SOURCES) ../c64.cfg
	ld65 $(SOURCES) -o $@ -C../c64.cfg

%.os: %.s
	ca65 $< -o $@

clean:
	$(RM) data.test *.out *.exo *.prg

%.exo: %.raw
	../../src/exomizer mem -q $<@0x1000 -o $@

%.o:	%.c
	@echo "Compiling $<"
	@$(CC) -c $(CFLAGS) $(CPPFLAGS) -o $@ $<

%:	%.o
	@$(CC) $(LDFLAGS) $< -o $@

# cancel built in rule that disturb things
%.out: %
