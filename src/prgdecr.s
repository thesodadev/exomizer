; -------------------------------------------------------------------
; -- i_start_addr, done
; -- i_safety_addr, done
; -- i_highest_addr, done
; -- i_ram_on_exit, done /* defined if true, otherwise not */
; -- i_config_effect, done /* defined if true, 0=lower right, 1=border flash */
; -------------------------------------------------------------------
; -- i_basic_start, done
; -- i_basic_picky, done
; -- i_table_addr, done
; -- i_bank_mode, done
; -- i_bank_addr, done
; -- i_bank_ram, done
; -- i_bank_rom, done
; -- i_end_of_mem_ram, done
; -- i_end_of_mem_rom, done
; -- i_effect_char, done
; -- i_effect_color, done
; -- i_effect_border, done
; -------------------------------------------------------------------
	.IF(!.DEFINED(i_start_addr))
	  .ERROR("Required symbol i_start_addr not defined.")
	.ENDIF
	i_safety_addr = .INCWORD("crunched_data", 0)
	.IF(!.DEFINED(i_safety_addr))
	  .ERROR("Required symbol i_safety_addr not defined.")
	.ENDIF
	i_highest_addr = .INCWORD("crunched_data", -2)
	.IF(!.DEFINED(i_highest_addr))
	  .ERROR("Required symbol i_highest_addr not defined.")
	.ENDIF

	.IF(i_bank_mode == 2 && .DEFINED(i_config_effect) &&
	    i_effect_color > i_table_addr + 156)
start_of_decrunchable_mem = i_effect_color
	.ELSE
start_of_decrunchable_mem = i_table_addr + 156
	.ENDIF

	.IF((i_safety_addr < start_of_decrunchable_mem ||
	     i_highest_addr > i_end_of_mem_ram) &&
	    (i_bank_mode != 0 || i_safety_addr < i_bank_ram ||
	     i_highest_addr > i_bank_rom))
	  .ERROR("This target can't support the memory demands of the data.")
	.ENDIF

	.IF(i_highest_addr > i_end_of_mem_rom)
config_ram_while_decrunch = 1
	.ENDIF
; -------------------------------------------------------------------
; -- The decrunch effect macro definition ---------------------------
; -------------------------------------------------------------------
.MACRO("effect")
  .IF(i_config_effect == 0)
    .IF(i_effect_color == $0be7)
	lda <$fd,x
	sta i_effect_color
    .ELSE
	stx i_effect_color
    .ENDIF
  .ELSE
    .IF(i_effect_border == $900f)
	txa
	ora #$18
	sta i_effect_border
    .ELIF(i_effect_border == $ff19)
	lda <$fd,x
	sta i_effect_border
    .ELSE
	stx i_effect_border
    .ENDIF
  .ENDIF
.ENDMACRO

.IF(.DEFINED(config_ram_while_decrunch))
  .IF(i_bank_mode == 3)
; -------------------------------------------------------------------
; -- The bank switch definitions for mode 3 (c64 & c128) ------------
; -------------------------------------------------------------------
    .MACRO("b2d")
	sei
      .IF(i_bank_ram == $38 && i_bank_addr == $01)
	inc <$01
      .ELSE
	lda #i_bank_ram
	sta i_bank_addr
      .ENDIF
    .ENDMACRO
    .MACRO("d2io")
      .IF(i_bank_rom == $37 && i_bank_ram == $38 && i_bank_addr == $01)
	dec <$01
      .ELSE
	lda #i_bank_rom
	sta i_bank_addr
      .ENDIF
    .ENDMACRO
    .MACRO("d2r")
      .IF(!.DEFINED(i_ram_on_exit))
        .IF(i_bank_rom == $37 && i_bank_ram == $38 && i_bank_addr == $01)
	dec <$01
	.ELSE
	lda #i_bank_rom
	sta i_bank_addr
	.ENDIF
	cli
      .ENDIF
    .ENDMACRO
    .MACRO("io2d")
      .IF(i_bank_rom == $37 && i_bank_ram == $38 && i_bank_addr == $01)
	inc <$01
      .ELSE
	lda #i_bank_ram
	sta i_bank_addr
      .ENDIF
    .ENDMACRO
  .ELIF(i_bank_mode == 2)
; -------------------------------------------------------------------
; -- The bank switch definitions for mode 2 (c16/+4) ----------------
; -------------------------------------------------------------------
    .MACRO("b2d")
	sta i_bank_ram
    .ENDMACRO
    .MACRO("d2io")
    .ENDMACRO
    .MACRO("d2r")
      .IF(!.DEFINED(i_ram_on_exit))
	sta i_bank_rom
      .ENDIF
    .ENDMACRO
    .MACRO("io2d")
    .ENDMACRO
  .ELSE
; -------------------------------------------------------------------
; -- The bank switch definitions for mode 1 & 2 (c20) ---------------
; -------------------------------------------------------------------
    .MACRO("b2d")
    .ENDMACRO
    .MACRO("d2io")
    .ENDMACRO
    .MACRO("d2r")
    .ENDMACRO
    .MACRO("io2d")
    .ENDMACRO
  .ENDIF
