#include <iomanip>
#include <iostream>

#include "funs.h"

using namespace std;

void f__p(Script *sc, Operand &r, deque<Operand> &p)
{
    deque<Operand>::const_iterator i;
    for (i = p.begin(); i != p.end(); ++i) {
        cout << *i << " ";
    }
}

void f__pw(Script *sc, Operand &r, deque<Operand> &p)
{
    deque<Operand>::const_iterator i;
    int w = p.front();
    p.pop_front();
    for (i = p.begin(); i != p.end(); ++i) {
        cout << setw(w) << *i;
    }
}

void f__pln(Script *sc, Operand &r, deque<Operand> &p)
{
    cout << endl;
}
