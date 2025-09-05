#include "data.h"

#include "error.h"

const char *const Data::TYPE_ERROR[] = {0, ERR_NOT_INT, ERR_NOT_STR, ERR_NOT_VEC, ERR_NOT_MAP};

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