.ELSE
  .MACRO("b2d")
  .ENDMACRO
  .MACRO("d2io")
  .ENDMACRO
  .MACRO("d2r")
    .IF(.DEFINED(i_ram_on_exit))
      .IF(i_bank_mode == 3)
	sei
        .IF(i_bank_ram == $38 && i_bank_addr == $01)
	inc <$01
        .ELSE
	lda #i_bank_ram
	sta i_bank_addr
        .ENDIF
      .ELIF(i_bank_mode == 2)
	sta i_bank_ram
      .ELSE
      .ENDIF
    .ENDIF
  .ENDMACRO
  .MACRO("io2d")
  .ENDMACRO
.ENDIF
; -------------------------------------------------------------------
; -- End of bank switch definitions ---------------------------------
; -------------------------------------------------------------------

zp_len_lo = $a7

zp_src_lo  = $ae
zp_src_hi  = zp_src_lo + 1

zp_bits_lo = $fb
zp_bits_hi = zp_bits_lo + 1

zp_bitbuf  = $fd
zp_dest_lo = zp_bitbuf + 1	; dest addr lo
zp_dest_hi = zp_bitbuf + 2	; dest addr hi

	.ORG(i_basic_start - 2)
stage1start:
	.WORD(i_basic_start)
	.WORD(basic_end, 2005)
	.BYTE($9e, decr_start / 1000 % 10 + 48, decr_start / 100 % 10 + 48)
	.BYTE(decr_start / 10 % 10 + 48, decr_start % 10 + 48, 0)
basic_end:
; -------------------------------------------------------------------
	.IF(i_basic_picky == 1)
	  .BYTE(0, 0)
	.ENDIF
decr_start:
	ldy #$00
	.INCLUDE("b2d")
	tsx
cploop:
	lda file2end - 4,x
	sta $0100 - 4,x
	dex
	bne cploop
	jmp stage2start
stage1end:
; -------------------------------------------------------------------
; -- end of stage 1 -------------------------------------------------
; -------------------------------------------------------------------
; -------------------------------------------------------------------
; -- start of file part 2 -------------------------------------------
; -------------------------------------------------------------------
file2start:
	.IF(i_safety_addr > file2start)
transfer_len = 0
lowest_addr = file2start
	.ELIF(.DEFINED(i_config_effect) &&
	      i_config_effect == 0 &&
	      i_effect_char < i_safety_addr &&
	      (file2start - i_effect_char < 257 ||
	       file2start - i_safety_addr > 256))
transfer_len = file2start - i_effect_char
lowest_addr = i_effect_char
	.ELSE
transfer_len = file2start - i_safety_addr
lowest_addr = i_safety_addr
	.ENDIF
	.INCBIN("crunched_data", transfer_len + 2, -2)
	.WORD(i_highest_addr)
file2end:
; -------------------------------------------------------------------
; -- end of file part 2 ---------------------------------------------
; -------------------------------------------------------------------
	.ORG($0100)
; -------------------------------------------------------------------
; -- start of stage 3 -----------------------------------------------
; -------------------------------------------------------------------
stage3start:
; -------------------------------------------------------------------
; get bits (29 bytes)
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
get_bits:
	lda #$00
	sta <zp_bits_hi
	cpx #$01
	bcc bits_done
bits_next:
	lsr <zp_bitbuf
	bne ok
	pha
literal_get_byte:
	lda get_byte_fixup + 1
	bne get_byte_skip_hi
	dec get_byte_fixup + 2
	.IF(.DEFINED(i_config_effect))
	  .IF(i_config_effect == 1)
get_byte_skip_hi:
	  .ENDIF
	  .INCLUDE("d2io")
	  .INCLUDE("effect")
	  .INCLUDE("io2d")
	  .IF(i_config_effect != 1)
get_byte_skip_hi:
	  .ENDIF
	.ELSE
get_byte_skip_hi:
	.ENDIF
	dec get_byte_fixup + 1
get_byte_fixup:
	lda file2end - 3
	bcc literal_byte_gotten
	ror
	sta <zp_bitbuf
	pla
ok:
	rol
	rol <zp_bits_hi
	dex
	bne bits_next
bits_done:
	rts
; -------------------------------------------------------------------
; main copy loop (16 bytes)
;
copy_next_hi:
	dex
	dec <zp_dest_hi
	dec <zp_src_hi
copy_next:
	dey
	lda (zp_src_lo),y
literal_byte_gotten:
	sta (zp_dest_lo),y
copy_start:
	tya
	bne copy_next
	txa
	bne copy_next_hi
; -------------------------------------------------------------------
; decruncher entry point, needs calculated tables (15 bytes)
; x and y must be #0 when entering
;
begin:
	dey
begin2:
	inx
	jsr bits_next
	lsr
	iny
	bcc begin2
	beq literal_start
	cpy #$11
	bcs decr_exit
