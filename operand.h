#ifndef _OPERAND_H_
#define _OPERAND_H_

#include "data.h"

enum OperandType { NO, LV, RV };

class Operand
{
public:
    Operand();
    Operand(const Operand &obj);
    Operand(const Data &d);
    Operand(Data *d);
    virtual ~Operand();

    operator int();
    operator const Data &();
    const Operand &operator=(const Operand &obj);
    const Operand &operator=(const Data &d);
    const Operand &operator=(Data *d);
    Data &operator[](const Operand &o);

    const Data &setLv(const Data &d);
    bool isTrue() const;

private:
    enum OperandType type;
    Data *pData;

    void copy(const Operand &obj);
    void release();

    friend ostream &operator<<(ostream &os, const Operand &obj);
};

#endif
