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

const int OPERATIONS_NUM = 10;

const Operation Operations[OPERATIONS_NUM] = 
{
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

enum Managers_enum
{
    OPEN_BLOCK_BRACKET,
    CLOSE_BLOCK_BRACKET,
    OPEN_EXPR_BRACKET,
    CLOSE_EXPR_BRACKET,
    COMMA,
    EOT
};

struct ManageElem
{
    const Managers_enum  name;
    const char          *symbol;
};

const int MANAGE_ELEMS_NUM = 6;

const ManageElem Managers[MANAGE_ELEMS_NUM] = 
{
    { OPEN_BLOCK_BRACKET,  "начало_блока"   },
    { CLOSE_BLOCK_BRACKET, "конец_блока" },
    { OPEN_EXPR_BRACKET,   "(" },
    { CLOSE_EXPR_BRACKET,  ")" },
    { COMMA,               "," },
    { EOT,                 "$" }
};

//---------------------------------------------------------------------------------------------------------------//

enum TypeIndicator
{
    VAR_TYPE,
    FUNC_TYPE
};

const char *const VAR_TYPE_SYMBOL  = "VAR_T";
const char *const FUNC_TYPE_SYMBOL = "FUNC_T";

//---------------------------------------------------------------------------------------------------------------//

enum KeyWord_enum
{
    INT_INIT,
    DOUBLE_INIT,
    NEW_EXPR,
    ASSIGN,
    IF
};

struct KeyWord
{
    const KeyWord_enum  name;
    const char         *symbol; 
};

const int KEY_WORDS_NUM = 5;

const KeyWord KeyWords[KEY_WORDS_NUM] = 
{
    { INT_INIT,    "инт"  },
    { DOUBLE_INIT, "дабл" },
    { NEW_EXPR,    "новая_строка" },
    { ASSIGN,      "="    },
    { IF,          "если" }
};

//---------------------------------------------------------------------------------------------------------------//

const Operation   *GetOperationBySymbol  (char *sym);
const KeyWord     *GetKeyWordBySymbol    (char *sym);
const ManageElem  *GetManageElemBySymbol (char *sym);

#endif