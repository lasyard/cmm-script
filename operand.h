#ifndef _CMM_OPERAND_H_
#define _CMM_OPERAND_H_

#include "data.h"

enum OperandType { NO, LV, RV };

class Operand
{
public:
    Operand() : type(NO), pData(0)
    {
    }

    Operand(const Operand &obj)
    {
        copy(obj);
    }

    Operand(const Data &d) : type(RV), pData(new Data(d))
    {
    }

    Operand(Data *d) : type(LV), pData(d)
    {
    }

    virtual ~Operand()
    {
        release();
    }

    operator int() const
    {
        return int(*pData);
    }

    operator const Data &() const
    {
        return (const Data &)(*pData);
    }

    const Operand &operator=(const Operand &obj)
    {
        release();
        copy(obj);
        return *this;
    }

    const Operand &operator=(const Data &d)
    {
        release();
        type = RV;
        pData = new Data(d);
        return *this;
    }

    const Operand &operator=(Data *d)
    {
        release();
        type = LV;
        pData = d;
        return *this;
    }

    Data &operator[](const Operand &o)
    {
        if (o.pData->is<INT>()) {
            return (*pData)[int(*o.pData)];
        } else if (o.pData->is<STR>()) {
            return (*pData)[(const string &)(*o.pData)];
        } else {
            throw ERR_BAD_INDEX;
        }
    }

    const Data &setLv(const Data &d)
    {
        if (type != LV) {
            throw ERR_NOT_LV;
        }
        *pData = d;
        return *pData;
    }

    bool isTrue() const
    {
        return pData->isTrue();
    }

private:
    enum OperandType type;
    Data *pData;

    void copy(const Operand &obj)
    {
        type = obj.type;
        if (type == LV) {
            pData = obj.pData;
        } else if (type == RV) {
            pData = new Data(*obj.pData);
        }
    }

    void release()
    {
        if (type == RV) {
            delete pData;
        }
    }

    friend ostream &operator<<(ostream &os, const Operand &obj)
    {
        os << *obj.pData;
        return os;
    }
};

#endif /* _CMM_OPERAND_H_ */
