#ifndef OPERATIONS_H
#define OPERATIONS_H

#include <stdio.h>

#include "tree_elem_t_info.h"

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

enum MathOperation_enum
{
    BOOL_EQ,
    BOOL_NEQ,
    BOOL_GREATER,
    BOOL_LOWER,
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

struct Node;
struct Tree;

Node *CalculateNode(Tree *tree, Node *op_node);

TreeElem_t IsEqual    (Node *arg1, Node *arg2);
TreeElem_t IsNotEqual (Node *arg1, Node *arg2);
TreeElem_t IsBelow    (Node *arg1, Node *arg2);
TreeElem_t IsAbove    (Node *arg1, Node *arg2);

TreeElem_t Add        (Node *arg1, Node *arg2);
TreeElem_t Sub        (Node *arg1, Node *arg2);
TreeElem_t Mul        (Node *arg1, Node *arg2);
TreeElem_t Div        (Node *arg1, Node *arg2);

TreeElem_t Deg        (Node *arg1, Node *arg2);
TreeElem_t Ln         (Node *arg1, Node *arg2);
TreeElem_t Log        (Node *arg1, Node *arg2);

TreeElem_t Sin        (Node *arg1, Node *arg2);
TreeElem_t Cos        (Node *arg1, Node *arg2);
TreeElem_t Tan        (Node *arg1, Node *arg2);

struct MathOperation
{
    const MathOperation_enum  num;
    const char           *my_symbol;
    const char           *real_symbol;
    const char           *asm_symbol;

    const FuncType       type;                                               // UNARY  / BINARY
    const FuncEntryForm  form;                                               // PREFIX / INFIX

    TreeElem_t  (*op_func)  (Node *arg1, Node *arg2);
};

const int OPERATIONS_NUM = 14;

const MathOperation MathOperations[OPERATIONS_NUM] = 
{
    { BOOL_EQ,      "==",   "==",  "JNE", BINARY, INFIX, IsEqual    },
    { BOOL_NEQ,     "!=",   "!=",  "JE",  BINARY, INFIX, IsNotEqual },
    { BOOL_LOWER,   "<",    "\\<", "JB",  BINARY, INFIX, IsBelow    },
    { BOOL_GREATER, ">",    "\\>", "JA",  BINARY, INFIX, IsAbove    },
    { ADD,          "+",    "+",   "ADD", BINARY, INFIX, Add        },
    { SUB,          "-",    "-",   "SUB", BINARY, INFIX, Sub        },
    { MUL,          "*",    "*",   "MUL", BINARY, INFIX, Mul        },
    { DIV,          "/",    "/",   "DIV", BINARY, INFIX, Div        },
    { DEG,          "^",    "^",   NULL,  BINARY, INFIX, Deg        },
  
    { LN,           "��",   "ln",  NULL,  UNARY,  PREFIX, Ln        },
    { LOG,          "���",  "log", NULL,  BINARY, PREFIX, Log       },
  
    { SIN,          "���",  "sin", "SIN", UNARY,  PREFIX, Sin       },
    { COS,          "���",  "cos", "COS", UNARY,  PREFIX, Cos       },
    { TAN,          "���",  "tg",  "TG",  UNARY,  PREFIX, Tan       }
};

//------------------------------------------------------------------------------------------------------------//

enum Managers_enum
{
    OPEN_BLOCK_BRACKET,
    CLOSE_BLOCK_BRACKET,
    OPEN_EXPR_BRACKET,
    CLOSE_EXPR_BRACKET,
    EOT
};

struct ManageElem
{
    const Managers_enum  name;
    const char          *my_symbol;
    const char          *real_symbol;
};

const int MANAGE_ELEMS_NUM = 5;

const ManageElem Managers[MANAGE_ELEMS_NUM] = 
{
    { OPEN_BLOCK_BRACKET,  "������_�����", "{"   },
    { CLOSE_BLOCK_BRACKET, "�����_�����",  "}"   },
    { OPEN_EXPR_BRACKET,   "(",            "("   },
    { CLOSE_EXPR_BRACKET,  ")",            ")"   },
    { EOT,                 "$",            "EOT" }
};

//------------------------------------------------------------------------------------------------------------//

// void PrintVarTAsm        (Node *var_t_node,    FILE *dest_file);
// void PrintInitAsm        (Node *init_node,     FILE *dest_file);
// void PrintAssignAsm      (Node *assign_node,   FILE *dest_file);
// void PrintChildrenAsm    (Node *new_expr_node, FILE *dest_file);
// void PrintIfAsm          (Node *if_node,       FILE *dest_file);
// void PrintWhileAsm       (Node *while_node,    FILE *dest_file);
// void PrintReturnAsm      (Node *ret_node,      FILE *dest_file);
// void PrintCallAsm        (Node *call_node,     FILE *dest_file);
// void PrintPassArgsInCall (Node *comma_node,    FILE *dest_file);


enum KeyWord_enum
{
    VAR_T_INDICATOR,
    FUNC_T_INDICATOR,
    FUNC_CALL,
    INT_INIT,
    DOUBLE_INIT,
    NEW_EXPR,
    COMMA,
    NEW_FUNC,
    ASSIGN,
    IF,
    WHILE,
    RETURN
};

struct KeyWord
{
    const KeyWord_enum  name;
    const char         *my_symbol;
    const char         *real_symbol;
};

const int KEY_WORDS_NUM = 12;

const KeyWord KeyWords[KEY_WORDS_NUM] = 
{
    { VAR_T_INDICATOR,   NULL,           "var_t"     },
    { FUNC_T_INDICATOR,  NULL,           "func_t"    },
    { FUNC_CALL,         NULL,           "func_call" },
    { INT_INIT,         "���",           "int"       },
    { DOUBLE_INIT,      "����",          "double"    },
    { NEW_EXPR,         "�����_������",  "new_line"  },
    { COMMA,            "�������",       "comma"     },
    { NEW_FUNC,         NULL,            "new_func"  },
    { ASSIGN,           "=" ,            "="         },
    { IF,               "����",          "if"        },
    { WHILE,            "����",          "while"     },
    { RETURN,           "���",           "return"    }
};

//---------------------------------------------------------------------------------------------------------------//

enum SymbolMode
{
    MY_CODE_MODE,
    REAL_CODE_MODE
};

const MathOperation *GetOperationBySymbol  (char *sym, SymbolMode mode);
const KeyWord       *GetKeyWordBySymbol    (char *sym, SymbolMode mode);
const ManageElem    *GetManageElemBySymbol (char *sym, SymbolMode mode);


#endif