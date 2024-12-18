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

void PrintAsmCodeByNode (Node *node,    FILE *dest_file);
void PrintMathOpAsm     (Node *math_op, FILE *dest_file);
void PopToEmptyRam      (FILE *asm_file);

struct MathOperation
{
    const MathOperation_enum  num;
    const char           *my_symbol;
    const char           *real_symbol;
    const char           *asm_symbol;

    const FuncType       type;                                               // UNARY  / BINARY
    const FuncEntryForm  form;                                               // PREFIX / INFIX
};

const int OPERATIONS_NUM = 14;

const MathOperation MathOperations[OPERATIONS_NUM] = 
{
    { BOOL_EQ,      "==",   "==",  "JNE", BINARY, INFIX  },
    { BOOL_NEQ,     "!=",   "!=",  "JE",  BINARY, INFIX  },
    { BOOL_GREATER, ">",    "\\>", "JB",  BINARY, INFIX  },
    { BOOL_LOWER,   "<",    "\\<", "JA",  BINARY, INFIX  },
    { ADD,          "+",    "+",   "ADD", BINARY, INFIX  },
    { SUB,          "-",    "-",   "SUB", BINARY, INFIX  },
    { MUL,          "*",    "*",   "MUL", BINARY, INFIX  },
    { DIV,          "/",    "/",   "DIV", BINARY, INFIX  },
    { DEG,          "^",    "^",   NULL,  BINARY, INFIX  },
  
    { LN,           "��",   "ln",  NULL,  UNARY,  PREFIX },
    { LOG,          "���",  "log", NULL,  BINARY, PREFIX },
  
    { SIN,          "���",  "sin", "SIN", UNARY,  PREFIX },
    { COS,          "���",  "cos", "COS", UNARY,  PREFIX },
    { TAN,          "���",  "tg",  "TG",  UNARY,  PREFIX }
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

void PrintVarTAsm        (Node *var_t_node,    FILE *dest_file);
void PrintInitAsm        (Node *init_node,     FILE *dest_file);
void PrintAssignAsm      (Node *assign_node,   FILE *dest_file);
void PrintChildrenAsm    (Node *new_expr_node, FILE *dest_file);
void PrintIfAsm          (Node *if_node,       FILE *dest_file);
void PrintWhileAsm       (Node *while_node,    FILE *dest_file);
void PrintReturnAsm      (Node *ret_node,      FILE *dest_file);
void PrintCallAsm        (Node *call_node,     FILE *dest_file);
void PrintPassArgsInCall (Node *comma_node,    FILE *dest_file);


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

    void (*PrintAsmCodeFunc)(Node *node, FILE *dest_file);
};

const int KEY_WORDS_NUM = 12;

const KeyWord KeyWords[KEY_WORDS_NUM] = 
{
    { VAR_T_INDICATOR,   NULL,           "var_t",     PrintVarTAsm },
    { FUNC_T_INDICATOR,  NULL,           "func_t",    NULL },
    { FUNC_CALL,         NULL,           "func_call", PrintCallAsm },
    { INT_INIT,         "���",           "int",       PrintInitAsm },
    { DOUBLE_INIT,      "����",          "double",    PrintInitAsm },
    { NEW_EXPR,         "�����_������",  "new_line",  PrintChildrenAsm },
    { COMMA,            "�������",       "comma",     PrintChildrenAsm },
    { NEW_FUNC,         NULL,            "new_func",  PrintChildrenAsm },
    { ASSIGN,           "=" ,            "=",         PrintAssignAsm },
    { IF,               "����",          "if",        PrintIfAsm },
    { WHILE,            "����",          "while",     PrintWhileAsm },
    { RETURN,           "���",           "return",    PrintReturnAsm }
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