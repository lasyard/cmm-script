#include "data.h"
#include "error.h"

const char *const Data::TYPE_ERROR[] = {0, ERR_NOT_INT, ERR_NOT_STR, ERR_NOT_VEC, ERR_NOT_MAP};

Data::Data() : type(UNDEF)
{
}

Data::Data(int v) : type(INT)
{
    value = v;
}

Data::Data(const string &s) : type(STR)
{
    pStr = new string(s);
}

Data::Data(const Data &obj)
{
    copy(obj);
}

Data::~Data()
{
    release();
}

Data::operator int() const
{
    assertType<INT>();
    return value;
}

Data::operator const string &() const
{
    assertType<STR>();
    return *pStr;
}

const Data &Data::operator=(const Data &obj)
{
    release();
    copy(obj);
    return *this;
}

Data &Data::operator[](int i)
{
    assertType<VEC>();
    while (pVec->size() <= (vector<Data>::size_type)i) {
        pVec->push_back(0);
    }
    return (*pVec)[i];
}

Data &Data::operator[](const string &name)
{
    assertType<MAP>();
    return (*pMap)[name];
}

void Data::newVec()
{
    release();
    type = VEC;
    pVec = new vector<Data>;
}

void Data::newMap()
{
    release();
    type = MAP;
    pMap = new map<string, Data>;
}

bool Data::isTrue() const
{
    if (type == UNDEF) {
        return false;
    }
    if (type == INT && value == 0) {
        return false;
    }
    return true;
}

bool Data::exists(const string &name) const
{
    if (type != MAP) {
        return false;
    }
    if (pMap->find(name) == pMap->end()) {
        return false;
    }
    return true;
}

void Data::assertNotUndef() const
{
    if (type == UNDEF) {
        throw ERR_UNDEF;
    }
}

void Data::copy(const Data &obj)
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

void Data::release()
{
    if (type == STR) {
        delete pStr;
    } else if (type == VEC) {
        delete pVec;
    } else if (type == MAP) {
        delete pMap;
    }
}

ostream &operator<<(ostream &os, const Data &obj)
{
    if (obj.type == UNDEF) {
        os << "UNDEF";
    } else if (obj.type == INT) {
        os << obj.value;
    } else if (obj.type == STR) {
        os << obj.pStr->c_str();
    } else if (obj.type == VEC) {
        vector<Data>::const_iterator i;
        os << "VEC(";
        for (i = obj.pVec->begin(); i != obj.pVec->end(); ++i) {
            os << *i << ", ";
        }
        os << ")";
    } else if (obj.type == MAP) {
        map<string, Data>::const_iterator i;
        os << "MAP(";
        for (i = obj.pMap->begin(); i != obj.pMap->end(); ++i) {
            os << i->first.c_str() << ": " << i->second << ", ";
        }
        os << ")";
    }
    return os;
}
