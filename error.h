#ifndef _CMM_ERROR_H_
#define _CMM_ERROR_H_

const char *const ERR_UNDEF = "Undefined var";
const char *const ERR_NOT_INT = "Var is not an integer";
const char *const ERR_NOT_STR = "Var is not a string";
const char *const ERR_NOT_VEC = "Var is not a vector";
const char *const ERR_NOT_MAP = "Var is not a map";
const char *const ERR_LACK_LPAR = "Lack left parenthesis";
const char *const ERR_LACK_RPAR = "Lack right parenthesis";
const char *const ERR_LACK_LBRK = "Lack left bracket";
const char *const ERR_LACK_RBRK = "Lack right bracket";
const char *const ERR_LACK_OPEE = "Lack operand";
const char *const ERR_DIV_BY_ZERO = "Devided by zero";
const char *const ERR_BAD_HEX_INT = "Invalid character in hex int const";
const char *const ERR_BAD_BIN_INT = "Invalid character in binary int const";
const char *const ERR_NXP_CONST = "Unexpected const";
const char *const ERR_NXP_NAME = "Unexpected name";
const char *const ERR_NXP_SIGIL = "Unexpected sigil";
const char *const ERR_NXP_OP = "Unexpected operator";
const char *const ERR_NOT_END = "Statement is not ended";
const char *const ERR_BAD_LOOP = "Malformed loop structure";
const char *const ERR_BAD_BRANCH = "Malformed branch";
const char *const ERR_STR_NOT_END = "String is not ended";
const char *const ERR_NOTE_NOT_END = "Note is not ended";
const char *const ERR_NOT_LV = "L-value required";
const char *const ERR_BAD_INDEX = "Index must be an integer or a string";

#endif /* _CMM_ERROR_H_ */
