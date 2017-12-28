;
; Copyright (c) 2002 - 2017 Magnus Lind.
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
; -------------------------------------------------------------------
; The decruncher jsr:s to the get_crunched_byte address when it wants to
; read a crunched byte into A. This subroutine has to preserve X and Y
; register and must not modify the state of the carry nor the overflow flag.
; -------------------------------------------------------------------
.import get_crunched_byte
; -------------------------------------------------------------------
; this function is the heart of the decruncher.
; It initializes the decruncher zeropage locations and precalculates the
; decrunch tables and decrunches the data
; This function will not change the interrupt status bit and it will not
; modify the memory configuration.
; -------------------------------------------------------------------
.export decrunch

; -------------------------------------------------------------------
; Controls if the shared get_bits routines should be inlined or not.
INLINE_GET_BITS=1
; -------------------------------------------------------------------
; if literal sequences is not used (the data was crunched with the -c
; flag) then the following line can be uncommented for shorter and.
; slightly faster code.
;LITERAL_SEQUENCES_NOT_USED = 1
; -------------------------------------------------------------------
; if the sequence length is limited to 256 (the data was crunched with
; the -M256 flag) then the following line can be uncommented for
; shorter and slightly faster code.
;MAX_SEQUENCE_LENGTH_256 = 1
; -------------------------------------------------------------------
; zero page addresses used
; -------------------------------------------------------------------
zp_len_lo = $a7
zp_len_hi = $a8

zp_src_lo  = $ae
zp_src_hi  = zp_src_lo + 1

zp_dest_y = $fb
zp_bits_hi = $fc

zp_bitbuf  = $fd
zp_dest_lo = zp_bitbuf + 1      ; dest addr lo
zp_dest_hi = zp_bitbuf + 2      ; dest addr hi

tabl_bi = decrunch_table
tabl_lo = decrunch_table + 52
tabl_hi = decrunch_table + 104

        ;; refill bits is always inlined
.MACRO mac_refill_bits
        pha
        jsr get_crunched_byte
        rol
        sta zp_bitbuf
        pla
.ENDMACRO

.IFDEF INLINE_GET_BITS
.MACRO mac_get_bits
.SCOPE
        adc #$80		; needs c=0, affects v
        asl
        bpl gb_skip
gb_next:
        asl <zp_bitbuf
        bne gb_ok
        mac_refill_bits
gb_ok:
        rol
        bmi gb_next
gb_skip:
        bvc skip
        sec
        sta <zp_bits_hi
        jsr get_crunched_byte
skip:
.ENDSCOPE
.ENDMACRO
.ELSE
.MACRO mac_get_bits
        jsr get_bits
.ENDMACRO
get_bits:
        adc #$80		; needs c=0, affects v
        asl
        bpl gb_skip
gb_next:
        asl <zp_bitbuf
        bne gb_ok
        mac_refill_bits
gb_ok:
        rol
        bmi gb_next
gb_skip:
        bvc return
        sec
        sta <zp_bits_hi
        jmp get_crunched_byte
.ENDIF
; -------------------------------------------------------------------
; no code below this comment has to be modified in order to generate
; a working decruncher of this source file.
; However, you may want to relocate the tables last in the file to a
; more suitable address.
; -------------------------------------------------------------------

; -------------------------------------------------------------------
; jsr this label to decrunch, it will in turn init the tables and
; call the decruncher
; no constraints on register content, however the
; decimal flag has to be #0 (it almost always is, otherwise do a cld)
decrunch:
; -------------------------------------------------------------------
; init zeropage, x and y regs. (12 bytes)
;
        ldy #0
        ldx #3
init_zp:
        jsr get_crunched_byte
        sta zp_bitbuf - 1,x
        dex
        bne init_zp
; -------------------------------------------------------------------
; calculate tables (52 bytes) + get_bits_nc macro
; x and y must be #0 when entering
;
table_gen:
        tax
        tya
        and #$0f
        sta tabl_lo,y
        beq shortcut            ; start a new sequence
; -------------------------------------------------------------------
        txa
        adc tabl_lo - 1,y
        sta tabl_lo,y
        lda <zp_bits_hi
        adc tabl_hi - 1,y
shortcut:
        sta tabl_hi,y
; -------------------------------------------------------------------
        lda #$78                ; %01111000
        mac_get_bits
        tax
        lda tabl_mask,x
        sta tabl_bi,y
; -------------------------------------------------------------------
        lda #0
        sta <zp_bits_hi
rolle:
        rol
        rol <zp_bits_hi
        dex
        bpl rolle
        inx
; -------------------------------------------------------------------
        iny
        cpy #52
        bne table_gen
; -------------------------------------------------------------------
; prepare for main decruncher
        ldy zp_dest_lo
        stx zp_dest_lo
        bcs literal_start1
; -------------------------------------------------------------------
; The used static mask table (16 bytes)
tabl_mask:
        .BYTE %00000000, %01000000, %01100000, %01110000
        .BYTE %01111000, %01111100, %01111110, %01111111
        .BYTE %10000000, %11000000, %11100000, %11110000
        .BYTE %11111000, %11111100, %11111110, %11111111
