#include "script.h"

/* clang-format off */
const struct Script::SigilProcMap Script::sigilTable[] = {
    {   0,  1, 29, 0}, // Root (0)
    { '=', 29, 30, &Script::proc_op2<ASSIGN>}, // Level 1 (1)
    { '<', 30, 32, &Script::proc_op2<LT>},
    { '>', 32, 34, &Script::proc_op2<GT>},
    { '!', 34, 35, &Script::proc_op1<NOT>},
    { '&', 35, 37, &Script::proc_op2<BAND>},
    { '|', 37, 39, &Script::proc_op2<BOR>},
    { '+', 39, 41, &Script::proc_op2<POS, ADD>},
    { '-', 41, 43, &Script::proc_op2<NEG, SUB>},
    { '*', 43, 45, &Script::proc_op2<MUL>},
    { '/', 45, 47, &Script::proc_op2<DIV>},
    { '%', 47, 48, &Script::proc_op2<MOD>},
    { '(',  0,  0, &Script::proc_lpar},
    { ')',  0,  0, &Script::proc_rpar},
    { '~',  0,  0, &Script::proc_op1<BINV>},
    { '^', 48, 49, &Script::proc_op2<BXOR>},
    { '?',  0,  0, &Script::proc_ques},
    { '#',  0,  0, &Script::proc_sharp},
    { '$',  0,  0, &Script::proc_end},
    { ':',  0,  0, &Script::proc_colo},
    { '"',  0,  0, &Script::proc_quot},
    {'\'',  0,  0, 0},
    { ',',  0,  0, &Script::proc_op2<COMM>},
    { '.',  0,  0, &Script::proc_dot},
    { '[',  0,  0, &Script::proc_op2<LBRK>},
    { ']',  0,  0, &Script::proc_rbrk},
    { '{',  0,  0, &Script::proc_lbrc},
    { '}',  0,  0, &Script::proc_rbrc},
    { ';',  0,  0, &Script::proc_end},
    { '=',  0,  0, &Script::proc_op2<EQ>}, // Level 2 (29)
    { '=',  0,  0, &Script::proc_op2<LE>},
    { '<', 49, 50, &Script::proc_op2<LSHF>},
    { '=',  0,  0, &Script::proc_op2<GE>},
    { '>', 50, 51, &Script::proc_op2<RSHF>},
    { '=',  0,  0, &Script::proc_op2<NE>},
    { '&', 51, 52, &Script::proc_op2<AND>},
    { '=',  0,  0, &Script::proc_op2<BANDAS>},
    { '|', 52, 53, &Script::proc_op2<OR>},
    { '=',  0,  0, &Script::proc_op2<BORAS>},
    { '+',  0,  0, &Script::proc_op1<INC>},
    { '=',  0,  0, &Script::proc_op2<ADDAS>},
    { '-',  0,  0, &Script::proc_op1<DEC>},
    { '=',  0,  0, &Script::proc_op2<SUBAS>},
    { '=',  0,  0, &Script::proc_op2<MULAS>},
    { '/',  0,  0, 0},
    { '*',  0,  0, &Script::proc_note},
    { '=',  0,  0, &Script::proc_op2<DIVAS>},
    { '=',  0,  0, &Script::proc_op2<MODAS>},
    { '=',  0,  0, &Script::proc_op2<BXORAS>},
    { '=',  0,  0, &Script::proc_op2<LSHFAS>}, // Level 3 (49)
    { '=',  0,  0, &Script::proc_op2<RSHFAS>},
    { '=',  0,  0, &Script::proc_op2<ANDAS>},
    { '=',  0,  0, &Script::proc_op2<ORAS>},
};
/* clang-format on */

static bool isLineEnd(int c)
{
    return c == '\r' || c == '\n';
}

static bool isSpace(int c)
{
    return c == ' ' || c == '\t';
}

static bool isNum(int c)
{

    return '0' <= c && c <= '9';
}

static bool isHexNum(int c)
{
    return ('a' <= c && c <= 'f') || ('A' <= c && c <= 'F');
}

static bool isAlpha(int c)
{
    return ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z');
}

static bool isNameInit(int c)
{
    return isAlpha(c) || c == '_' || c == '@' || c == '\\';
}

static bool isName(int c)
{
    return isNameInit(c) || isNum(c);
}

