;
; Copyright (c) 2002 - 2005 Magnus Lind.
;
; This software is provided 'as-is', without any express or implied warranty.
; In no event will the authors be held liable for any damages arising from
; the use of this software.
;
; Permission is granted to anyone to use this software, alter it and re-
; distribute it freely for any non-commercial, non-profit purpose subject to
; the following restrictions:
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
; -------------------------------------------------------------------
; -- r_start_addr, done /* required, -2=basic start */
; -- r_target, done /* required, 20, 23, 52, 55, 4, 64 or 128 */
; -- i_literal_sequences_used, done /* defined if true, otherwise not */
; -- i_ram_enter, done /* undef=c_rom_config_value */
; -- i_irq_enter, done /* undef=on, 0=off, 1=on */
; -- i_ram_during, done /* undef=auto
; -- i_irq_during, done /* undef=auto, 0=off, 1=on */
; -- i_ram_exit, done /* undef=auto
; -- i_irq_exit, done /* undef=auto, 0=off, 1=on */
; -- i_effect, done /* -1=none, 0(default)=lower right, 1-3=border */
; -- i_fast_effect, done /* defined if true, otherwise not */
; -- i_table_addr, done /* undef=$0334 or if(r_target == 128) $0b00 */
; -------------------------------------------------------------------
; - if basic start --------------------------------------------------
; -- i_basic_txt_start    /* will be set before basic start, if defined */
; -- i_basic_var_start    /* will be set before basic start, if defined */
; -- i_basic_highest_addr /* will be set before basic start, if defined */
; -------------------------------------------------------------------
.IF(!.DEFINED(r_target))
  .ERROR("Required symbol r_target not defined.")
.ENDIF
.IF(!.DEFINED(i_effect))
  i_effect = 0
.ENDIF

.IF(.DEFINED(i_irq_exit) && i_irq_exit != 0 && i_irq_exit != 1)
  .ERROR("Symbol i_irq_exit must be undefined, 0 or 1.")
.ENDIF

.IF(.DEFINED(i_irq_enter) && i_irq_enter != 0 && i_irq_enter != 1)
  .ERROR("Symbol i_irq_enter must be undefined, 0 or 1.")
.ENDIF

.IF(r_target == 20)
  c_basic_start    = $1001
  c_end_of_mem_ram = $2000
  c_end_of_mem_rom = $2000
  c_effect_char    = $1ff9
  c_effect_color   = $97f9
  c_border_color   = $900f
  c_rom_config_value = 0
  c_ram_config_value = 0
  c_default_table = $0334
.ELIF(r_target == 23)
  c_basic_start    = $0401
  c_end_of_mem_ram = $2000
  c_end_of_mem_rom = $2000
  c_effect_char    = $1ff9
  c_effect_color   = $97f9
  c_border_color   = $900f
  c_rom_config_value = 0
  c_ram_config_value = 0
  c_default_table = $0334
.ELIF(r_target == 52)
  c_basic_start    = $1201
  c_end_of_mem_ram = $8000
  c_end_of_mem_rom = $8000
  c_effect_char    = $11f9
  c_effect_color   = $95f9
  c_border_color   = $900f
  c_rom_config_value = 0
  c_ram_config_value = 0
  c_default_table = $0334
.ELIF(r_target == 55)
  c_basic_start    = $1201
  c_end_of_mem_ram = $8000
  c_end_of_mem_rom = $8000
  c_effect_char    = $11f9
  c_effect_color   = $95f9
  c_border_color   = $900f
  c_rom_config_value = 0
  c_ram_config_value = 0
  c_default_table = $0334
.ELIF(r_target == 4)
  c_basic_start    = $1001
  c_end_of_mem_ram = $fd00
  c_end_of_mem_rom = $8000
  c_effect_char    = $0fe7
  c_effect_color   = $0be7
  c_border_color   = $ff19
  c_rom_config_value = 0
  c_ram_config_value = 1
  c_default_table = $0334
.ELIF(r_target == 64)
  c_basic_start    = $0801
  c_end_of_mem_ram = $10000
  c_end_of_mem_rom = $a000
  c_effect_char    = $07e7
  c_effect_color   = $dbe7
  c_border_color   = $d020
  c_rom_config_value = $37
  c_ram_config_value = $38
  c_default_table = $0334
