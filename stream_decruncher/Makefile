
SOURCES1 = main1.os exostreamdecr1.os data.os
SOURCES2 = main2.os exostreamdecr2.os data.os
NAME1 = test1.prg
NAME2 = test2.prg


build: $(NAME1) $(NAME2)

$(NAME1): $(MAKEFILE) $(SOURCES1)
	ld65 -t c64 $(SOURCES1) -o $@

$(NAME2): $(MAKEFILE) $(SOURCES2)
	ld65 -t c64 $(SOURCES2) -o $@

%.os: %.s
	ca65 $< -o $@

clean:
	$(RM) $(SOURCES1) $(SOURCES2) $(NAME1) $(NAME2) data.exo

data.os: data.exo

data.exo: data.bin
	exomizer -m 1024 data.bin -o data.exo