const struct Script::FunMap Script::funTable[] = {
    {"_p", f__p},
    {"_pw", f__pw},
    {"_pln", f__pln},
};

Script::Script()
{
    addFuns();
    exp.bind(this);
    reset();
}

Script::~Script()
{
}

void Script::exec(const char *src)
{
    pc = src;
    try {
        while (true) {
            if (pc[0] == '\0') {
                break;
            }
            if (isLineEnd(pc[0])) {
                proc_end();
                pc++;
            } else if (isSpace(pc[0])) {
                pc++;
            } else if (isNum(pc[0])) {
                parseConst();
            } else if (isNameInit(pc[0])) {
                parseName();
            } else {
                parseSigil();
            }
        }
        proc_end();
    } catch (const char *err) {
        cerr << err << ": " << pc << endl;
        clear();
    }
}

void Script::reset()
{
    clear();
    clearIds();
}

void Script::printIds()
{
    cout << ids << endl;
}

void Script::printFuns()
{
    map<string, FunType *>::const_iterator i;
    for (i = funs.begin(); i != funs.end(); ++i) {
        cout << i->first << endl;
    }
}

void Script::clear()
{
    while (!pcStk.empty()) {
        pcStk.pop();
    }
    sigilIndex = 0;
    exp.clear();
    state = CLEAR;
}

void Script::clearIds()
{
    ids.newMap();
    addSpecialVars();
}

int Script::parseHexInt()
{
    int value = 0;
    bool ok = false;
    while (true) {
        if (isNum(pc[0])) {
            value <<= 4;
            value += pc[0] & 0x0F;
            ok = true;
        } else if (isHexNum(pc[0])) {
            value <<= 4;
            value += (pc[0] & 0x0F) + 9;
            ok = true;
        } else {
            break;
        }
        pc++;
    }
    if (!ok) {
        throw ERR_BAD_HEX_INT;
    }
    return value;
}

int Script::parseBinInt()
{
    int value = 0;
    bool ok = false;
    while (true) {
        if (pc[0] == '1') {
            value <<= 1;
            value++;
            ok = true;
        } else if (pc[0] == '0') {
            ok = true;
        } else {
            break;
        }
        pc++;
    }
    if (!ok) {
        throw ERR_BAD_BIN_INT;
    }
    return value;
}

int Script::parseDecInt()
{
    int value = 0;
    while (true) {
        if (isNum(pc[0])) {
            value *= 10;
            value += pc[0] & 0x0F;
        } else {
            break;
        }
        pc++;
    }
    return value;
}

void Script::parseConst()
{
    if (pc[0] == '0' && (pc[1] == 'x' || pc[1] == 'X')) {
        pc += 2;
        proc_int(parseHexInt());
    } else if (pc[0] == '0' && (pc[1] == 'b' || pc[1] == 'B')) {
        pc += 2;
        proc_int(parseBinInt());
    } else {
        proc_int(parseDecInt());
    }
}

void Script::parseName()
{
    const char *name = pc;
    while (isName(pc[0])) {
        pc++;
    }
    proc_name(string(name, pc - name));
}

void Script::parseSigil()
{
    while (true) {
        int i;
        for (i = sigilTable[sigilIndex].si; i < sigilTable[sigilIndex].ei; i++) {
            if (pc[0] == sigilTable[i].ch) {
                break;
            }
        }
        if (i < sigilTable[sigilIndex].ei) {
            sigilIndex = i;
        } else {
            if (sigilIndex != 0) {
                proc_sigil();
            }
            break;
        }
        pc++;
    }
}

bool Script::stateOperand()
{
    return state == CLEAR || state == AFT_OP || state == AFT_FUNS;
}

void Script::proc_int(int v)
{
    if (stateOperand()) {
        exp.push(Data(v));
    } else {
        throw ERR_NXP_CONST;
    }
    state = AFT_OPERAND;
}

void Script::proc_name(const string &name)
{
    if (state == AFT_DOT) {
        exp.pushOp(LBRK);
        proc_str(name);
        exp.pushOp(RBRK);
        state = AFT_OPERAND;
    } else if (stateOperand()) {
        if (funs.find(name) != funs.end()) {
            exp.pushFun(funs[name]);
            state = AFT_FUN;
        } else {
            if (!ids.exists(name))
                ids[name] = Data();
            exp.push(&ids[name]);
            state = AFT_OPERAND;
        }
    } else {
        throw ERR_NXP_NAME;
    }
}

