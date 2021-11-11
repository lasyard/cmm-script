#ifndef _DATA_H_
#define _DATA_H_

#include <iostream>
#include <map>
#include <string>
#include <vector>

using namespace std;

enum DataType { UNDEF, INT, STR, VEC, MAP };

class Data
{
public:
    Data();
    Data(int v);
    Data(const string &s);
    Data(const Data &obj);
    virtual ~Data();

    operator int() const;
    operator const string &() const;
    const Data &operator=(const Data &obj);
    Data &operator[](int i);
    Data &operator[](const string &name);

    void newVec();
    void newMap();
    bool isTrue() const;
    bool exists(const string &name) const;

    template <enum DataType t> bool is() const
    {
        return type == t;
    }

private:
    static const char *const TYPE_ERROR[];

    enum DataType type;
    union {
        int value;
        string *pStr;
        vector<Data> *pVec;
        map<string, Data> *pMap;
    };

    void assertNotUndef() const;

    template <enum DataType t> void assertType() const
    {
        assertNotUndef();
        if (type != t) {
            throw TYPE_ERROR[t];
        }
    }

    void copy(const Data &obj);
    void release();

    friend ostream &operator<<(ostream &os, const Data &obj);
};

#endif
