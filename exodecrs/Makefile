
SOURCES = main.os exodecrunch.os data.os
SOURCES1 = main1.os exostreamdecr1.os streamdata.os
SOURCES2 = main2.os exostreamdecr2.os streamdata.os
NAME = test.prg
NAME1 = test1.prg
NAME2 = test2.prg


build: $(NAME) $(NAME1) $(NAME2)

$(NAME): $(MAKEFILE) $(SOURCES)
	ld65 -t c64 $(SOURCES) -o $@

$(NAME1): $(MAKEFILE) $(SOURCES1)
	ld65 -t c64 $(SOURCES1) -o $@

$(NAME2): $(MAKEFILE) $(SOURCES2)
	ld65 -t c64 $(SOURCES2) -o $@

%.os: %.s
	ca65 $< -o $@

clean:
	$(RM) $(SOURCES) $(SOURCES1) $(SOURCES2) $(NAME) $(NAME1) $(NAME2) data.exo streamdata.exo

data.os: data.exo
streamdata.os: streamdata.exo

data.exo: data.bin
	../src/exomizer mem -m 1024 data.bin,0x3000 -o data.exo

streamdata.exo: data.bin
	../src/exoraw -b -c -m 1024 data.bin -o streamdata.exo
