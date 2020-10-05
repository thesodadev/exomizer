;Exomizer3 Intel 8080 decoder version by Ivan Gorodetsky
;Based on Exomizer2 Z80 decoder by Metalbrain, but now it is nearly completely rewritten
;Compression algorithm by Magnus Lind
;input: 	hl=compressed data start
;			de=uncompressed destination start
;			you may change exo_mapbasebits to point to any 256-byte aligned free buffer
;v1.1 - 04.09.2019
;compress with <raw -P15 -M256 -b> options
;246 bytes - reusable
;241 bytes - single-use
;
;Compile with The Telemark Assembler (TASM) 3.2

exo_mapbasebits		.equ 0FF00h

deexo:
			mvi a,80h
			sta	exo_getbit+1
;Two lines above can be commented out in case of single-use
			
			push d
			xra a
			mov c,a
exo_initbits:
			sui 48
			jnc exo_initbits
			adi 48
			jnz exo_node1
			lxi d,1
exo_node1:
			mvi b,4
			push d
			call	exo_getbits_
			xra a
			ora d
			pop d
			rrc
			push	h
			mvi b,exo_mapbasebits>>8
			stax b
			jnc skipPlus8
			adi -128+8
skipPlus8:
			lxi h,1
			jz skip_exo_setbit
exo_setbit:
			dad	h
			dcr a
			jnz	exo_setbit
skip_exo_setbit:
			dad	d
			inr c
			mov a,e
			stax b
			inr c
			mov a,d
			stax b
			inr c
			xchg
			pop	h
			mov a,c
			cpi 52*3
			jnz	exo_initbits
			pop	d
			lda exo_getbit+1
exo_literalcopy1:
			mov c,m\ xchg\ mov m,c\ xchg
			dcx h
			dcx d
exo_mainloop:
			add a
			jnz $+6\ mov a,m\ dcx h\ adc a
			jc	exo_literalcopy1
			mvi	c, 0FFh
exo_getindex:
			inr	c
			add a
			jnz $+6\ mov a,m\ dcx h\ adc a
			jnc	exo_getindex
			sta	exo_getbit+1
			mov a,c
			cpi 16
			jc	exo_continue
			rz
			dcx h
			mov c,m
			dcx h
exo_literalcopy:
			mov	a,m
			stax d
			dcx	h
			dcx	d
			dcr c
			jnz $-5
			jmp exo_getbit_
exo_continue:
			push d
			add a
			add c
			mov c,a
			mvi	b,exo_mapbasebits>>8
			ldax	b
			mov b,a
			mov d,a
			ora a
			cnz	exo_getbits
			mvi b,exo_mapbasebits>>8
			inr c
			ldax b
			add d
			push psw
			lxi	b,0230h
			dcr a
			jz	exo_goforit
			lxi	b,0420h
			dcr a
			jz	exo_goforit
			mvi	c,10h
exo_goforit:
			call exo_getbits_
			mov a,c
			add d
			mov c,a
			add a
			add c
			mov c,a
			mvi b,exo_mapbasebits>>8
			ldax b
			mov b,a
			mov e,a
			mov d,a
			ora a
			cnz exo_getbits
			mvi b,exo_mapbasebits>>8
			inr c
			ldax b
			inr c
			add d
			mov d,a
			ldax b
			adc e
			mov e,d
			mov d,a
			pop b
			xthl
			xchg
			dad d
			mov	a,m
			stax d
			dcx	h
			dcx	d
			dcr b
			jnz	$-5
			pop	h
exo_getbit_:
			lda exo_getbit+1
			jmp	exo_mainloop

exo_getbits:
			jm GT7
exo_getbits_:
			xchg
exo_getbit:
			lxi h,0080h
			mov a,l
exo_gettingbits:
			add a
			jnz $+7 \ ldax d\ dcx d\ mov l,a\ adc a
			dad h
			dcr	b
			jnz	exo_gettingbits
exo_gettingbitsexit:
			xchg
			sta exo_getbit+1
			mov e,b
			ret
GT7:
			ani 7Fh
			mov b,a
			mov d,a
			cnz exo_getbits_
			mov e,d
			mov d,m
			dcx h
			ret

			.end