
WFLAGS = -Wall -Wstrict-prototypes
CFLAGS = $(WFLAGS) -O3 -ffast-math -fomit-frame-pointer -fgcse -pedantic
LDFLAGS = -s

#CFLAGS = -g -mtune=i686 $(WFLAGS)
#LDFLAGS = -g -mtune=i686

TEST_OBJS = testrun.o ../src/6502emu.o ../src/exo_util.o ../src/log.o

SOURCES = main.os exodecrunch.os data.os
SOURCES256 = main.os exodecrunch.os256 data256.os
SOURCESc = main.os exodecrunch.osc datac.os
SOURCES256c = main.os exodecrunch.os256c data256c.os
SOURCESF = mainf.os krilldecr.os dataf.os
SOURCES1 = main1.os exostreamdecr1.os streamdata.os
SOURCES2 = main2.os exostreamdecr2.os streamdata.os
NAME = test.prg
NAME256 = test256.prg
NAMEc = testc.prg
NAME256c = test256c.prg
NAMEF = testf.prg
NAME1 = test1.prg
NAME2 = test2.prg


build: $(NAME) $(NAMEF) $(NAME1) $(NAME2)

testrun: $(TEST_OBJS)
	@echo "Linking $@"
	@$(CC) $(LDFLAGS) -o $@ $(TEST_OBJS)

$(NAME): $(MAKEFILE) testrun $(SOURCES)
	ld65 $(SOURCES) -o $@ -Cc64.cfg
	./testrun $@ data.bin,0x3000

$(NAME256): $(MAKEFILE) testrun $(SOURCES256)
	ld65 $(SOURCES256) -o $@ -Cc64.cfg
	./testrun $@ data.bin,0x3000

$(NAMEc): $(MAKEFILE) testrun $(SOURCESc)
	ld65 $(SOURCESc) -o $@ -Cc64.cfg
	./testrun $@ data.bin,0x3000

$(NAME256c): $(MAKEFILE) testrun $(SOURCES256c)
	ld65 $(SOURCES256c) -o $@ -Cc64.cfg
	./testrun $@ data.bin,0x3000

$(NAMEF): $(MAKEFILE) testrun $(SOURCESF)
	ld65 $(SOURCESF) -o $@ -Cc64.cfg
	./testrun $@ data.bin,0x3000

$(NAME1): $(MAKEFILE) testrun $(SOURCES1)
	ld65 $(SOURCES1) -o $@ -Cc64.cfg
	./testrun $@ data.bin,0xEA00

$(NAME2): $(MAKEFILE) testrun $(SOURCES2)
	ld65 $(SOURCES2) -o $@ -Cc64.cfg
	./testrun $@ data.bin,0xEA00

%.os: %.s
	ca65 $< -o $@

%.os256c: %.s
	ca65 -DLITERAL_SEQUENCES_NOT_USED=1 -DMAX_SEQUENCE_LENGTH_256=1 $< -o $@

%.os256: %.s
	ca65 -DMAX_SEQUENCE_LENGTH_256=1 $< -o $@

%.osc: %.s
	ca65 -DLITERAL_SEQUENCES_NOT_USED=1 $< -o $@

clean:
	$(RM) $(TEST_OBJS) $(SOURCES) $(SOURCES256) $(SOURCESc) $(SOURCES256c) $(SOURCESF) $(SOURCES1) $(SOURCES2) $(NAME) $(NAME256) $(NAMEc) $(NAME256c) $(NAMEF) $(NAME1) $(NAME2) testrun data.exo data256.exo datac.exo data256c.exo dataf.exo \
	streamdata.exo

data.os: data.exo
data256.os: data256.exo
datac.os: datac.exo
data256c.os: data256c.exo
dataf.os: dataf.exo
streamdata.os: streamdata.exo

data.exo: data.bin
	../src/exomizer mem -q -C data.bin,0x3000 -o data.exo

data256.exo: data.bin
	../src/exomizer mem -q -M256 -C data.bin,0x3000 -o data256.exo

datac.exo: data.bin
	../src/exomizer mem -q -c -C data.bin,0x3000 -o datac.exo

data256c.exo: data.bin
	../src/exomizer mem -q -M256 -c -C data.bin,0x3000 -o data256c.exo

dataf.exo: data.bin
	../src/exomizer mem -q -C -S1 -f data.bin,0x3000 -o dataf.exo

streamdata.exo: data.bin
	../src/exomizer raw -q -C -S1 -b -c -m 1024 data.bin -o streamdata.exo

%.o:	%.c
	@echo "Compiling $<"
	@$(CC) -c $(CFLAGS) $(CPPFLAGS) -o $@ $<

%:	%.o
	@$(CC) $(LDFLAGS) $< -o $@
