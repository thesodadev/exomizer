;
; Copyright (c) 2002 Magnus Lind.
;
; This software is provided 'as-is', without any express or implied warranty.
; In no event will the authors be held liable for any damages arising from
; the use of this software.
;
; Permission is granted to anyone to use this software for any purpose,
; including commercial applications, and to alter it and redistribute it
; freely, subject to the following restrictions:
;
;   1. The origin of this software must not be misrepresented; you must not
;   claim that you wrote the original software. If you use this software in a
;   product, an acknowledgment in the product documentation would be
;   appreciated but is not required.
;
;   2. Altered source versions must be plainly marked as such, and must not
;   be misrepresented as being the original software.
;
;   3. This notice may not be removed or altered from any distribution.
;
;   4. The names of this software and/or it's copyright holders may not be
;   used to endorse or promote products derived from this software without
;   specific prior written permission.
;
; exodecruncher.s, a part of the exomizer v1.0beta3 release
;
; -------------------------------------------------------------------
; zero page adresses used
; -------------------------------------------------------------------
zp_len_lo = $a7

zp_src_lo  = $ae
zp_src_hi  = zp_src_lo + 1

zp_bits_lo = $fb
zp_bits_hi = zp_bits_lo + 1

zp_bitbuf  = $fd
zp_dest_lo  = zp_bitbuf + 1		; dest addr lo
zp_dest_hi  = zp_bitbuf + 2		; dest addr hi

; -------------------------------------------------------------------
; Here an example of how to call the decruncher.
; -------------------------------------------------------------------
	sei
	inc $01			; assumes that $01 is #$37
	jsr decrunch_file
	dec $01
	cli
	rts
; -------------------------------------------------------------------
; this is an example implementation of the get_byte routine.
; You may implement this yourselves to read bytes from any datasource.
; The get_byte routine must not modify x-reg, y-reg, carry-flag.
; -------------------------------------------------------------------	
get_byte:
	lda byte_lo
	bne byte_skip_hi
	dec $01			; assumes that $01 is #$38
 	inc $d020		; lame depack effect here
	inc $01
	dec byte_hi
byte_skip_hi:
	dec byte_lo
byte_lo = * + 1
byte_hi = * + 2
	lda $FFFF		; needs to be set correctly before
	rts		; before decrunch_file is called.
; -------------------------------------------------------------------
; no code below this comment has to be modified in order to generate
; a working decruncher of this source file.
; However, you may want to relocate the tables last in the file to a
; more suitable adress.	
; -------------------------------------------------------------------	
decrunch_file:	
	ldy #0
	ldx #3
init_zp:
	jsr get_byte
	sta zp_bitbuf - 1,x
	dex
	bne init_zp

nextone:
	inx
	tya
	and #$0f
	beq shortcut		; starta på ny sekvens

	lda zp_bits_lo
	adc tabl_lo-1,y
	tax
	
	lda zp_bits_hi
	adc tabl_hi-1,y
shortcut:
	sta tabl_hi,y
	txa
	sta tabl_lo,y

	ldx #4
	jsr get_bits		; clears x-reg.
	sta tabl_bi,y

	stx zp_bits_lo		; zp_bits_hi is already 0 because
	tax			; we called get_bits with x = 4
	inx
	sec
rolle:	
	rol zp_bits_lo
	rol zp_bits_hi
	dex 
	bne rolle		; c = 0 after this
skip:	
	iny
	cpy #52
	bne nextone
	ldy #0
	beq begin	
; -------------------------------------------------------------------
; get bits (30 bytes)
;
; args:
;   x = number of bits to get
; returns:
;   a = #bits_lo
;   x = #0
;   c = 0
;   zp_bits_lo = #bits_lo
;   zp_bits_hi = #bits_hi
; notes:
;   y is untouched
;   other status bits are set to (a == #0)
; -------------------------------------------------------------------
get_bit1:
	inx
get_bits:
	lda #$00
	sta zp_bits_lo
	sta zp_bits_hi
	cpx #$01
	bcc bits_done
	lda zp_bitbuf
bits_next:	
	lsr a
	bne ok
	jsr get_byte
	ror a
ok:
	rol zp_bits_lo
	rol zp_bits_hi
	dex
	bne bits_next
	sta zp_bitbuf
	lda zp_bits_lo
bits_done:	
	rts
; -------------------------------------------------------------------
copy_next_hi:
	dex
	dec zp_dest_hi	
	dec zp_src_hi
copy_next:
	dey
	lda (zp_src_lo),y
literal_entry:	
	sta (zp_dest_lo),y
copy_start:
	tya
	bne copy_next
	txa
	bne copy_next_hi
; -------------------------------------------------------------------
; we start here (7 bytes)
; x and y must be #0 when entering
begin:
	jsr get_bit1
	beq sequence
; -------------------------------------------------------------------
; literal handling (8 bytes)
literal_start:	
	lda zp_dest_lo
	bne avoid_hi
	dec zp_dest_hi	
avoid_hi:	
	dec zp_dest_lo
	jsr get_byte
	bcc literal_entry
; -------------------------------------------------------------------
; get gamma code  (8 bytes)
; y = x = 0 when entering
sequence:	
next1:
	iny
	jsr get_bit1
	beq next1
	cpy #$11
	bcs bits_done
; -------------------------------------------------------------------
; calulate length of sequence (zp_len) (19 bytes)
;
	ldx tabl_bi - 1,y
	jsr get_bits
	adc tabl_lo - 1,y
	sta zp_len_lo
	tax
	lda zp_bits_hi
	adc tabl_hi - 1,y
	pha
; -------------------------------------------------------------------
; here we decide what offset table to use (18 bytes)
; x is 0 here
	bne nots123
	cpx #$04
	bcc size123
nots123:	
	ldx #$03
size123:	
	txa
	tay
	
	ldx tabl_bit - 1,y
	jsr get_bits
	adc tabl_off - 1,y
	tay
; -------------------------------------------------------------------
; prepare zp_dest (11 bytes)
;
	sec
	lda zp_dest_lo
	sbc zp_len_lo
	sta zp_dest_lo
	bcs noborrow
	dec zp_dest_hi
noborrow:

; -------------------------------------------------------------------
; calulate absolute offset (zp_src) (27 bytes)
;
	ldx tabl_bi,y
	jsr get_bits;
	adc tabl_lo,y
	bcc skipcarry
	inc zp_bits_hi
	clc
skipcarry:
	adc zp_dest_lo
	sta zp_src_lo
	lda zp_bits_hi
	adc tabl_hi,y
	adc zp_dest_hi
	sta zp_src_hi
;
	ldy zp_len_lo
	pla
	tax
	bcc copy_start
tabl_bit:
	.byte 2,4,4
tabl_off:
	.byte 48,32,16
; -------------------------------------------------------------------
; end of decruncher
; -------------------------------------------------------------------

; -------------------------------------------------------------------
; this table area may be relocated and clobbered between decrunches.
; the ordering between the tabl_x lables must not be changed and
; they must be adjacent in memory to each other like they are here.
; -------------------------------------------------------------------
tabl_bi:
	.byte 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	.byte 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	.byte 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	.byte 0,0,0,0
tabl_lo:
	.byte 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	.byte 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	.byte 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	.byte 0,0,0,0	
tabl_hi:
	.byte 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	.byte 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	.byte 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	.byte 0,0,0,0
; -------------------------------------------------------------------
; end of decruncher
; -------------------------------------------------------------------