.ELIF(r_target == 128)
  c_basic_start    = $1c01
  c_end_of_mem_ram = $fff0
  c_end_of_mem_rom = $4000
  c_effect_char    = $07e7
  c_effect_color   = $dbe7
  c_border_color   = $d020
  c_rom_config_value = $00
  c_ram_config_value = $3f
  c_default_table = $0334
.ELIF(r_target == $a8)
  c_end_of_mem_ram = $d000
  c_end_of_mem_rom = $a000
  c_effect_color = $d017
  c_border_color   = $d01a
  c_rom_config_value = $fd
  c_ram_config_value = $ff
  c_default_table = $0600
.ELSE
  .ERROR("Symbol r_target_addr has an invalid value.")
.ENDIF

.IF(!.DEFINED(i_ram_enter))
  i_ram_enter = c_rom_config_value
.ENDIF

.IF(!.DEFINED(i_irq_enter))
  i_irq_enter = 1
.ENDIF

.IF(!.DEFINED(i_ram_exit))
  i_ram_exit = c_rom_config_value
.ENDIF

.IF(!.DEFINED(i_irq_exit))
  .IF(i_ram_exit != i_ram_enter)
  i_irq_exit = 0
  .ELSE
  i_irq_exit = i_irq_enter
  .ENDIF
.ENDIF

.IF(!.DEFINED(i_table_addr))
  i_table_addr = c_default_table
.ENDIF

.IF(i_table_addr < $0200 || i_table_addr > 65536 - 156)
  .ERROR("Symbol i_table_addr must not be < $0200 or > $FF64.")
.ENDIF

.IF(!.DEFINED(r_start_addr))
  .ERROR("Required symbol r_start_addr not defined.")
.ENDIF

.IF(r_start_addr == 0 && i_ram_exit != c_rom_config_value))
  .ERROR("Basic start and non-ROM configuration can't be combined.")
.ENDIF

; -------------------------------------------------------------------
; -- validate some input parameters ---------------------------------
; -------------------------------------------------------------------
.IF(r_target == 4)
  .IF(i_ram_exit != 0 && i_ram_exit != 1)
    .ERROR("Symbol i_ram_exit must have a value [0-1].")
  .ENDIF
  .IF(i_ram_enter != 0 && i_ram_enter != 1)
    .ERROR("Symbol i_ram_enter must have a value [0-1].")
  .ENDIF
.ELIF(r_target == 64)
  .IF(i_ram_exit > $38 || i_ram_exit < $34)
    .ERROR("Symbol i_ram_exit must be undefined or be within [$34-$38].")
  .ENDIF
  .IF(i_ram_enter > $38 || i_ram_exit < $34)
    .ERROR("Symbol i_ram_enter must be undefined or be within [$34-$38].")
  .ENDIF
.ELIF(r_target == 128)
  .IF(i_ram_exit > $3f || i_ram_exit < 0)
    .ERROR("Symbol i_ram_exit must be undefined or be within [0-$3f].")
  .ENDIF
  .IF(i_ram_enter > $3f || i_ram_exit < 0)
    .ERROR("Symbol i_ram_enter must be undefined or be within [0-$3f].")
  .ENDIF
.ENDIF

v_safety_addr = .INCWORD("crunched_data", 0)

; -------------------------------------------------------------------
; -- convert $0 to $10000 but leave $1 - $ffff ----------------------
; -------------------------------------------------------------------
v_highest_addr = (.INCWORD("crunched_data", -2) + 65535) % 65536 + 1

.IF(i_effect == 0 &&
    .DEFINED(c_effect_color) &&
	    c_effect_color < v_safety_addr &&
	    c_effect_color > i_table_addr + 156)
v_start_of_decrunchable_mem = c_effect_color
.ELSE
v_start_of_decrunchable_mem = i_table_addr + 156
.ENDIF

.IF(v_safety_addr < v_start_of_decrunchable_mem)
	  .ERROR("This target can't support the memory demands of the data.")
.ENDIF

