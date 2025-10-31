#ifndef _CMM_EXPRESSION_H_
#define _CMM_EXPRESSION_H_

#include <stack>

#include "error.h"
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

class Expression
{
public:
    Expression()
    {
    }

    virtual ~Expression()
    {
    }

    void bind(void *context)
    {
        m_context = context;
    }

    void push(const Operand &o)
    {
        nStack.push(o);
    }

    const Operand &value() const
    {
        return nStack.top();
    }

    bool empty() const
    {
        return nStack.empty() && oStack.empty();
    }

    void pushFun(FunType *f)
    {
        oStack.push(FUN);
        fStack.push(f);
    }

    void clear();
    void pushOp(enum OpCode op);

private:
    enum OpDir { L2R, R2L };

    struct OpInfo {
        int pri;
        int operandsNum;
        enum OpDir dir;
        void (Expression::*op)(const Operand &i);
    };

    static const struct OpInfo ops[];

    void *m_context;

    stack<enum OpCode> oStack;
    stack<Operand> nStack;
    stack<FunType *> fStack;

    void checkOperandsNum(enum OpCode op)
    {
        if (nStack.size() < (stack<Operand>::size_type)ops[op].operandsNum) {
            throw ERR_LACK_OPEE;
        }
    }

    void popOp();
    void popFun(bool has_para = false);

    void op_inc(const Operand &i);
    void op_dec(const Operand &i);
    void op_binv(const Operand &i);
    void op_pos(const Operand &i);
    void op_neg(const Operand &i);
    void op_not(const Operand &i);
    void op_mul(const Operand &i);
    void op_div(const Operand &i);
    void op_mod(const Operand &i);
    void op_add(const Operand &i);
    void op_sub(const Operand &i);
    void op_lshf(const Operand &i);
    void op_rshf(const Operand &i);
    void op_lt(const Operand &i);
    void op_gt(const Operand &i);
    void op_le(const Operand &i);
    void op_ge(const Operand &i);
    void op_eq(const Operand &i);
    void op_ne(const Operand &i);
    void op_band(const Operand &i);
    void op_bxor(const Operand &i);
    void op_bor(const Operand &i);
    void op_and(const Operand &i);
    void op_or(const Operand &i);
    void op_assign(const Operand &i);
    void op_mulas(const Operand &i);
    void op_divas(const Operand &i);
    void op_modas(const Operand &i);
    void op_addas(const Operand &i);
    void op_subas(const Operand &i);
    void op_lshfas(const Operand &i);
    void op_rshfas(const Operand &i);
    void op_bandas(const Operand &i);
    void op_bxoras(const Operand &i);
    void op_boras(const Operand &i);
    void op_andas(const Operand &i);
    void op_oras(const Operand &i);
    void op_lpar(const Operand &i);
    void op_lbrk(const Operand &i);
    void op_comm(const Operand &i);

    void do_lpar();
    void do_lbrk();
};

#endif /* _CMM_EXPRESSION_H_ */
