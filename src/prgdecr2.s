; -------------------------------------------------------------------
; -- i_start_addr, done /* required */
; -- i_target, done /* required */
; -- i_ram_on_exit, done /* defined if true, otherwise not */
; -- i_irq_on_exit, done /* undef=auto, 1=sei, 0=cli */
; -- i_literal_sequences_used, done /* defined if true, otherwise not */
; -- i_config_effect, done /* -1=none, undef||0=lower right, 1=border flash */
; -- i_fast_effect, done /* defined if true, otherwise not */
; -- i_table_addr, done /* undef=$0334 or if(i_target == 128) $0b00 */
; -------------------------------------------------------------------
.IF(!.DEFINED(i_target))
  .ERROR("Required symbol i_target_addr not defined.")
.ENDIF
.IF(!.DEFINED(i_config_effect))
  i_config_effect = 0
.ENDIF

.IF(.DEFINED(i_irq_on_exit) && i_irq_on_exit != 0 && i_irq_on_exit != 1)
  .ERROR("Symbol i_irq_on_exit must be undefined, 0 or 1.")
.ENDIF

.IF(i_target == 20)
  i_basic_start    = $1001
  i_end_of_mem_ram = $2000
  i_end_of_mem_rom = $2000
  i_effect_char    = $1ff9
  i_effect_color   = $97f9
  i_rom_config_value = 0
  i_ram_config_value = 0
.ELIF(i_target == 23)
  i_basic_start    = $0401
  i_end_of_mem_ram = $2000
  i_end_of_mem_rom = $2000
  i_effect_char    = $1ff9
  i_effect_color   = $97f9
  i_rom_config_value = 0
  i_ram_config_value = 0
.ELIF(i_target == 52)
  i_basic_start    = $1201
  i_end_of_mem_ram = $8000
  i_end_of_mem_rom = $8000
  i_effect_char    = $11f9
  i_effect_color   = $95f9
  i_rom_config_value = 0
  i_ram_config_value = 0
.ELIF(i_target == 55)
  i_basic_start    = $1201
  i_end_of_mem_ram = $8000
  i_end_of_mem_rom = $8000
  i_effect_char    = $11f9
  i_effect_color   = $95f9
  i_rom_config_value = 0
  i_ram_config_value = 0
.ELIF(i_target == 4)
  i_basic_start    = $1001
  i_end_of_mem_ram = $fd00
  i_end_of_mem_rom = $8000
  i_effect_char    = $0fe7
  i_effect_color   = $0be7
  i_rom_config_value = 0
  i_ram_config_value = 1
.ELIF(i_target == 64)
  i_basic_start    = $0801
  i_end_of_mem_ram = $10000
  i_end_of_mem_rom = $a000
  i_effect_char    = $07e7
  i_effect_color   = $dbe7
  i_rom_config_value = $37
  i_ram_config_value = $38
.ELIF(i_target == 128)
  i_basic_start    = $1c01
  i_end_of_mem_ram = $fff0
  i_end_of_mem_rom = $4000
  i_effect_char    = $07e7
  i_effect_color   = $dbe7
  i_rom_config_value = $00
  i_ram_config_value = $3f
.ELSE
  .ERROR("Symbol i_target_addr has an invalid value.")
.ENDIF

.IF(!.DEFINED(i_ram_on_exit))
  i_ram_on_exit = i_rom_config_value
.ENDIF

.IF(!.DEFINED(i_table_addr))
  .IF(i_target == 128)
i_table_addr = $0b00
  .ELSE
i_table_addr = $0334
  .ENDIF
.ENDIF

.IF(i_table_addr < $0200 || i_table_addr > 65536 - 156)
  .ERROR("Symbol i_table_addr must not be < $0200 or > $FF64.")
.ENDIF

.IF(i_table_addr < $0334 && !.DEFINED(i_irq_on_exit))
  i_irq_on_exit = 0
.ENDIF

.IF(!.DEFINED(i_start_addr))
  .ERROR("Required symbol i_start_addr not defined.")
.ENDIF

