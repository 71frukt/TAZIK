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
    SQRT,
    DEG,

    LN,
    LOG,

    SIN,
    COS,
    TAN
};

struct Node;
struct Tree;

TreeElem_t IsEqual    (Node *arg1, Node *arg2);
TreeElem_t IsNotEqual (Node *arg1, Node *arg2);
TreeElem_t IsBelow    (Node *arg1, Node *arg2);
TreeElem_t IsAbove    (Node *arg1, Node *arg2);

TreeElem_t Add        (Node *arg1, Node *arg2);
TreeElem_t Sub        (Node *arg1, Node *arg2);
TreeElem_t Mul        (Node *arg1, Node *arg2);
TreeElem_t Div        (Node *arg1, Node *arg2);

TreeElem_t Sqrt       (Node *arg1, Node *arg2);
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

const int MATH_OPERATIONS_NUM = 15;

const MathOperation MathOperations[MATH_OPERATIONS_NUM] = 
{
    { BOOL_EQ,      "�����",    "==",  "JNE",  BINARY, INFIX,  IsEqual    },
    { BOOL_NEQ,     "��������", "!=",  "JE",   BINARY, INFIX,  IsNotEqual },
    { BOOL_LOWER,   "������",   "\\<", "JA",   BINARY, INFIX,  IsBelow    },         // JA ��� JB ���������� 
    { BOOL_GREATER, "�������",  "\\>", "JB",   BINARY, INFIX,  IsAbove    },         // JB ��� JA ����������   
    { ADD,          "������",   "+",   "ADD",  BINARY, INFIX,  Add        },
    { SUB,          "���",      "-",   "SUB",  BINARY, INFIX,  Sub        },
    { MUL,          "������",   "*",   "MUL",  BINARY, INFIX,  Mul        },
    { DIV,          "��������", "/",   "DIV",  BINARY, INFIX,  Div        },
    { SQRT,         "�������",  "cor", "SQRT", UNARY,  PREFIX, Sqrt       },
    { DEG,          "^",        "^",   NULL,   BINARY, INFIX,  Deg        },
   
    { LN,           "��",       "ln",  NULL,   UNARY,  PREFIX, Ln        },
    { LOG,          "���",      "log", NULL,   BINARY, PREFIX, Log       },
   
    { SIN,          "���",      "sin", "SIN",  UNARY,  PREFIX, Sin       },
    { COS,          "���",      "cos", "COS",  UNARY,  PREFIX, Cos       },
    { TAN,          "���",      "tg",  "TG",   UNARY,  PREFIX, Tan       }
};

// const MathOperation MathOperations[MATH_OPERATIONS_NUM] = 
// {
//     { BOOL_EQ,      "==",       "==",  "JNE",  BINARY, INFIX,  IsEqual    },
//     { BOOL_NEQ,     "!=",       "!=",  "JE",   BINARY, INFIX,  IsNotEqual },
//     { BOOL_LOWER,   "<",        "\\<", "JA",   BINARY, INFIX,  IsBelow    },         // JA ��� JB ���������� 
//     { BOOL_GREATER, ">",        "\\>", "JB",   BINARY, INFIX,  IsAbove    },         // JB ��� JA ����������   
//     { ADD,          "+",        "+",   "ADD",  BINARY, INFIX,  Add        },
//     { SUB,          "-",        "-",   "SUB",  BINARY, INFIX,  Sub        },
//     { MUL,          "*",        "*",   "MUL",  BINARY, INFIX,  Mul        },
//     { DIV,          "/",        "/",   "DIV",  BINARY, INFIX,  Div        },
//     { SQRT,         "������",   "cor", "SQRT", UNARY,  PREFIX, Sqrt       },
//     { DEG,          "^",        "^",   NULL,   BINARY, INFIX,  Deg        },
   
//     { LN,           "��",       "ln",  NULL,   UNARY,  PREFIX, Ln        },
//     { LOG,          "���",      "log", NULL,   BINARY, PREFIX, Log       },
   
//     { SIN,          "���",      "sin", "SIN",  UNARY,  PREFIX, Sin       },
//     { COS,          "���",      "cos", "COS",  UNARY,  PREFIX, Cos       },
//     { TAN,          "���",      "tg",  "TG",   UNARY,  PREFIX, Tan       }
// };

//------------------------------------------------------------------------------------------------------------//

enum Managers_enum
{
    OPEN_BLOCK_BRACKET_P1,
    OPEN_BLOCK_BRACKET_P2,
    OPEN_BLOCK_BRACKET,
    CLOSE_BLOCK_BRACKET_P1,
    CLOSE_BLOCK_BRACKET_P2,
    CLOSE_BLOCK_BRACKET,
    OPEN_EXPR_BRACKET,
    CLOSE_EXPR_BRACKET,
    EOT,
    
