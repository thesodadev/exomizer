/* scanner for a simple assembler */
%{
#include <stdio.h>
#include <string.h>
#include "int.h"
#include "membuf.h"
#include "parse.h"
#include "asm.tab.h"

#define MAX_SRC_BUFFER_DEPTH 10
static YY_BUFFER_STATE src_buffers[MAX_SRC_BUFFER_DEPTH];
static int src_buffer_depth = 0;

static char *strdupped_get(char *text);
static int strdupped_cmp(const void *a, const void *b);
static void strdupped_free(void *a);

int num_lines = 1;
int push_state_skip = 0;
int push_state_init = 0;
int push_state_macro = 0;
struct vec strdupped[1];

%}

%x SKIP SKIP_ALL QUOTED_STRING SKIP_LINE MACROO

%option noyywrap
%option case-insensitive
%option stack

%%

	if(push_state_init)
	{push_state_init = 0; yy_push_state(INITIAL); }
	if(push_state_skip)
	{push_state_skip = 0; yy_push_state(SKIP); }
	if(push_state_macro)
	{push_state_macro = 0; yy_push_state(MACROO); }

\.if		return IF;
\.elif		BEGIN(SKIP_ALL);
\.else		BEGIN(SKIP);
\.endif		yy_pop_state();

\.include	return INCLUDE;
\.macro		return MACRO;

\.defined	return DEFINED;
\.org		return ORG;
\.error		return ERROR;
\.echo		return ECHO;
\.incbin	return INCBIN;
\.incword	return INCWORD;
\.res		return RES;
\.word		return WORD;
\.byte		return BYTE;

\"		BEGIN(QUOTED_STRING);
\;		BEGIN(SKIP_LINE);

lda		return LDA;
ldx		return LDX;
ldy		return LDY;
sta		return STA;
stx		return STX;
sty		return STY;
and		return AND;
ora		return ORA;
eor		return EOR;
adc		return ADC;
sbc		return SBC;
cmp		return CMP;
cpx		return CPX;
cpy		return CPY;

tsx		return TSX;
txs		return TXS;
pha		return PHA;
pla		return PLA;
php		return PHP;
plp		return PLP;
sei		return SEI;
cli		return CLI;
nop		return NOP;
tya		return TYA;
tay		return TAY;
txa		return TXA;
tax		return TAX;
clc		return CLC;
sec		return SEC;
rts		return RTS;

jsr		return JSR;
jmp		return JMP;
beq		return BEQ;
bne		return BNE;
bcc		return BCC;
bcs		return BCS;
bpl		return BPL;
bmi		return BMI;
bvc		return BVC;
bvs		return BCS;
inx		return INX;
dex		return DEX;
iny		return INY;
dey		return DEY;
inc		return INC;
dec		return DEC;
lsr		return LSR;
asl		return ASL;
ror		return ROR;
rol		return ROL;

[0-9]+		{ yylval.num = atoi(yytext); return NUMBER; }

$[0-9a-z]+	{ yylval.num = strtol(yytext + 1, NULL, 16); return NUMBER; }

\<		return LT;
\>		return GT;
==		return EQ;
!=		return NEQ;
!		return LNOT;
\&\&		return LAND;
\|\|		return LOR;

\(		return LPAREN;
\)		return RPAREN;
\,		return COMMA;
\:		return COLON;
\#		return HASH;
\+		return PLUS;
\-		return MINUS;
\*		return MULT;
\/		return DIV;
\%		return MOD;

\=		return ASSIGN;

x		return X;
y		return Y;

[a-z][_a-z0-9]*	{ yylval.str = strdupped_get(yytext); return SYMBOL; }

\r\n|\n		++num_lines;

[ \t]|;	/* eat whitespace */

.		printf("unknown character found %s\n", yytext);

<SKIP>\.if		yy_push_state(SKIP_ALL);
<SKIP>\.elif		{ yy_pop_state(); return IF; }
<SKIP>\.else		BEGIN(INITIAL);
<SKIP>\.endif		yy_pop_state();
<SKIP>\r\n|\n		++num_lines;
<SKIP>.

<MACROO>\.endmacro	yy_pop_state();
<MACROO>\.+		{ yylval.str = yytext; return MACRO_STRING; }
<MACROO>[^\.]+		{ yylval.str = yytext; return MACRO_STRING; }

<SKIP_ALL>\.if		yy_push_state(SKIP_ALL);
<SKIP_ALL>\.endif	yy_pop_state();
<SKIP_ALL>\r\n|\n	++num_lines;
<SKIP_ALL>.

<QUOTED_STRING>[^\"]*	{
    yylval.str = strdupped_get(yytext);
    return STRING;
}
<QUOTED_STRING>\r\n|\n	++num_lines;
<QUOTED_STRING>\"	BEGIN(INITIAL);

<SKIP_LINE>\r\n|\n	{ ++num_lines; BEGIN(INITIAL); }

<<EOF>>	{
	    if(--src_buffer_depth == 0)
	    {
		yyterminate();
	    }
	    else
	    {
		yy_delete_buffer(YY_CURRENT_BUFFER);
		yy_switch_to_buffer(src_buffers[src_buffer_depth]);
	    }
	}

%%

void scanner_init()
{
    vec_init(strdupped, sizeof(char*));
}

void asm_src_buffer_push(struct membuf *buffer)
{
    if(src_buffer_depth == MAX_SRC_BUFFER_DEPTH)
    {
	fprintf(stderr, "source buffers nested too deep\n");
	exit(-1);
    }
    src_buffers[src_buffer_depth++] = YY_CURRENT_BUFFER;
    yy_scan_bytes(membuf_get(buffer), membuf_memlen(buffer));
}

static char *strdupped_get(char *text)
{
    char **pp;
    /*printf("get \"%s\" => ", text);*/
    if(vec_insert_uniq(strdupped, strdupped_cmp, &text, (void*)&pp))
    {
        /* replace the pointer to <text> since <text> will be reused */
        *pp = strdup(text);
    }
    /*printf("%p\n", *pp);*/
    return *pp;
}

static void strdupped_free(void *a)
{
    char *b = *(char**)a;
    /*printf("free => %p \"%s\"\n", b, b);*/
    free(b);
}

static int strdupped_cmp(const void *a, const void *b)
{
    char *c = *(char**)a;
    char *d = *(char**)b;

    return strcmp(c, d);
}

void scanner_free()
{
    vec_free(strdupped, strdupped_free);
}
