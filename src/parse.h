#ifndef INCLUDED_PARSE_H
#define INCLUDED_PARSE_H

#include "int.h"
#include "vec.h"
#include "membuf.h"
#include "expr.h"

#define ATOM_TYPE_OP_ARG_NONE    0	/* uses u.op */
#define ATOM_TYPE_OP_ARG_U8      1	/* uses u.op */
#define ATOM_TYPE_OP_ARG_U16     2	/* uses u.op */
#define ATOM_TYPE_OP_ARG_I8      3	/* uses u.op */
#define ATOM_TYPE_OP_ARG_UI8     4	/* uses u.op */
#define ATOM_TYPE_EXPRS		12	/* uses u.exprs */
#define ATOM_TYPE_WORD_EXPRS	10	/* uses u.exprs */
#define ATOM_TYPE_BYTE_EXPRS	11	/* uses u.exprs */
#define ATOM_TYPE_RES		13	/* uses u.res */
#define ATOM_TYPE_BUFFER	14	/* uses u.buffer */

struct op
{
    struct expr *arg;
    u8 code;
};

struct res
{
    struct expr *length;
    struct expr *value;
};

struct buffer
{
    const char *name;
    i32 length;
    i32 skip;
};

struct atom
{
    u8 type;
    union
    {
        struct op op;
        struct vec *exprs;
        struct buffer buffer;
        struct res res;
    } u;
};

extern int push_state_skip;
extern int push_state_macro;
extern int push_state_init;
extern int num_lines;

void parse_init();
void parse_free();

struct atom *new_op(u8 op_code, u8 op_size, struct expr *arg);
struct atom *new_op0(u8 op_code);

struct atom *new_exprs(struct expr *arg);
struct atom *exprs_add(struct atom *atom, struct expr *arg);
struct atom *exprs_to_byte_exprs(struct atom *atom);
struct atom *exprs_to_word_exprs(struct atom *atom);

struct atom *new_res(struct expr *len, struct expr *value);
struct atom *new_incbin(const char *name, struct expr *skip, struct expr *len);

struct expr *new_is_defined(const char *symbol);
struct expr *new_expr_incword(const char *name, struct expr *skip);

void new_symbol(const char *symbol, i32 value);
void new_symbol_expr(const char *symbol, struct expr *arg);

void symbol_dump_resolved(int level, const char *symbol);

void new_label(const char *label);
void set_org(struct expr *arg);
void push_if_state(struct expr *arg);
void push_macro_state(const char *name);
void macro_append(const char *text);
void asm_error(const char *msg);
void asm_echo(const char *msg);
void asm_include(const char *msg);

int assemble(struct membuf *source, struct membuf *dest);
void output_atoms(struct membuf *out, struct vec *mem);
void asm_src_buffer_push(struct membuf *buf);

#endif
