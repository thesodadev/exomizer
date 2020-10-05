This directory contains two exomizer decrunchers in assembly for 32bit
ARM contributed by ALeX Kazik (alex@kazik.de). They are useful for
embedded systems like stm32f411.

The difference between the two files, universal.S and speed.S, is that
one is shorter and the other is faster. They use only the stack and no
global variables and can be optionally compiled with checks for
input/output buffer size and/or in/out pointer overrun detection. They
also suports all combinations of exomizer protocol flags as compile
options.

To compile using gcc you can use the following command line:
> arm-none-eabi-gcc -c speed.S -mcpu=cortex-m4

(cortext-m4 or better with thumb2 instructions is required)

In case you wonder why in and out is switched to the C parameter
list: the out pointer is passed on in r0 and never leaves - so the
result (r0) is the new out pointer.

Here follows some benchmark numbers from an NUCLEO-64 board with a
STM32F411 on it.  These numbers where measured decrunching test3.bin
crunched using -P7 (no offset reuse) and was done using 0 waitstates
and without caching.

exodecr.c, -Os:
468 bytes, 7993103 cycles

exodecr.c, -O3:
1700 bytes, 3675799 cycles

universal.S:
252 bytes, 2647856 cycles

speed.S:
256 bytes, 2537281 cycles
