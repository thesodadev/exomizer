
SOURCES = main.os exodecrunch.os data.os
NAME = test.prg


build: $(NAME) $(NAME1) $(NAME2)

$(NAME): $(MAKEFILE) $(SOURCES)
	ld65 -t c64 $(SOURCES) -o $@

%.os: %.s
	ca65 $< -o $@

clean:
	$(RM) $(SOURCES) $(NAME) data.exo

data.os: data.exo

data.exo: data.bin
	../exomizer mem -lnone data.bin,0x3000 -o data.exo
