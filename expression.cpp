#include "expression.h"
#include "error.h"

const struct Expression::OpInfo Expression::ops[] = {
    {0, 0, L2R, 0},
    {1, 1, R2L, &Expression::op_inc},
    {1, 1, R2L, &Expression::op_dec},
    {1, 1, R2L, &Expression::op_binv},
    {1, 1, R2L, &Expression::op_pos},
    {1, 1, R2L, &Expression::op_neg},
    {1, 1, R2L, &Expression::op_not},
    {2, 2, L2R, &Expression::op_mul},
    {2, 2, L2R, &Expression::op_div},
    {2, 2, L2R, &Expression::op_mod},
    {3, 2, L2R, &Expression::op_add},
    {3, 2, L2R, &Expression::op_sub},
    {4, 2, L2R, &Expression::op_lshf},
    {4, 2, L2R, &Expression::op_rshf},
    {5, 2, L2R, &Expression::op_lt},
    {5, 2, L2R, &Expression::op_gt},
    {5, 2, L2R, &Expression::op_le},
    {5, 2, L2R, &Expression::op_ge},
    {6, 2, L2R, &Expression::op_eq},
    {6, 2, L2R, &Expression::op_ne},
    {7, 2, L2R, &Expression::op_band},
    {8, 2, L2R, &Expression::op_bxor},
    {9, 2, L2R, &Expression::op_bor},
    {10, 2, L2R, &Expression::op_and},
    {11, 2, L2R, &Expression::op_or},
    {12, 2, R2L, &Expression::op_assign},
    {12, 2, R2L, &Expression::op_mulas},
    {12, 2, R2L, &Expression::op_divas},
    {12, 2, R2L, &Expression::op_modas},
    {12, 2, R2L, &Expression::op_addas},
    {12, 2, R2L, &Expression::op_subas},
    {12, 2, R2L, &Expression::op_lshfas},
    {12, 2, R2L, &Expression::op_rshfas},
    {12, 2, R2L, &Expression::op_bandas},
    {12, 2, R2L, &Expression::op_bxoras},
    {12, 2, R2L, &Expression::op_boras},
    {12, 2, R2L, &Expression::op_andas},
    {12, 2, R2L, &Expression::op_oras},
    {99, 0, R2L, &Expression::op_lpar},
    {99, 0, R2L, 0},
    {99, 2, R2L, &Expression::op_lbrk},
    {99, 0, R2L, 0},
    {99, 0, R2L, &Expression::op_comm},
    {99, 0, R2L, 0},
    {99, 0, R2L, 0},
    {100, 0, R2L, 0}, // END
};

Expression::Expression()
{
    clear();
}

Expression::~Expression()
{
}

void Expression::bind(Script *sc)
{
    m_script = sc;
}

void Expression::clear()
{
    while (!oStack.empty()) {
        oStack.pop();
    }
    while (!nStack.empty()) {
        nStack.pop();
    }
    while (!fStack.empty()) {
        fStack.pop();
    }
}

void Expression::push(const Operand &o)
{
    nStack.push(o);
}

void Expression::pushOp(enum OpCode op)
{
    if (op == LPAR || op == LBRK) {
        oStack.push(op);
    } else if (op == FUNE) {
        popFun();
    } else {
        while (!oStack.empty() &&
               (ops[oStack.top()].pri < ops[op].pri || (ops[oStack.top()].pri == ops[op].pri && ops[op].dir == L2R))) {
            popOp();
        }
        if (op == COMM) {
            if (!oStack.empty() && (oStack.top() == FUN || oStack.top() == COMM)) {
                oStack.push(op);
            } else {
                do_lbrk();
            }
        } else if (op == RBRK) {
            do_lbrk();
            oStack.pop();
        } else if (op == RPAR) {
            if (!oStack.empty() && (oStack.top() == FUN || oStack.top() == COMM)) {
                popFun(true);
            } else {
                do_lpar();
                oStack.pop();
            }
        } else if (op != END) {
            oStack.push(op);
        }
    }
}

void Expression::pushFun(FunType *f)
{
    oStack.push(FUN);
    fStack.push(f);
}

const Operand &Expression::value()
{
    return nStack.top();
}

bool Expression::empty() const
{
    return nStack.empty() && oStack.empty();
}

void Expression::checkOperandsNum(enum OpCode op)
{
    if (nStack.size() < (stack<Operand>::size_type)ops[op].operandsNum) {
        throw ERR_LACK_OPEE;
    }
}

void Expression::popOp()
{
    Operand i;
    enum OpCode op = oStack.top();
    checkOperandsNum(op);
    if (ops[op].operandsNum == 2) {
        i = nStack.top();
        nStack.pop();
    }
    (this->*ops[op].op)(i);
    oStack.pop();
}

void Expression::popFun(bool has_para)
{
    Operand r(Data(0));
    deque<Operand> p;
    FunType *f = fStack.top();
    if (has_para) {
        while (oStack.top() == COMM) {
            p.push_front(nStack.top());
            nStack.pop();
            if (oStack.top() == FUN) {
                break;
            }
            oStack.pop();
        }
        p.push_front(nStack.top());
        nStack.pop();
    }
    f(m_script, r, p);
    nStack.push(r);
    oStack.pop();
    fStack.pop();
}