; -------------------------------------------------------------------
; calulate length of sequence (zp_len) (11 bytes)
;
	ldx tabl_bi - 1,y
	jsr get_bits
	adc tabl_lo - 1,y	; we have now calculated zp_len_lo
	sta <zp_len_lo
; -------------------------------------------------------------------
; now do the hibyte of the sequence length calculation (6 bytes)
	lda <zp_bits_hi
	adc tabl_hi - 1,y	; c = 0 after this.
	pha
; -------------------------------------------------------------------
; here we decide what offset table to use (20 bytes)
; x is 0 here
;
	bne nots123
	ldy <zp_len_lo
	cpy #$04
	bcc size123
nots123:
	ldy #$03
size123:
	ldx tabl_bit - 1,y
	jsr get_bits
	adc tabl_off - 1,y	; c = 0 after this.
	tay			; 1 <= y <= 50 here
; -------------------------------------------------------------------
; Here we do the dest_lo -= len_lo subtraction to prepare zp_dest
; but we do it backwards:	a - b == (b - a - 1) ^ ~0 (C-syntax)
; (14 bytes)
	lda <zp_len_lo
literal_start:			; literal enters here with y = 0, c = 1
	sbc <zp_dest_lo
	bcc noborrow
	dec <zp_dest_hi
noborrow:
	eor #$ff
	sta <zp_dest_lo
	cpy #$01		; y < 1 then literal
	bcc literal_get_byte
; -------------------------------------------------------------------
; calulate absolute offset (zp_src) (27 bytes)
;
	ldx tabl_bi,y
	jsr get_bits;
	adc tabl_lo,y
	bcc skipcarry
	inc <zp_bits_hi
	clc
skipcarry:
	adc <zp_dest_lo
	sta <zp_src_lo
	lda <zp_bits_hi
	adc tabl_hi,y
	adc <zp_dest_hi
	sta <zp_src_hi
; -------------------------------------------------------------------
; prepare for copy loop (6 bytes)
;
	pla
	tax
	ldy <zp_len_lo
	bcc copy_start
decr_exit:
	.INCLUDE("d2r")
	jmp i_start_addr
; -------------------------------------------------------------------
; two small static tables (6 bytes)
;
tabl_bit:
	.BYTE(2, 4, 4)
tabl_off:
	.BYTE(48, 32, 16)
stage3end:
; -------------------------------------------------------------------
; -- end of stage 3 -------------------------------------------------
; -------------------------------------------------------------------
	.ORG(file2end + stage3end - stage3start)
; -------------------------------------------------------------------
; -- start of file part 1 -------------------------------------------
; -------------------------------------------------------------------
file1start:
	.INCBIN("crunched_data", 2, transfer_len)
file1end:
; -------------------------------------------------------------------
; -- end of file part 1 ---------------------------------------------
; -------------------------------------------------------------------
; -------------------------------------------------------------------
; -- start of stage 2 -----------------------------------------------
; -------------------------------------------------------------------
tabl_bi = i_table_addr
tabl_lo = i_table_addr + 52
tabl_hi = i_table_addr + 104
stage2start:
	inx
	tya
	and #$0f
	beq shortcut		; starta på ny sekvens

	txa			; this clears reg a
	lsr			; and sets the carry flag
	ldx tabl_bi-1,y
rolle:
	rol
	rol <zp_bits_hi
	dex
	bpl rolle		; c = 0 after this (rol zp_bits_hi)

	adc tabl_lo-1,y
	tax

	lda <zp_bits_hi
	adc tabl_hi-1,y
shortcut:
	sta tabl_hi,y
	txa
	sta tabl_lo,y

	ldx #4
	jsr get_bits		; clears x-reg.
	sta tabl_bi,y
	iny
	cpy #52
	bne stage2start
	.IF(transfer_len == 0)
	ldy #0
	.ELSE
	  .IF(transfer_len < 257)
	ldy #transfer_len % 256
copy1_loop:
	    .IF(transfer_len == 256)
	lda file1start,y
	sta i_safety_addr,y
	    .ELSE
	lda file1start - 1,y
	sta i_safety_addr - 1,y
	    .ENDIF
	dey
	bne copy1_loop
	  .ELSE
	ldx #transfer_len / 256
	ldy #transfer_len % 256
	bcs copy2_start
copy2_loop2:
	dex
	dec lda_fixup + 2
	dec sta_fixup + 2
copy2_loop1:
	dey
lda_fixup:
	lda file1start + transfer_len / 256 * 256,y
sta_fixup:
	sta i_safety_addr + transfer_len / 256 * 256,y
copy2_start:
	tya
	bne copy2_loop1
	txa
	bne copy2_loop2
	  .ENDIF
	.ENDIF
	.IF(.DEFINED(i_config_effect) && i_config_effect == 0)
	  .IF(i_effect_char < lowest_addr || i_effect_char > i_highest_addr)
	sta i_effect_char
	  .ENDIF
	.ENDIF
	jmp begin
stage2end:
; -------------------------------------------------------------------
; -- end of stage 2 -------------------------------------------------
; -------------------------------------------------------------------