    DIGIT_ZERO,
    DIGIT_ONE,
    DIGIT_TWO,
    DIGIT_TREE,
    DIGIT_FOUR,
    DIGIT_FIVE,
    DIGIT_SIX,
    DIGIT_SEVEN,
    DIGIT_EIGHT,
    DIGIT_NINE
};

struct ManageElem
{
    const Managers_enum  name;
    const char          *my_symbol;
    const char          *real_symbol;
};

const int MANAGE_ELEMS_NUM = 19;

const ManageElem Managers[MANAGE_ELEMS_NUM] = 
{
    { OPEN_BLOCK_BRACKET_P1,  "�����"       , "{1"  },    
    { OPEN_BLOCK_BRACKET_P2,  "���"         , "{2"  },    
    { OPEN_BLOCK_BRACKET,     "����� ���"   , "{"   },  
    { CLOSE_BLOCK_BRACKET_P1, "�����"       , "}1"  },
    { CLOSE_BLOCK_BRACKET_P2, "������"      , "}2"  },
    { CLOSE_BLOCK_BRACKET,    "����� ������", "}"   },
    { OPEN_EXPR_BRACKET,      "�������"     , "("   },  
    { CLOSE_EXPR_BRACKET,     "��������"    , ")"   }, 
    { EOT,                    "$"           , "EOT" },

    { DIGIT_ZERO,             "����"        , "0"   },
    { DIGIT_ONE,              "���"         , "1"   },
    { DIGIT_TWO,              "���"         , "2"   },
    { DIGIT_TREE,             "���"         , "3"   },
    { DIGIT_FOUR,             "������"      , "4"   },
    { DIGIT_FIVE,             "����"        , "5"   },
    { DIGIT_SIX,              "�����"       , "6"   },
    { DIGIT_SEVEN,            "����"        , "7"   },
    { DIGIT_EIGHT,            "������"      , "8"   },
    { DIGIT_NINE,             "������"      , "9"   },
};


// const ManageElem Managers[MANAGE_ELEMS_NUM] = 
// {
//     { OPEN_BLOCK_BRACKET_P1,  "{", "{"   },
//     { CLOSE_BLOCK_BRACKET, "}", "}"   },
//     { OPEN_EXPR_BRACKET,   "(", "("   },
//     { CLOSE_EXPR_BRACKET,  ")", ")"   },
//     { EOT,                 "$", "EOT" }
// };

//------------------------------------------------------------------------------------------------------------//


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
    RETURN,
    SPU_IN,
    SPU_OUT,
};

struct KeyWord
{
    const KeyWord_enum  name;
    const char         *my_symbol;
    const char         *real_symbol;
};

const int KEY_WORDS_NUM = 14;

const KeyWord KeyWords[KEY_WORDS_NUM] = 
{
    { VAR_T_INDICATOR,  "",         "var_t"     },
    { FUNC_T_INDICATOR, "",         "func_t"    },
    { FUNC_CALL,        "",         "call"      },
    { INT_INIT,         "�����",    "int"       },
    { DOUBLE_INIT,      "����",     "double"    },
    { NEW_EXPR,         "���",      "new_line"  },
    { COMMA,            ",",        "param"     },
    { NEW_FUNC,         "",         "new_func"  },
    { ASSIGN,           "������" ,  "="         },
    { IF,               "����",     "if"        },
    { WHILE,            "����",     "while"     },
    { RETURN,           "���",      "return"    },
    { SPU_IN,           "�������",  "scanf"     },
    { SPU_OUT,          "������",   "printf"    },
};

// const KeyWord KeyWords[KEY_WORDS_NUM] = 
// {
//     { VAR_T_INDICATOR,  "",      "var_t"     },
//     { FUNC_T_INDICATOR, "",      "func_t"    },
//     { FUNC_CALL,        "",      "call"      },
//     { INT_INIT,         "���",   "int"       },
//     { DOUBLE_INIT,      "����",  "double"    },
//     { NEW_EXPR,         ";",     "new_line"  },
//     { COMMA,            ",",     "param"     },
//     { NEW_FUNC,         "",      "new_func"  },
//     { ASSIGN,           "=" ,    "="         },
//     { IF,               "����",  "if"        },
//     { WHILE,            "����",  "while"     },
//     { RETURN,           "���",   "return"    },
//     { SPU_IN,           "����",  "scanf"     },
//     { SPU_OUT,          "�����", "printf"    },
// };


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