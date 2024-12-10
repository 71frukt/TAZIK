#ifndef OPERATIONS_H
#define OPERATIONS_H

#include <stdio.h>

#include "tree_elem_t_info.h"

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

enum Operation_enum
{
    ASSIGN,
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

struct Operation
{
    const Operation_enum num;
    const char          *symbol;

    const FuncType      type;                                               // UNARY / BINARY
    const FuncEntryForm form;                                               // PREFIX / INFIX в реальности

    TreeElem_t  (*op_func)     (TreeElem_t arg1, TreeElem_t arg2);
};

const int OPERATIONS_NUM = 11;

const Operation Operations[OPERATIONS_NUM] = 
{
    { ASSIGN, "=",    BINARY, INFIX,  NULL },
    { ADD,    "+",    BINARY, INFIX,  Add },
    { SUB,    "-",    BINARY, INFIX,  Sub },
    { MUL,    "*",    BINARY, INFIX,  Mul },
    { DIV,    "/",    BINARY, INFIX,  Div },
    { DEG,    "**",   BINARY, INFIX,  Deg },

    { LN,     "лн",   UNARY,  PREFIX, Ln  },
    { LOG,    "лог",  BINARY, PREFIX, Log },

    { SIN,    "син",  UNARY,  PREFIX, Sin },
    { COS,    "кос",  UNARY,  PREFIX, Cos },
    { TAN,    "тан",  UNARY,  PREFIX, Tan }
};

//------------------------------------------------------------------------------------------------------------//

enum Type
{
    INT_T,
    DOUBLE_T
};

struct Initializer
{
    Type type;
    const char *symbol;
};

const int INITIALIZERS_NUM = 2;

const Initializer Initializers[INITIALIZERS_NUM] =
{
    { INT_T,    "инт" },
    { DOUBLE_T, "ссср"  }
};

//------------------------------------------------------------------------------------------------------------//

enum Managers_enum
{
    OPEN_BRACKET,
    CLOSE_BRACKET,
    COMMA,
};

struct ManageElem
{
    const Managers_enum  name;
    const char          *symbol;
};

const int MANAGE_ELEMS_NUM = 3;

const ManageElem Managers[MANAGE_ELEMS_NUM] = 
{
    { OPEN_BRACKET,  "("  },
    { CLOSE_BRACKET, ")"  },
    { COMMA,         ","  },
};

//---------------------------------------------------------------------------------------------------------------//

const char *const NEW_EXPR_SYMBOL = ";";

//---------------------------------------------------------------------------------------------------------------//

enum NamedNode
{
    FUNC_TYPE,
    VAR_TYPE
};

const char *const FUNC_TYPE_SYMBOL = "FUNC_T";
const char *const VAR_TYPE_SYMBOL  = "VAR_T";

//---------------------------------------------------------------------------------------------------------------//

const Operation   *GetOperationBySymbol  (char *sym);
const Initializer *GetInitBySymbol       (char *sym);
const ManageElem  *GetManageElemBySymbol (char *sym);

#endif