return:
        rts
; -------------------------------------------------------------------
; main copy loop (30 bytes)
; entry point is at the copy_start label
; y must contain low byte of dest zp pointer
; x must contain copy length + 1 % 256,
; zp_len_hi must contain copy length / 256
;
.IFNDEF MAX_SEQUENCE_LENGTH_256
copy_next_hi:
        dec zp_len_hi
.ENDIF
copy_next:
        tya
        bne copy_skip_hi
        dec zp_dest_hi
        dec zp_src_hi
copy_skip_hi:
        dey
.IFNDEF LITERAL_SEQUENCES_NOT_USED
        bcc skip_literal_byte
        jsr get_crunched_byte
        bcs literal_byte_gotten
skip_literal_byte:
.ENDIF
        lda (zp_src_lo),y
literal_byte_gotten:
        sta (zp_dest_lo),y
copy_start:
        dex
        bne copy_next
.IFNDEF MAX_SEQUENCE_LENGTH_256
        lda zp_len_hi
        bne copy_next_hi
.ENDIF
        beq next_round
; -------------------------------------------------------------------
; copy one literal byte to destination (11 bytes)
;
literal_start1:
        tya
        bne no_hi_decr
        dec zp_dest_hi
no_hi_decr:
        dey
        jsr get_crunched_byte
        sta (zp_dest_lo),y
; -------------------------------------------------------------------
; fetch sequence length index (14 bytes)
; x must be #0 when entering and contains the length index + 1
; when exiting or 0 for literal byte
next_round:
        dex
        lda zp_bitbuf
no_literal1:
        asl
        bne nofetch8
        jsr get_crunched_byte
        rol
nofetch8:
        inx
        bcc no_literal1
        sta zp_bitbuf
; -------------------------------------------------------------------
; check for literal byte (2 bytes)
;
        beq literal_start1
; -------------------------------------------------------------------
; check for decrunch done and literal sequences (6 bytes)
;
        cpx #$11
        bcc sequence_start
        beq return
.IFNDEF LITERAL_SEQUENCES_NOT_USED
; -------------------------------------------------------------------
; literal sequence handling (12 bytes)
;
        jsr get_crunched_byte
        sta zp_len_hi
        jsr get_crunched_byte
        tax
.IFNDEF MAX_SEQUENCE_LENGTH_256
        inx
        bcs copy_start
.ELSE
        bcs copy_next
.ENDIF
.ENDIF
; -------------------------------------------------------------------
; calulate length of sequence (zp_len) (14 bytes) + get_bits macro
;
sequence_start:
        sty zp_dest_y
        ldy #0
        sty zp_bits_hi
        lda tabl_bi - 1,x
        mac_get_bits
        adc tabl_lo - 1,x       ; we have now calculated zp_len_lo
        sta zp_len_lo
.IFNDEF MAX_SEQUENCE_LENGTH_256
; -------------------------------------------------------------------
; now do the hibyte of the sequence length calculation (9 bytes)
        lda zp_bits_hi
        adc tabl_hi - 1,x       ; c = 0 after this.
        sta zp_len_hi
        sty zp_bits_hi
; -------------------------------------------------------------------
; here we decide what offset table to use (14 bytes) + get_bits_nc macro
; z-flag reflects zp_len_hi here
;
        bne nots123
        ldx zp_len_lo
.ELSE
        tax
        beq nots123
.ENDIF
        cpx #$04
        bcc size123
nots123:
        ldx #$03
size123:
        lda tabl_bit - 1,x
gbnc2_next:
        asl <zp_bitbuf
        bne gbnc2_ok
        mac_refill_bits
gbnc2_ok:
        rol
        bcs gbnc2_next
        tax
; -------------------------------------------------------------------
; calulate absolute offset (zp_src) (17 bytes) + get_bits macro
;
        lda tabl_bi,x
        mac_get_bits
        adc tabl_lo,x
        sta zp_src_lo
        lda zp_bits_hi
        adc tabl_hi,x
        adc zp_dest_hi
        sta zp_src_hi
; -------------------------------------------------------------------
; prepare for copy loop (8 bytes)
;
pre_copy:
        ldx zp_len_lo
        ldy zp_dest_y
.IFNDEF MAX_SEQUENCE_LENGTH_256
        inx
        jmp copy_start
.ELSE
        jmp copy_next
.ENDIF
; -------------------------------------------------------------------
; the static stable used for bits+offset 1,2 and 3+ (3 bytes)
; bits 2,4,4 and offs 48,32,16.
tabl_bit:
        .BYTE %10001100, %11100010, %11100001
; -------------------------------------------------------------------
; end of decruncher
; -------------------------------------------------------------------

; -------------------------------------------------------------------
; this 156 byte table area may be relocated. It may also be clobbered
; by other data between decrunches.
; -------------------------------------------------------------------
decrunch_table:
        .byte 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
        .byte 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
        .byte 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
        .byte 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
        .byte 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
        .byte 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
        .byte 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
        .byte 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
        .byte 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
        .byte 0,0,0,0,0,0,0,0,0,0,0,0
; -------------------------------------------------------------------
; end of decruncher
; -------------------------------------------------------------------
