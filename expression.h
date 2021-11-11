#ifndef _EXPRESSION_H_
#define _EXPRESSION_H_

#include <stack>

#include "funs.h"

enum OpCode {
    NOOP = 0,
    INC,
    DEC,
    BINV,
    POS,
    NEG,
    NOT,
    MUL,
    DIV,
    MOD,
    ADD,
    SUB,
    LSHF,
    RSHF,
    LT,
    GT,
    LE,
    GE,
    EQ,
    NE,
    BAND,
    BXOR,
    BOR,
    AND,
    OR,
    ASSIGN,
    MULAS,
    DIVAS,
    MODAS,
    ADDAS,
    SUBAS,
    LSHFAS,
    RSHFAS,
    BANDAS,
    BXORAS,
    BORAS,
    ANDAS,
    ORAS,
    LPAR,
    RPAR,
    LBRK,
    RBRK,
    COMM,
    FUN,
    FUNE,
    END
};

class Script;

class Expression
{
public:
    Expression();
    virtual ~Expression();

    void bind(Script *sc);
    void clear();
    void push(const Operand &o);
    void pushOp(enum OpCode op);
    void pushFun(FunType *f);
    const Operand &value();
    bool empty() const;

private:
    enum OpDir { L2R, R2L };

    struct OpInfo {
        int pri;
        int operandsNum;
        enum OpDir dir;
        void (Expression::*op)(Operand &i);
    };

    static const struct OpInfo ops[];

    Script *m_script;

    stack<enum OpCode> oStack;
    stack<Operand> nStack;
    stack<FunType *> fStack;

    void checkOperandsNum(enum OpCode op);
    void popOp();
    void popFun(bool has_para = false);

    void op_inc(Operand &i);
    void op_dec(Operand &i);
    void op_binv(Operand &i);
    void op_pos(Operand &i);
    void op_neg(Operand &i);
    void op_not(Operand &i);
    void op_mul(Operand &i);
    void op_div(Operand &i);
    void op_mod(Operand &i);
    void op_add(Operand &i);
    void op_sub(Operand &i);
    void op_lshf(Operand &i);
    void op_rshf(Operand &i);
    void op_lt(Operand &i);
    void op_gt(Operand &i);
    void op_le(Operand &i);
    void op_ge(Operand &i);
    void op_eq(Operand &i);
    void op_ne(Operand &i);
    void op_band(Operand &i);
    void op_bxor(Operand &i);
    void op_bor(Operand &i);
    void op_and(Operand &i);
    void op_or(Operand &i);
    void op_assign(Operand &i);
    void op_mulas(Operand &i);
    void op_divas(Operand &i);
    void op_modas(Operand &i);
    void op_addas(Operand &i);
    void op_subas(Operand &i);
    void op_lshfas(Operand &i);
    void op_rshfas(Operand &i);
    void op_bandas(Operand &i);
    void op_bxoras(Operand &i);
    void op_boras(Operand &i);
    void op_andas(Operand &i);
    void op_oras(Operand &i);
    void op_lpar(Operand &i);
    void op_lbrk(Operand &i);
    void op_comm(Operand &i);

    void do_lpar();
    void do_lbrk();
};

#endif