; -------------------------------------------------------------------
; -- file2_start_hook and stage2_exit_hook --------------------------
; -------------------------------------------------------------------
.IF(i_effect == 0 && .DEFINED(c_effect_char))
file2_start_hook = 1
  .MACRO("file2_start_hook")
    .IF(v_safety_addr < file2start && ; if we are transferring anyhow
	c_effect_char < v_safety_addr &&
        (file2start - c_effect_char < 257 ||
         file2start - v_safety_addr > 256))
transfer_len = file2start - c_effect_char
lowest_addr = c_effect_char
    .ENDIF
  .ENDMACRO
stage2_exit_hook = 1
  .MACRO("stage2_exit_hook")
    .IF(c_effect_char < lowest_addr || c_effect_char > v_highest_addr)
	sty c_effect_char
    .ENDIF
  .ENDMACRO
.ENDIF

; -------------------------------------------------------------------
; -- calculate "*_during" parameters --------------------------------
; -------------------------------------------------------------------
.IF(!.DEFINED(i_ram_during))
  .IF(v_highest_addr > c_end_of_mem_rom)
    i_ram_during = c_ram_config_value
  .ELSE
    i_ram_during = i_ram_enter
  .ENDIF
.ENDIF

.IF(!.DEFINED(i_irq_during))
  .IF(i_irq_enter == i_irq_exit && i_ram_during == i_ram_enter)
    i_irq_during = i_irq_enter
  .ELSE
    i_irq_during = 0
.ENDIF

; -------------------------------------------------------------------
; -- The decruncher enter macro definition --------------------------
enter_hook = 1
.MACRO("enter_hook")
  .IF(i_irq_during != i_irq_enter)
    .IF(i_irq_during == 1)
	cli
    .ELSE
	sei
    .ENDIF
  .ENDIF
  .IF(i_ram_during != i_ram_enter)
    .INCLUDE("b2d_ram")
  .ENDIF
.ENDMACRO
; -------------------------------------------------------------------
; -------------------------------------------------------------------
; -- The decruncher exit macro definition ---------------------------
; -------------------------------------------------------------------
exit_hook = 1
.MACRO("exit_hook")
  .IF(i_ram_exit != i_ram_during)
    .INCLUDE("d2r_ram")
  .ENDIF
  .IF(i_irq_exit != i_irq_during)
    .IF(i_irq_exit == 1)
	cli
    .ELSE
	sei
    .ENDIF
  .ENDIF
  .IF(r_start_addr == -2)
    .IF(r_target == $a8)
	.ERROR("Atari target can't handle basic start (yet).")
    .ELIF(r_target == 128)
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
    .IF(r_target == 20 || r_target == 23 || r_target == 52 || r_target == 55)
	jsr $c659		; init
	jsr $c533		; regenerate line links and set $2d/$2e
	jmp $c7ae		; start
    .ELIF(r_target == 4)
	jsr $8bbe		; init
	jsr $8818		; regenerate line links and set $2d/$2e
	jsr $f3b5		; regen color table at $0113
	jmp $8bdc		; start
    .ELIF(r_target == 64)
	jsr $a659		; init
	jsr $a533		; regenerate line links and set $2d/$2e
	jmp $a7ae		; start
    .ELIF(r_target == 128)
	jsr $5ab5		; init
	jsr $4f4f		; regenerate line links and set $1210/$1211
	jmp $4af6		; start
    .ENDIF
  .ELSE
	jmp r_start_addr
  .ENDIF
.ENDMACRO

; -------------------------------------------------------------------
; -- The decrunch effect macro definition ---------------------------
; -------------------------------------------------------------------
.IF(.DEFINED(i_fast_effect))
  fast_effect_hook = 1
.ELSE
  slow_effect_hook = 1
