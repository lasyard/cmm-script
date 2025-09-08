#include <iomanip>
#include <iostream>

#include "funs.h"

using namespace std;

void f_p([[maybe_unused]] void *context, Operand &r, deque<Operand> &p)
{
    deque<Operand>::const_iterator i;
    int count = 0;
    for (i = p.begin(); i != p.end(); ++i) {
        cout << *i;
        ++count;
    }
    r = count;
}

void f_pln(void *context, Operand &r, deque<Operand> &p)
{
    f_p(context, r, p);
    cout << endl;
}

void f_pw([[maybe_unused]] void *context, [[maybe_unused]] Operand &r, deque<Operand> &p)
{
    deque<Operand>::const_iterator i;
    int w = p.front();
    p.pop_front();
    cout << setw(w);
    f_p(context, r, p);
}