void Script::proc_sigil()
{
    if (sigilTable[sigilIndex].proc != 0) {
        (this->*sigilTable[sigilIndex].proc)();
    } else {
        throw ERR_NXP_SIGIL;
    }
    sigilIndex = 0;
}

void Script::proc_end()
{
    if (state == AFT_OPERAND) {
        exp.pushOp(END);
        exp.clear();
    } else if (state != CLEAR) {
        throw ERR_NOT_END;
    }
    state = CLEAR;
}

void Script::proc_str(const string &str)
{
    exp.push(Data(str));
    state = AFT_OPERAND;
}

void Script::proc_lpar()
{
    if (stateOperand()) {
        exp.pushOp(LPAR);
        state = AFT_OP;
    } else if (state == AFT_FUN) {
        state = AFT_FUNS;
    } else {
        throw ERR_NXP_SIGIL;
    }
}

void Script::proc_rpar()
{
    if (state == AFT_OPERAND) {
        exp.pushOp(RPAR);
    } else if (state == AFT_FUNS) {
        exp.pushOp(FUNE);
    } else {
        throw ERR_NXP_SIGIL;
    }
    state = AFT_OPERAND;
}

void Script::proc_lbrc()
{
    proc_end();
    pcStk.push(pc);
}

void Script::proc_rbrc()
{
    proc_end();
    if (pcStk.empty()) {
        throw ERR_BAD_LOOP;
    }
    pc = pcStk.top();
}

void Script::proc_rbrk()
{
    if (state == AFT_OPERAND) {
        exp.pushOp(RBRK);
    } else {
        throw ERR_NXP_SIGIL;
    }
    state = AFT_OPERAND;
}

void Script::proc_sharp()
{
    proc_end();
    if (pcStk.empty()) {
        throw ERR_BAD_LOOP;
    }
    pcStk.pop();
    exitLoop();
}

void Script::proc_ques()
{
    if (state == AFT_OPERAND) {
        exp.pushOp(END);
        if (!exp.value().isTrue()) {
            bypassBranch();
        }
        exp.clear();
    } else {
        throw ERR_NXP_SIGIL;
    }
    state = CLEAR;
}

void Script::proc_colo()
{
    proc_end();
    bypassBranch();
}

void Script::proc_quot()
{
    const char *str = pc;
    while (pc[0] != '\"' && pc[0] != '\0') {
        pc++;
    }
    if (pc[0] == '\0') {
        throw ERR_STR_NOT_END;
    }
    proc_str(string(str, pc - str));
    pc++;
}

void Script::proc_dot()
{
    state = AFT_DOT;
}

void Script::proc_note()
{
    while (!(pc[0] == '*' && pc[1] == '/') && pc[0] != '\0') {
        pc++;
    }
    if (pc[0] == '\0') {
        throw ERR_NOTE_NOT_END;
    }
    pc += 2;
}

void Script::bypassBranch()
{
    int n;
    for (n = 0; (pc[0] != ':' && pc[0] != '$' && pc[0] != '\0') || n > 0; pc++) {
        if (pc[0] == '?') {
            n++;
        } else if (pc[0] == '$') {
            n--;
        }
    }
    if (pc[0] == '\0') {
        throw ERR_BAD_BRANCH;
    }
    pc++;
}

void Script::exitLoop()
{
    int n;
    for (n = 1; pc[0] != '\0'; pc++) {
        if (pc[0] == '{') {
            n++;
        } else if (pc[0] == '}') {
            n--;
            if (n == 0) {
                break;
            }
        }
    }
    if (pc[0] == '\0') {
        throw ERR_BAD_LOOP;
    }
    pc++;
}

void Script::addSpecialVars()
{
    string var;
    var = "@";
    ids[var] = Data();
    ids[var].newVec();
    var = "\\";
    ids[var] = Data();
    ids[var].newMap();
}

void Script::addFuns()
{
    unsigned int i;
    for (i = 0; i < sizeof(funTable) / sizeof(struct FunMap); i++) {
        funs[funTable[i].name] = funTable[i].pFun;
    }
}