.ENDIF
.MACRO("effect_hook")
  .IF(i_effect != -1)
    .INCLUDE("d2io")
    .IF(i_effect == 0)
      .IF(r_target == 4)
	lda <$fd,x
	sta c_effect_color
      .ELSE
	stx c_effect_color
      .ENDIF
    .ELIF(i_effect == 1)
      .IF(r_target == 20 || r_target == 23 || r_target == 52 || r_target == 55)
	and #$07
	ora #$18
      .ELIF(r_target == 4)
	ora #$30
      .ENDIF
	sta c_border_color
    .ELIF(i_effect == 2)
      .IF(r_target == 20 || r_target == 23 || r_target == 52 || r_target == 55)
	txa
	and #$07
	ora #$18
	sta c_border_color
      .ELIF(r_target == 4)
	lda <$fd,x
	sta c_border_color
      .ELSE
	stx c_boder_color
      .ENDIF
    .ELIF(i_effect == 3)
      .IF(r_target == 20 || r_target == 23 || r_target == 52 || r_target == 55)
	tya
	and #$07
	ora #$18
	sta c_border_color
      .ELIF(r_target == 4)
	sty c_border_color
      .ELSE
	sty c_border_color
      .ENDIF
    .ELSE
      .ERROR("Unknown decrunch effect.")
    .ENDIF
    .INCLUDE("io2d")
  .ENDIF
.ENDMACRO

; -------------------------------------------------------------------
; -- The ram/rom switch macros for decrunching ----------------------
; -------------------------------------------------------------------
.IF(r_target == 64)
; -------------------------------------------------------------------
; -- The ram/rom switch macros for c64 ------------------------------
; -------------------------------------------------------------------
  .MACRO("b2d_ram")
    .IF(i_ram_during == i_ram_enter + 1)
	inc <$01
    .ELIF(i_ram_during == i_ram_enter - 1)
	dec <$01
    .ELSE
	lda #i_ram_during
	sta <$01
    .ENDIF
  .ENDMACRO
  .MACRO("d2io")
    .IF(i_ram_during == $34 || i_ram_during == $38)
      .IF(i_irq_during == 1)
	sei
      .ENDIF
      .IF(i_ram_during == $34)
	inc <$01
      .ELIF(i_ram_during == $38)
	dec <$01
      .ENDIF
    .ENDIF
  .ENDMACRO
  .MACRO("io2d")
    .IF(i_ram_during == $34 || i_ram_during == $38)
      .IF(i_ram_during == $34)
	dec <$01
      .ELIF(i_ram_during == $38)
	inc <$01
      .ENDIF
      .IF(i_irq_during == 1)
	cli
      .ENDIF
    .ENDIF
  .ENDMACRO
  .MACRO("d2r_ram")
    .IF(i_ram_exit == i_ram_during + 1)
	inc <$01
    .ELIF(i_ram_exit == i_ram_during - 1)
	dec <$01
    .ELSE
	lda i_ram_exit
	sta <$01
    .ENDIF
  .ENDMACRO
.ELIF(r_target == 128)
; -------------------------------------------------------------------
; -- The ram/rom switch macros for c128 -----------------------------
; -------------------------------------------------------------------
  .MACRO("b2d_ram")
	lda #i_ram_during
	sta $fff0
  .ENDMACRO
  .MACRO("d2io")
    .IF(i_ram_during == $3f)
      .IF(i_irq_during == 1)
	sei
      .ENDIF
      .IF(i_effect == 1)
	pha
      .ENDIF
	lda #c_rom_config_value
	sta $fff0
      .IF(i_effect == 1)
	pla
      .ENDIF
    .ENDIF
  .ENDMACRO
  .MACRO("io2d")
    .IF(i_ram_during == $3f)
      .IF(i_effect == 1)
	pha
      .ENDIF
	lda #i_ram_during
	sta $fff0
      .IF(i_effect == 1)
	pla
      .ENDIF
      .IF(i_irq_during == 1)
	cli
      .ENDIF
    .ENDIF
  .ENDMACRO
  .MACRO("d2r_ram")
	lda #i_ram_exit
	sta $fff0
  .ENDMACRO
.ELIF(r_target == 4)
; -------------------------------------------------------------------
; -- The ram/rom switch macros for c16/+4 ---------------------------
; -------------------------------------------------------------------
  .MACRO("b2d_ram")
    .IF(i_ram_during == c_ram_config_value)
	sta $ff3f
    .ELSE
	sta $ff3e
    .ENDIF
  .ENDMACRO
  .MACRO("d2io")
  .ENDMACRO
  .MACRO("io2d")
  .ENDMACRO
  .MACRO("d2r_ram")
    .IF(i_ram_exit == c_rom_config_value)
	sta $ff3e
    .ELSE
	sta $ff3f
    .ENDIF
  .ENDMACRO