.IF(i_start_addr == 0 && i_ram_on_exit != i_rom_config_value))
  .ERROR("Basic start and non-ROM configuration can't be combined.")
.ENDIF

i_safety_addr = .INCWORD("crunched_data", 0)

; -------------------------------------------------------------------
; -- convert $0 to $10000 but leave $1 - $ffff ----------------------
; -------------------------------------------------------------------
i_highest_addr = (.INCWORD("crunched_data", -2) + 65535) % 65536 + 1

	.IF(i_target == 4 && i_config_effect == 0 &&
	    i_effect_color > i_table_addr + 156)
start_of_decrunchable_mem = i_effect_color
	.ELSE
start_of_decrunchable_mem = i_table_addr + 156
	.ENDIF

	.IF((i_safety_addr < start_of_decrunchable_mem ||
	     i_highest_addr > i_end_of_mem_ram) &&
	    ((i_target != 20 && i_target != 52) ||
	     i_safety_addr < i_bank_ram ||
	     i_highest_addr > i_bank_rom))
	  .ERROR("This target can't support the memory demands of the data.")
	.ENDIF

.IF(i_highest_addr > i_end_of_mem_rom || i_ram_on_exit == i_ram_config_value)
config_ram_while_decrunch = 1
.ENDIF
; -------------------------------------------------------------------
; -- The decruncher exit macro definition ---------------------------
; -------------------------------------------------------------------
.MACRO("run")
  .IF(i_start_addr == -2)
    .IF(i_target == 128)
      .IF(.DEFINED(i_basic_txt_start))
	lda #i_basic_txt_start % 256
	sta <$2d
	lda #i_basic_txt_start / 256
	sta <$2e
      .ENDIF
      .IF(.DEFINED(i_basic_var_start))
	lda #i_basic_var_start % 256
	sta $1210
	lda #i_basic_var_start / 256
	sta $1211
      .ENDIF
      .IF(.DEFINED(i_basic_highest_addr))
	lda #i_basic_highest_addr % 256
	sta $1212
	lda #i_basic_highest_addr / 256
	sta $1213
      .ENDIF
    .ELSE
      .IF(.DEFINED(i_basic_txt_start))
	lda #i_basic_txt_start % 256
	sta <$2b
	lda #i_basic_txt_start / 256
	sta <$2c
      .ENDIF
      .IF(.DEFINED(i_basic_var_start))
	lda #i_basic_var_start % 256
	sta <$2d
	lda #i_basic_var_start / 256
	sta <$2e
      .ENDIF
      .IF(.DEFINED(i_basic_highest_addr))
	lda #i_basic_highest_addr % 256
	sta <$37
	lda #i_basic_highest_addr / 256
	sta <$38
      .ENDIF
    .ENDIF
    .IF(i_target == 20 || i_target == 23 || i_target == 52 || i_target == 55)
	jsr $c659		; init
	jsr $c533		; regenerate line links and set $2d/$2e
	jmp $c7ae		; start
    .ELIF(i_target == 4)
	jsr $8bbe		; init
	jsr $8818		; regenerate line links and set $2d/$2e
	jsr $f3b5		; regen color table at $0113
	jmp $8bdc		; start
    .ELIF(i_target == 64)
	jsr $a659		; init
	jsr $a533		; regenerate line links and set $2d/$2e
	jmp $a7ae		; start
    .ELIF(i_target == 128)
	jsr $5ab5		; init
	jsr $4f4f		; regenerate line links and set $1210/$1211
	jmp $4af6		; start
    .ENDIF
  .ELSE
	jmp i_start_addr
  .ENDIF
