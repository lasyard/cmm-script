#ifndef _CMM_DATA_H_
#define _CMM_DATA_H_

#include <cstdint>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "error.h"

using namespace std;

enum DataType { UNDEF, INT, STR, VEC, MAP };

class Data
{
public:
    Data() : type(UNDEF)
    {
    }

    Data(int v) : type(INT)
    {
        value = v;
    }

    Data(const string &s) : type(STR)
    {
        pStr = new string(s);
    }

    Data(const Data &obj)
    {
        copy(obj);
    }

    virtual ~Data()
    {
        release();
    }

    template <enum DataType t> void assertType() const
    {
        assertNotUndef();
        if (type != t) {
            throw TYPE_ERROR[t];
        }
    }

    operator int() const
    {
        assertType<INT>();
        return value;
    }

    operator const string &() const
    {
        assertType<STR>();
        return *pStr;
    }

    const Data &operator=(const Data &obj)
    {
        release();
        copy(obj);
        return *this;
    }

    const Data &operator[](int i) const
    {
        assertType<VEC>();
        while (pVec->size() <= (vector<Data>::size_type)i) {
            pVec->push_back(0);
        }
        return (*pVec)[i];
    }

    Data &operator[](int i)
    {
        return const_cast<Data &>(std::as_const(*this)[i]);
    }

    const Data &operator[](const string &name) const
    {
        assertType<MAP>();
        return (*pMap)[name];
    }

    Data &operator[](const string &name)
    {
        return const_cast<Data &>(std::as_const(*this)[name]);
    }

    void newVec()
    {
        release();
        type = VEC;
        pVec = new vector<Data>;
    }

    void newMap()
    {
        release();
        type = MAP;
        pMap = new map<string, Data>;
    }

    bool isTrue() const
    {
        if (type == UNDEF) {
            return false;
        }
        if (type == INT && value == 0) {
            return false;
        }
        return true;
    }

    bool exists(const string &name) const
    {
        if (type != MAP) {
            return false;
        }
        if (pMap->find(name) == pMap->end()) {
            return false;
        }
        return true;
    }

    template <enum DataType t> bool is() const
    {
        return type == t;
    }

private:
    static const char *const TYPE_ERROR[];

    enum DataType type;

    union {
        int64_t value;
        string *pStr;
        vector<Data> *pVec;
        map<string, Data> *pMap;
    };

    void assertNotUndef() const
    {
        if (type == UNDEF) {
            throw ERR_UNDEF;
        }
    }

    void copy(const Data &obj)
    {
        type = obj.type;
        if (type == INT) {
            value = obj.value;
        } else if (type == STR) {
            pStr = new string(*obj.pStr);
        } else if (type == VEC) {
            pVec = new vector<Data>(*obj.pVec);
        } else if (type == MAP) {
            pMap = new map<string, Data>(*obj.pMap);
        }
    }

    void release()
    {
        if (type == STR) {
            delete pStr;
        } else if (type == VEC) {
            delete pVec;
        } else if (type == MAP) {
            delete pMap;
        }
    }

    friend ostream &operator<<(ostream &os, const Data &obj);
};

#endif /* _CMM_DATA_H_ */