.ELIF(r_target == 20 || r_target == 23 || r_target == 52 || r_target == 55)
; -------------------------------------------------------------------
; -- The ram/rom switch macros for c20 ------------------------------
; -------------------------------------------------------------------
  .MACRO("b2d_ram")
  .ENDMACRO
  .MACRO("d2io")
  .ENDMACRO
  .MACRO("io2d")
  .ENDMACRO
  .MACRO("d2r_ram")
  .ENDMACRO
.ELIF(r_target == $a8)
; -------------------------------------------------------------------
; -- The ram/rom switch macros for a8 -------------------------------
; -------------------------------------------------------------------
  .MACRO("b2d_ram")
  .ENDMACRO
  .MACRO("d2io")
  .ENDMACRO
  .MACRO("io2d")
  .ENDMACRO
  .MACRO("d2r_ram")
  .ENDMACRO
.ELSE
  .ERROR("Unhanded target for macro definitions.")
.ENDIF
; -------------------------------------------------------------------
; -- End of bank switch definitions ---------------------------------
; -------------------------------------------------------------------
; -------------------------------------------------------------------
; -- Start of file header stuff -------------------------------------
; -------------------------------------------------------------------
.IF(r_target == 20 || r_target == 23 || r_target == 52 || r_target == 55 ||
    r_target == 4 || r_target == 64 || r_target == 128)
zp_lo_len = $a7
zp_src_addr = $ae
zp_hi_bits = $9f

	.WORD(c_basic_start)
	.ORG(c_basic_start)
	.WORD(basic_end, 20)
	.BYTE($9e, decr_start / 1000 % 10 + 48, decr_start / 100 % 10 + 48)
	.BYTE(decr_start / 10 % 10 + 48, decr_start % 10 + 48, 0)
basic_end:
; -------------------------------------------------------------------
  .IF(r_target == 4 || r_target == 128)
	  .BYTE(0, 0)
  .ENDIF
decr_start:
.ELIF(r_target == $a8)
zp_lo_len = $f7
zp_src_addr = $f9
zp_hi_bits = $f8

	.WORD($FFFF, a8start, a8end - 1)
	.ORG($2c00)
a8start:
.ELSE
  .ERROR("Unhandled target for file header stuff")
.ENDIF
; -------------------------------------------------------------------
; -- End of file header stuff ---------------------------------------
; -------------------------------------------------------------------

; -------------------------------------------------------------------
; -- Start of the actual decruncher ---------------------------------
; -------------------------------------------------------------------

; -------------------------------------------------------------------
; -- required symbols:
; --
; --  zp_lo_len			A zerpoage location used for a byte.
; --  zp_src_addr		A zeropage location used for a word.
; --  zp_hi_bits		A zeropage location used for a byte.
; --  v_safety_addr
; --  i_literal_sequences_used
; --  i_table_addr
; --
; -- optional symbols
; --  enter_hook		must exit with y == 0.
; --  fast_effect_hook		must not touch x or y.
; --  slow_effect_hook		must not touch x or y.
; --  exit_hook
; --  stage2_exit_hook		must exit with x == 0 and y == 0.
; --
; -- macros:
; --  enter_hook
; --  effect_hook
; --  exit_hook
; --  stage2_exit_hook
; -------------------------------------------------------------------

zp_bitbuf  = $fd
zp_dest_lo = zp_bitbuf + 1	; dest addr lo
zp_dest_hi = zp_bitbuf + 2	; dest addr hi

; -------------------------------------------------------------------
; -- start of stage 1 -----------------------------------------------
; -------------------------------------------------------------------
	ldy #$00
	.IF(.DEFINED(enter_hook))
	  .INCLUDE("enter_hook")
	.ENDIF
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
.IF(.DEFINED(file2_start_hook))
  .INCLUDE("file2_start_hook")
.ENDIF
.IF(!.DEFINED(transfer_len))
	.IF(v_safety_addr > file2start)
transfer_len = 0
lowest_addr = file2start
	.ELSE
transfer_len = file2start - v_safety_addr
lowest_addr = v_safety_addr
	.ENDIF
