; -------------------------------------------------------------------
; zero page adresses used
; -------------------------------------------------------------------
zp_src_lo  = $ae
zp_src_hi  = $af

zp_bits_lo = $fa
zp_bits_hi = $fb

zp_bitbuf  = $fc
	
zp_dest_lo  = $fd		; dest addr lo
zp_dest_hi  = $fe		; dest addr hi

zp_len_lo = $ff

; -------------------------------------------------------------------
; start here, but first make sure that byte_lo and byte_hi in the
; 'get_byte' routine is initialized properly.
; -------------------------------------------------------------------
	sei
	inc $01			; assumes that $01 is #$37
	ldx #0
	ldy #0
	jsr decrunch_file
	dec $01
	cli
	rts
; -------------------------------------------------------------------
; get byte, must not modify x-reg, y-reg, carry-flag.
; may be exchanged for a routine that reads a byte from disc.
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
	lda $FFFF		; needs to be set correctly
	rts
; -------------------------------------------------------------------
; no code below this comment has to be modified in order to generate
; a working decruncher of this source file.
; However, you may want to relocate the tables last in the file to a
; more suitable adress.	
; -------------------------------------------------------------------	
decrunch_file:	
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
	bcs begin
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
; -------------------------------------------------------------------
; calctable x = 0, y = 0;
; -------------------------------------------------------------------
copy2zp:
nextzp:	
	jsr get_byte
	sta zp_bitbuf - 1,x
	dex
	bne nextzp
	rts
; -------------------------------------------------------------------
; these tables may be relocated
; -------------------------------------------------------------------
tabl_bit:
	.byte 2,4,4
tabl_off:
	.byte 48,32,16
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
