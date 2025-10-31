#ifndef _CMM_SCRIPT_H_
#define _CMM_SCRIPT_H_

#include "expression.h"

class Script
{
public:
    Script();
    virtual ~Script();

    void reset()
    {
        clear();
        clearIds();
    }

    const Data &getVar(const string &name) const
    {
        return ids[name];
    }

    void registerFun(const string &name, FunType *pFun)
    {
        funs[name] = pFun;
    }

    void exec(const char *src);
    void printIds();
    void printFuns();

private:
    enum ExeState { CLEAR, AFT_OPERAND, AFT_OP, AFT_DOT, AFT_FUN, AFT_FUNS };

    struct SigilProcMap {
        char ch;
        int si;
        int ei;
        void (Script::*proc)();
    };

    static const struct SigilProcMap sigilTable[];

    const char *pc;
    enum ExeState state;
    stack<const char *> pcStk;
    Data ids;
    int sigilIndex;
    Expression exp;
    map<string, FunType *> funs;

    void clear();
    void clearIds();
    int parseHexInt();
    int parseBinInt();
    int parseDecInt();
    void parseConst();
    void parseId();
    void parseSigil();

    bool stateOperand();

    void proc_int(int v);
    void proc_id(const string &name);
    void proc_sigil();
    void proc_end();
    void proc_str(const string &str);

    template <enum OpCode op> void proc_op1()
    {
        if (stateOperand()) {
            exp.pushOp(op);
        } else {
            throw ERR_NXP_OP;
        }
        state = AFT_OP;
    }

    template <enum OpCode op> void proc_op2()
    {
        if (state == AFT_OPERAND) {
            exp.pushOp(op);
        } else {
            throw ERR_NXP_OP;
        }
        state = AFT_OP;
    }

    template <enum OpCode op1, enum OpCode op2> void proc_op2()
    {
        if (state == AFT_OPERAND) {
            exp.pushOp(op2);
        } else if (stateOperand()) {
            exp.pushOp(op1);
        } else {
            throw ERR_NXP_OP;
        }
        state = AFT_OP;
    }

    void proc_lpar();
    void proc_rpar();
    void proc_lbrc();
    void proc_rbrc();
    void proc_rbrk();
    void proc_sharp();
    void proc_ques();
    void proc_colo();
    void proc_quot();
    void proc_dot();
    void proc_note();

    void bypassBranch();
    void exitLoop();
};

#endif /* _CMM_SCRIPT_H_ */