.ENDIF
	.INCBIN("crunched_data", transfer_len + 2, -2)
	.WORD(v_highest_addr % 65536)
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
;   zp_hi_bits = #bits_hi
; notes:
;   y is untouched
; -------------------------------------------------------------------
get_bits:
	lda #$00
	sta <zp_hi_bits
	cpx #$01
	bcc bits_done
bits_next:
	lsr <zp_bitbuf
	bne ok
	pha
literal_get_byte:
	.IF(.DEFINED(fast_effect_hook))
	  .INCLUDE("effect_hook")
	.ENDIF
	lda get_byte_fixup + 1
	bne get_byte_skip_hi
	dec get_byte_fixup + 2
	.IF(.DEFINED(slow_effect_hook))
	  .INCLUDE("effect_hook")
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
	rol <zp_hi_bits
	dex
	bne bits_next
bits_done:
	.IF(!.DEFINED(exit_hook))
decr_exit:
	.ENDIF
	rts
; -------------------------------------------------------------------
; main copy loop (16 bytes)
;
copy_next_hi:
	dex
	dec <zp_dest_hi
	dec <(zp_src_addr + 1)
copy_next:
	dey
.IF(.DEFINED(i_literal_sequences_used))
	bcc literal_get_byte
.ENDIF
	lda (zp_src_addr),y
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
	beq decr_exit
; -------------------------------------------------------------------
; literal sequence handling
;
	ldx #$10
	jsr get_bits
literal_start1:
	sta <zp_lo_len
	ldx <zp_hi_bits
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
	adc tabl_lo - 1,y	; we have now calculated zp_lo_len
	sta <zp_lo_len
; -------------------------------------------------------------------
; now do the hibyte of the sequence length calculation (6 bytes)
	lda <zp_hi_bits
	adc tabl_hi - 1,y	; c = 0 after this.
	pha
; -------------------------------------------------------------------
; here we decide what offset table to use (20 bytes)
; x is 0 here
;
	bne nots123
	ldy <zp_lo_len
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
	lda <zp_lo_len
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
	inc <zp_hi_bits
	clc
skipcarry:
	adc <zp_dest_lo
	sta <zp_src_addr
	lda <zp_hi_bits
	adc tabl_hi,y
	adc <zp_dest_hi
	sta <(zp_src_addr + 1)
; -------------------------------------------------------------------
; prepare for copy loop (6 bytes)
;
	pla
	tax
.IF(.DEFINED(i_literal_sequences_used))
	sec
pre_copy:
	ldy <zp_lo_len
	jmp copy_start
.ELSE
	ldy <zp_lo_len
	bcc copy_start
.ENDIF
	.IF(.DEFINED(exit_hook))
decr_exit:
	  .INCLUDE("exit_hook")
	.ENDIF
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
	beq shortcut		; starta p� ny sekvens

	txa			; this clears reg a
	lsr			; and sets the carry flag
	ldx tabl_bi-1,y
rolle:
	rol
	rol <zp_hi_bits
	dex
	bpl rolle		; c = 0 after this (rol zp_hi_bits)

	adc tabl_lo-1,y
	tax

	lda <zp_hi_bits
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
	sta v_safety_addr + transfer_len / 256 * 256,y
copy2_start:
	tya
	bne copy2_loop1
	txa
	bne copy2_loop2
	  .ENDIF
	.ENDIF
	.IF(.DEFINED(stage2_exit_hook))
	  .INCLUDE("stage2_exit_hook")
	.ENDIF
	jmp begin
; -------------------------------------------------------------------
; -- end of stage 2 -------------------------------------------------
; -------------------------------------------------------------------

; -------------------------------------------------------------------
; -- End of the actual decruncher -----------------------------------
; -------------------------------------------------------------------

; -------------------------------------------------------------------
; -- Start of file footer stuff -------------------------------------
; -------------------------------------------------------------------

.IF(r_target == 20 || r_target == 23 || r_target == 52 || r_target == 55 ||
    r_target == 4 || r_target == 64 || r_target == 128)
.ELIF(r_target == $a8)
a8end:
	.WORD($02e0, $02e1, a8start)
.ELSE
  .ERROR("Unhandled target for file header stuff")
.ENDIF
; -------------------------------------------------------------------
; -- End of file footer stuff ---------------------------------------
; -------------------------------------------------------------------