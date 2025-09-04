#ifndef _FUNS_H_
#define _FUNS_H_

#include <deque>

#include "operand.h"

using namespace std;

class Script;

typedef void FunType([[maybe_unused]] Script *sc, Operand &r, deque<Operand> &p);

void f__p(Script *sc, Operand &r, deque<Operand> &p);
void f__pw(Script *sc, Operand &r, deque<Operand> &p);
void f__pln(Script *sc, Operand &r, deque<Operand> &p);

#endif
