#ifndef OPERATIONS_H
#define OPERATIONS_H

#include <stdio.h>
#include "tree.h"


TreeElem_t Add(TreeElem_t arg1, TreeElem_t arg2);
TreeElem_t Sub(TreeElem_t arg1, TreeElem_t arg2);
TreeElem_t Mul(TreeElem_t arg1, TreeElem_t arg2);
TreeElem_t Div(TreeElem_t arg1, TreeElem_t arg2);

TreeElem_t Deg(TreeElem_t arg1, TreeElem_t arg2);
TreeElem_t Ln (TreeElem_t arg1, TreeElem_t arg2);
TreeElem_t Log(TreeElem_t arg1, TreeElem_t arg2);

TreeElem_t Sin(TreeElem_t arg1, TreeElem_t arg2);
TreeElem_t Cos(TreeElem_t arg1, TreeElem_t arg2);
TreeElem_t Tan(TreeElem_t arg1, TreeElem_t arg2);

enum FuncEntryForm
{
    PREFIX,
    INFIX
};

enum FuncType
{
    UNARY,
    BINARY
};

struct Operation
{
    const int    num;
    const char  *symbol;

    const FuncType      type;                                               // UNARY / BINARY
    const FuncEntryForm form;                                               // PREFIX / INFIX в реальности

    TreeElem_t  (*op_func)     (TreeElem_t arg1, TreeElem_t arg2);
};

struct ManageElem
{
    const int   name;
    const char *symbol;
};

enum Operation_enum
{
    ADD,
    SUB,
    MUL,
    DIV,
    DEG,

    LN,
    LOG,

    SIN,
    COS,
    TAN
};

enum ManagerElems_enum
{
    OPEN_BRACKET,
    CLOSE_BRACKET,
    COMMA,
    EXPR_END
};

const int OPERATIONS_NUM = 10;

const Operation Operations[OPERATIONS_NUM] = 
{
    { ADD, "с",    BINARY, INFIX,  Add },
    { SUB, "без",  BINARY, INFIX,  Sub },
    { MUL, "множ", BINARY, INFIX,  Mul },
    { DIV, "дел",  BINARY, INFIX,  Div },
    { DEG, "степ", BINARY, INFIX,  Deg },

    { LN,  "лн",   UNARY,  PREFIX, Ln  },
    { LOG, "лог",  BINARY, PREFIX, Log },

    { SIN, "син",  UNARY,  PREFIX, Sin },
    { COS, "кос",  UNARY,  PREFIX, Cos },
    { TAN, "тан",  UNARY,  PREFIX, Tan }
};

const int MANAGE_ELEMS_NUM = 4;

const ManageElem ManagerElems[MANAGE_ELEMS_NUM] = 
{
    { OPEN_BRACKET,  "("  },
    { CLOSE_BRACKET, ")"  },
    { COMMA,         ","  },
    { EXPR_END,      "\n" }
};

// const Operation  *GetOperationByNode   (Node *node);
// const Operation  *GetOperationByNum    (int num);
const Operation  *GetOperationBySymbol (char *sym);
const ManageElem *GetManageElemBySymbol(char *sym);

#endif