
WFLAGS = -Wall -Wstrict-prototypes
CFLAGS = $(WFLAGS) -O3 -ffast-math -fomit-frame-pointer -fgcse -pedantic
LDFLAGS = -s

#CFLAGS = -g -mtune=i686 $(WFLAGS)
#LDFLAGS = -g -mtune=i686

TEST_OBJS = ../testrun.o ../../src/6502emu.o ../../src/exo_util.o ../../src/log.o ../../src/areatrace.o ../../src/vec.o ../../src/membuf_io.o ../../src/membuf.o

SOURCES = ../main.os ../exodecrunch.os
NAME = pfp_1.prg

.PHONY: build %.test
.PRECIOUS: %.exo $(SOURCES) $(TEST_OBJS)
.INTERMEDIATE: %_data.os %_data.s

build: data.test

../testrun: $(TEST_OBJS)
	@echo "Linking $@"
	@$(CC) $(LDFLAGS) -o $@ $(TEST_OBJS)

%.test: %.prg.out
	cmp $< $*.raw

%.prg.out: %.prg ../testrun
	../testrun $<

%.prg: %_data.os $(MAKEFILE) $(SOURCES)
	ld65 $(SOURCES) $< -o $@ -C../c64.cfg

%_data.s: %.exo data.s.template
	sed 's/PLACEHOLDER/$</' data.s.template > $@

%_data.os: %_data.s %.exo
	ca65 $< -o $@

%.os: %.s
	ca65 $< -o $@

clean:
	$(RM) data.test *.out *.exo *.prg

%.exo: %.raw
	../../src/exomizer mem -B $<@0x1000 -o $@

%.o:	%.c
	@echo "Compiling $<"
	@$(CC) -c $(CFLAGS) $(CPPFLAGS) -o $@ $<

%:	%.o
	@$(CC) $(LDFLAGS) $< -o $@
