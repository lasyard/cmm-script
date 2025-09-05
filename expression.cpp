#include "expression.h"

const struct Expression::OpInfo Expression::ops[] = {
    {  0, 0, L2R,                      0},
    {  1, 1, R2L,    &Expression::op_inc},
    {  1, 1, R2L,    &Expression::op_dec},
    {  1, 1, R2L,   &Expression::op_binv},
    {  1, 1, R2L,    &Expression::op_pos},
    {  1, 1, R2L,    &Expression::op_neg},
    {  1, 1, R2L,    &Expression::op_not},
    {  2, 2, L2R,    &Expression::op_mul},
    {  2, 2, L2R,    &Expression::op_div},
    {  2, 2, L2R,    &Expression::op_mod},
    {  3, 2, L2R,    &Expression::op_add},
    {  3, 2, L2R,    &Expression::op_sub},
    {  4, 2, L2R,   &Expression::op_lshf},
    {  4, 2, L2R,   &Expression::op_rshf},
    {  5, 2, L2R,     &Expression::op_lt},
    {  5, 2, L2R,     &Expression::op_gt},
    {  5, 2, L2R,     &Expression::op_le},
    {  5, 2, L2R,     &Expression::op_ge},
    {  6, 2, L2R,     &Expression::op_eq},
    {  6, 2, L2R,     &Expression::op_ne},
    {  7, 2, L2R,   &Expression::op_band},
    {  8, 2, L2R,   &Expression::op_bxor},
    {  9, 2, L2R,    &Expression::op_bor},
    { 10, 2, L2R,    &Expression::op_and},
    { 11, 2, L2R,     &Expression::op_or},
    { 12, 2, R2L, &Expression::op_assign},
    { 12, 2, R2L,  &Expression::op_mulas},
    { 12, 2, R2L,  &Expression::op_divas},
    { 12, 2, R2L,  &Expression::op_modas},
    { 12, 2, R2L,  &Expression::op_addas},
    { 12, 2, R2L,  &Expression::op_subas},
    { 12, 2, R2L, &Expression::op_lshfas},
    { 12, 2, R2L, &Expression::op_rshfas},
    { 12, 2, R2L, &Expression::op_bandas},
    { 12, 2, R2L, &Expression::op_bxoras},
    { 12, 2, R2L,  &Expression::op_boras},
    { 12, 2, R2L,  &Expression::op_andas},
    { 12, 2, R2L,   &Expression::op_oras},
    { 99, 0, R2L,   &Expression::op_lpar},
    { 99, 0, R2L,                      0},
    { 99, 2, R2L,   &Expression::op_lbrk},
    { 99, 0, R2L,                      0},
    { 99, 0, R2L,   &Expression::op_comm},
    { 99, 0, R2L,                      0},
    { 99, 0, R2L,                      0},
    {100, 0, R2L,                      0}, // END
};

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
    f(m_context, r, p);
    nStack.push(r);
    oStack.pop();
    fStack.pop();
}

void Expression::op_inc([[maybe_unused]] const Operand &i)
{
    nStack.top() = nStack.top().setLv(nStack.top() + 1);
}

void Expression::op_dec([[maybe_unused]] const Operand &i)
{
    nStack.top() = nStack.top().setLv(nStack.top() - 1);
}

void Expression::op_binv([[maybe_unused]] const Operand &i)
{
    nStack.top() = ~nStack.top();
}

void Expression::op_pos([[maybe_unused]] const Operand &i)
{
}

void Expression::op_neg([[maybe_unused]] const Operand &i)
{
    nStack.top() = -nStack.top();
}

void Expression::op_not([[maybe_unused]] const Operand &i)
{
    nStack.top() = !nStack.top();
}

void Expression::op_mul(const Operand &i)
{
    nStack.top() = nStack.top() * i;
}

void Expression::op_div(const Operand &i)
{
    if (i == 0) {
        throw ERR_DIV_BY_ZERO;
    }
    nStack.top() = nStack.top() / i;
}

void Expression::op_mod(const Operand &i)
{
    if (i == 0) {
        throw ERR_DIV_BY_ZERO;
    }
    nStack.top() = nStack.top() % i;
}

void Expression::op_add(const Operand &i)
{
    nStack.top() = nStack.top() + i;
}

void Expression::op_sub(const Operand &i)
{
    nStack.top() = nStack.top() - i;
}

void Expression::op_lshf(const Operand &i)
{
    nStack.top() = nStack.top() << i;
}

void Expression::op_rshf(const Operand &i)
{
    nStack.top() = nStack.top() >> i;
}

void Expression::op_lt(const Operand &i)
{
    nStack.top() = nStack.top() < i;
}

void Expression::op_gt(const Operand &i)
{
    nStack.top() = nStack.top() > i;
}

void Expression::op_le(const Operand &i)
{
    nStack.top() = nStack.top() <= i;
}

void Expression::op_ge(const Operand &i)
{
    nStack.top() = nStack.top() >= i;
}

void Expression::op_eq(const Operand &i)
{
    nStack.top() = nStack.top() == i;
}

void Expression::op_ne(const Operand &i)
{
    nStack.top() = nStack.top() != i;
}

void Expression::op_band(const Operand &i)
{
    nStack.top() = nStack.top() & i;
}

void Expression::op_bxor(const Operand &i)
{
    nStack.top() = nStack.top() ^ i;
}

void Expression::op_bor(const Operand &i)
{
    nStack.top() = nStack.top() | i;
}

void Expression::op_and(const Operand &i)
{
    nStack.top() = nStack.top() && i;
}

void Expression::op_or(const Operand &i)
{
    nStack.top() = nStack.top() || i;
}

void Expression::op_assign(const Operand &i)
{
    nStack.top() = nStack.top().setLv(i);
}

void Expression::op_mulas(const Operand &i)
{
    nStack.top() = nStack.top().setLv(nStack.top() * i);
}

void Expression::op_divas(const Operand &i)
{
    nStack.top() = nStack.top().setLv(nStack.top() / i);
}

void Expression::op_modas(const Operand &i)
{
    nStack.top() = nStack.top().setLv(nStack.top() % i);
}

void Expression::op_addas(const Operand &i)
{
    nStack.top() = nStack.top().setLv(nStack.top() + i);
}

void Expression::op_subas(const Operand &i)
{
    nStack.top() = nStack.top().setLv(nStack.top() - i);
}

void Expression::op_lshfas(const Operand &i)
{
    nStack.top() = nStack.top().setLv(nStack.top() << i);
}

void Expression::op_rshfas(const Operand &i)
{
    nStack.top() = nStack.top().setLv(nStack.top() >> i);
}

void Expression::op_bandas(const Operand &i)
{
    nStack.top() = nStack.top().setLv(nStack.top() & i);
}

void Expression::op_bxoras(const Operand &i)
{
    nStack.top() = nStack.top().setLv(nStack.top() ^ i);
}

void Expression::op_boras(const Operand &i)
{
    nStack.top() = nStack.top().setLv(nStack.top() | i);
}

void Expression::op_andas(const Operand &i)
{
    nStack.top() = nStack.top().setLv(nStack.top() && i);
}

void Expression::op_oras(const Operand &i)
{
    nStack.top() = nStack.top().setLv(nStack.top() || i);
}

void Expression::op_lpar([[maybe_unused]] const Operand &i)
{
    throw ERR_LACK_RPAR;
}

void Expression::op_lbrk([[maybe_unused]] const Operand &i)
{
    throw ERR_LACK_RBRK;
}

void Expression::op_comm([[maybe_unused]] const Operand &i)
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