.ENDMACRO
; -------------------------------------------------------------------
; -- The decrunch effect macro definition ---------------------------
; -------------------------------------------------------------------
.MACRO("effect")
  .IF(i_config_effect == -1)
  .ELIF(i_config_effect == 0)
    .IF(i_target == 4)
	lda <$fd,x
	sta i_effect_color
    .ELSE
	stx i_effect_color
    .ENDIF
  .ELIF(i_config_effect == 1)
    .IF(i_target == 20 || i_target == 23 || i_target == 52 || i_target == 55)
	and #$07
	ora #$18
	sta $900f
    .ELIF(i_target == 4)
	ora #$30
	sta $ff19
    .ELSE
	sta $d020
    .ENDIF
  .ELIF(i_config_effect == 2)
    .IF(i_target == 20 || i_target == 23 || i_target == 52 || i_target == 55)
	txa
	and #$07
	ora #$18
	sta $900f
    .ELIF(i_target == 4)
	lda <$fd,x
	sta $ff19
    .ELSE
	stx $d020
    .ENDIF
  .ELIF(i_config_effect == 3)
    .IF(i_target == 20 || i_target == 23 || i_target == 52 || i_target == 55)
	tya
	and #$07
	ora #$18
	sta $900f
    .ELIF(i_target == 4)
	sty $ff19
    .ELSE
	sty $d020
    .ENDIF
  .ELSE
    .ERROR("Unknown decrunch effect.")
  .ENDIF
.ENDMACRO

.IF(.DEFINED(config_ram_while_decrunch))
; -------------------------------------------------------------------
; -- The ram/rom switch macros for decrunching in ram mode ----------
; -------------------------------------------------------------------
  .IF(i_target == 64)
; -------------------------------------------------------------------
; -- The ram/rom switch macros for c64 ------------------------------
; -------------------------------------------------------------------
    .MACRO("b2d")
	sei
	inc <$01
    .ENDMACRO
    .MACRO("d2io")
	dec <$01
    .ENDMACRO
    .MACRO("d2r")
      .IF(i_ram_on_exit == i_rom_config_value)
	dec <$01
        .IF(!.DEFINED(i_irq_on_exit) || i_irq_on_exit == 1)
	cli
	.ENDIF
      .ELSE
	.IF(i_ram_on_exit != i_ram_config_value && i_ram_on_exit != $34)
	lda #i_ram_on_exit % 256
	sta <$01
	.ENDIF
        .IF(.DEFINED(i_irq_on_exit) && i_irq_on_exit == 1)
	cli
	.ENDIF
      .ENDIF
    .ENDMACRO
    .MACRO("io2d")
	inc <$01
    .ENDMACRO
  .ELIF(i_target == 128)
; -------------------------------------------------------------------
; -- The ram/rom switch macros for c128 -----------------------------
; -------------------------------------------------------------------
    .MACRO("b2d")
	sei
	lda #$3f
	sta $fff0
    .ENDMACRO
    .MACRO("d2io")
	lda #$00
	sta $fff0
    .ENDMACRO
    .MACRO("d2r")
      .IF(i_ram_on_exit == i_rom_config_value)
	lda #$00
	sta $fff0
        .IF(!.DEFINED(i_irq_on_exit) || i_irq_on_exit == 1)
	cli
	.ENDIF
      .ELSE
	.IF(i_ram_on_exit != i_ram_config_value)
	lda #i_ram_on_exit % 256
	sta $fff0
	.ENDIF
        .IF(.DEFINED(i_irq_on_exit) && i_irq_on_exit == 1)
	cli
	.ENDIF
      .ENDIF
    .ENDMACRO
    .MACRO("io2d")
	lda #$3f
	sta $fff0
    .ENDMACRO
  .ELIF(i_target == 4)
; -------------------------------------------------------------------
; -- The ram/rom switch macros for c16/+4 ---------------------------
; -------------------------------------------------------------------
    .MACRO("b2d")
	sei
	sta $ff3f
    .ENDMACRO
    .MACRO("d2io")
    .ENDMACRO
    .MACRO("d2r")
      .IF(i_ram_on_exit == i_rom_config_value)
	sta $ff3e
        .IF(!.DEFINED(i_irq_on_exit) || i_irq_on_exit == 1)
	cli
	.ENDIF
      .ELSE
        .IF(.DEFINED(i_irq_on_exit) && i_irq_on_exit == 1)
	cli
	.ENDIF
      .ENDIF
    .ENDMACRO
    .MACRO("io2d")
    .ENDMACRO
  .ELSE