void Expression::op_inc(Operand &i)
{
    nStack.top() = nStack.top().setLv(nStack.top() + 1);
}

void Expression::op_dec(Operand &i)
{
    nStack.top() = nStack.top().setLv(nStack.top() - 1);
}

void Expression::op_binv(Operand &i)
{
    nStack.top() = ~nStack.top();
}

void Expression::op_pos(Operand &i)
{
}

void Expression::op_neg(Operand &i)
{
    nStack.top() = -nStack.top();
}

void Expression::op_not(Operand &i)
{
    nStack.top() = !nStack.top();
}

void Expression::op_mul(Operand &i)
{
    nStack.top() = nStack.top() * i;
}

void Expression::op_div(Operand &i)
{
    if (i == 0) {
        throw ERR_DIV_BY_ZERO;
    }
    nStack.top() = nStack.top() / i;
}

void Expression::op_mod(Operand &i)
{
    if (i == 0) {
        throw ERR_DIV_BY_ZERO;
    }
    nStack.top() = nStack.top() % i;
}

void Expression::op_add(Operand &i)
{
    nStack.top() = nStack.top() + i;
}

void Expression::op_sub(Operand &i)
{
    nStack.top() = nStack.top() - i;
}

void Expression::op_lshf(Operand &i)
{
    nStack.top() = nStack.top() << i;
}

void Expression::op_rshf(Operand &i)
{
    nStack.top() = nStack.top() >> i;
}

void Expression::op_lt(Operand &i)
{
    nStack.top() = nStack.top() < i;
}

void Expression::op_gt(Operand &i)
{
    nStack.top() = nStack.top() > i;
}

void Expression::op_le(Operand &i)
{
    nStack.top() = nStack.top() <= i;
}

void Expression::op_ge(Operand &i)
{
    nStack.top() = nStack.top() >= i;
}

void Expression::op_eq(Operand &i)
{
    nStack.top() = nStack.top() == i;
}

void Expression::op_ne(Operand &i)
{
    nStack.top() = nStack.top() != i;
}

void Expression::op_band(Operand &i)
{
    nStack.top() = nStack.top() & i;
}

void Expression::op_bxor(Operand &i)
{
    nStack.top() = nStack.top() ^ i;
}

void Expression::op_bor(Operand &i)
{
    nStack.top() = nStack.top() | i;
}

void Expression::op_and(Operand &i)
{
    nStack.top() = nStack.top() && i;
}

void Expression::op_or(Operand &i)
{
    nStack.top() = nStack.top() || i;
}

void Expression::op_assign(Operand &i)
{
    nStack.top() = nStack.top().setLv(i);
}

void Expression::op_mulas(Operand &i)
{
    nStack.top() = nStack.top().setLv(nStack.top() * i);
}

void Expression::op_divas(Operand &i)
{
    nStack.top() = nStack.top().setLv(nStack.top() / i);
}

void Expression::op_modas(Operand &i)
{
    nStack.top() = nStack.top().setLv(nStack.top() % i);
}

void Expression::op_addas(Operand &i)
{
    nStack.top() = nStack.top().setLv(nStack.top() + i);
}

void Expression::op_subas(Operand &i)
{
    nStack.top() = nStack.top().setLv(nStack.top() - i);
}

void Expression::op_lshfas(Operand &i)
{
    nStack.top() = nStack.top().setLv(nStack.top() << i);
}

void Expression::op_rshfas(Operand &i)
{
    nStack.top() = nStack.top().setLv(nStack.top() >> i);
}

void Expression::op_bandas(Operand &i)
{
    nStack.top() = nStack.top().setLv(nStack.top() & i);
}

void Expression::op_bxoras(Operand &i)
{
    nStack.top() = nStack.top().setLv(nStack.top() ^ i);
}

void Expression::op_boras(Operand &i)
{
    nStack.top() = nStack.top().setLv(nStack.top() | i);
}

void Expression::op_andas(Operand &i)
{
    nStack.top() = nStack.top().setLv(nStack.top() && i);
}

void Expression::op_oras(Operand &i)
{
    nStack.top() = nStack.top().setLv(nStack.top() || i);
}

void Expression::op_lpar(Operand &i)
{
    throw ERR_LACK_RPAR;
}

void Expression::op_lbrk(Operand &i)
{
    throw ERR_LACK_RBRK;
}

void Expression::op_comm(Operand &i)
{
    throw ERR_LACK_RPAR;
}

void Expression::do_lpar()
{
    if (oStack.empty() || oStack.top() != LPAR) {
        throw ERR_LACK_LPAR;
    }
}

void Expression::do_lbrk()
{
    if (oStack.empty() || oStack.top() != LBRK) {
        throw ERR_LACK_LBRK;
    }
    checkOperandsNum(LBRK);
    Operand i = nStack.top();
    nStack.pop();
    nStack.top() = &nStack.top()[i];
}
