#include "operand.h"

#include "error.h"

Operand::Operand() : type(NO), pData(0)
{
}

Operand::Operand(const Operand &obj)
{
    copy(obj);
}

Operand::Operand(const Data &d) : type(RV), pData(new Data(d))
{
}

Operand::Operand(Data *d) : type(LV), pData(d)
{
}

Operand::~Operand()
{
    release();
}

Operand::operator int() const
{
    return int(*pData);
}

Operand::operator const Data &() const
{
    return (const Data &)(*pData);
}

const Operand &Operand::operator=(const Operand &obj)
{
    release();
    copy(obj);
    return *this;
}

const Operand &Operand::operator=(const Data &d)
{
    release();
    type = RV;
    pData = new Data(d);
    return *this;
}

const Operand &Operand::operator=(Data *d)
{
    release();
    type = LV;
    pData = d;
    return *this;
}

Data &Operand::operator[](const Operand &o)
{
    if (o.pData->is<INT>()) {
        return (*pData)[int(*o.pData)];
    } else if (o.pData->is<STR>()) {
        return (*pData)[(const string &)(*o.pData)];
    } else {
        throw ERR_BAD_INDEX;
    }
}

const Data &Operand::setLv(const Data &d)
{
    if (type != LV)
        throw ERR_NOT_LV;
    *pData = d;
    return *pData;
}

bool Operand::isTrue() const
{
    return pData->isTrue();
}

void Operand::copy(const Operand &obj)
{
    type = obj.type;
    if (type == LV) {
        pData = obj.pData;
    } else if (type == RV) {
        pData = new Data(*obj.pData);
    }
}

void Operand::release()
{
    if (type == RV) {
        delete pData;
    }
}

ostream &operator<<(ostream &os, const Operand &obj)
{
    os << *obj.pData;
    return os;
}