; -------------------------------------------------------------------
; -- The ram/rom switch definitions for c20 -------------------------
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
; -------------------------------------------------------------------
; -- The ram/rom switch macros for decrunching in rom mode ----------
; -------------------------------------------------------------------
  .MACRO("b2d")
    .IF(.DEFINED(i_irq_on_exit) && i_irq_on_exit == 0)
	sei
    .ENDIF
  .ENDMACRO
  .MACRO("d2io")
  .ENDMACRO
  .MACRO("d2r")
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

zp_bits_hi = $fb

zp_bitbuf  = $fd
zp_dest_lo = zp_bitbuf + 1	; dest addr lo
zp_dest_hi = zp_bitbuf + 2	; dest addr hi

	.ORG(i_basic_start - 2)
stage1start:
	.WORD(i_basic_start)
	.WORD(basic_end, 20)
	.BYTE($9e, decr_start / 1000 % 10 + 48, decr_start / 100 % 10 + 48)
	.BYTE(decr_start / 10 % 10 + 48, decr_start % 10 + 48, 0)
basic_end:
; -------------------------------------------------------------------
	.IF(i_target == 4 || i_target == 128)
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
	.ELIF(i_config_effect == 0 &&
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
	.WORD(i_highest_addr % 65536)
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
;   z = 1
;   zp_bits_hi = #bits_hi
; notes:
;   y is untouched
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
	.IF(i_config_effect != -1 && .DEFINED(i_fast_effect))
	  .INCLUDE("d2io")
	  .INCLUDE("effect")
	  .INCLUDE("io2d")
	.ENDIF
	lda get_byte_fixup + 1
	bne get_byte_skip_hi
	dec get_byte_fixup + 2
	.IF(i_config_effect != -1 && !.DEFINED(i_fast_effect))
	  .INCLUDE("d2io")
	  .INCLUDE("effect")
	  .INCLUDE("io2d")
	.ENDIF
get_byte_skip_hi:
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
.IF(.DEFINED(i_literal_sequences_used))
	bcc literal_get_byte
.ENDIF
	lda (zp_src_lo),y
literal_byte_gotten:
	sta (zp_dest_lo),y
copy_start:
	tya
	bne copy_next
begin:
	txa
	bne copy_next_hi
; -------------------------------------------------------------------
; decruncher entry point, needs calculated tables (15 bytes)
; x and y must be #0 when entering
;
.IF(.DEFINED(i_literal_sequences_used))
	inx
	jsr get_bits
	tay
	bne literal_start1
.ELSE
	dey
.ENDIF
begin2:
	inx
	jsr bits_next
	lsr
	iny
	bcc begin2
.IF(!.DEFINED(i_literal_sequences_used))
	beq literal_start
.ENDIF
	cpy #$11
.IF(.DEFINED(i_literal_sequences_used))
	bcc sequence_start
	bne decr_exit
; -------------------------------------------------------------------
; literal sequence handling
;
	ldx #$10
	jsr get_bits
literal_start1:
	sta <zp_len_lo
	ldx <zp_bits_hi
	ldy #0
	bcc literal_start
sequence_start:
.ELSE
	bcs decr_exit
.ENDIF
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
	tay			; 1 <= y <= 52 here
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
.IF(.DEFINED(i_literal_sequences_used))
	bcc pre_copy
.ELSE
	bcc literal_get_byte
.ENDIF
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
.IF(.DEFINED(i_literal_sequences_used))
	sec
pre_copy:
	ldy <zp_len_lo
	jmp copy_start
.ELSE
	ldy <zp_len_lo
	bcc copy_start
.ENDIF
decr_exit:
	.INCLUDE("d2r")
	.INCLUDE("run")
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
	sta lowest_addr,y
	    .ELSE
	lda file1start - 1,y
	sta lowest_addr - 1,y
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
	.IF(i_config_effect == 0)
	  .IF(i_effect_char < lowest_addr || i_effect_char > i_highest_addr)
	sty i_effect_char
	  .ENDIF
	.ENDIF
	jmp begin
stage2end:
; -------------------------------------------------------------------
; -- end of stage 2 -------------------------------------------------
; -------------------------------------------------------------------
