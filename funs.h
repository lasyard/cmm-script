#ifndef _CMM_FUNS_H_
#define _CMM_FUNS_H_

#include <deque>

#include "operand.h"

using namespace std;

typedef void FunType(
    void *context,
    Operand &r,       // return value
    deque<Operand> &p // parameters
);

void f_p(void *context, Operand &r, deque<Operand> &p);
void f_pln(void *context, Operand &r, deque<Operand> &p);
void f_pw(void *context, Operand &r, deque<Operand> &p);

#endif /* _CMM_FUNS_H_ */
