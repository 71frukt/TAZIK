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
    { BOOL_EQ,      "будто",    "==",  "JNE",  BINARY, INFIX,  IsEqual    },
    { BOOL_NEQ,     "отличает", "!=",  "JE",   BINARY, INFIX,  IsNotEqual },
    { BOOL_LOWER,   "слабее",   "\\<", "JA",   BINARY, INFIX,  IsBelow    },         // JA для JB специально 
    { BOOL_GREATER, "сильнее",  "\\>", "JB",   BINARY, INFIX,  IsAbove    },         // JB для JA специально   
    { ADD,          "вместе",   "+",   "ADD",  BINARY, INFIX,  Add        },
    { SUB,          "без",      "-",   "SUB",  BINARY, INFIX,  Sub        },
    { MUL,          "дохера",   "*",   "MUL",  BINARY, INFIX,  Mul        },
    { DIV,          "ограбить", "/",   "DIV",  BINARY, INFIX,  Div        },
    { SQRT,         "корешок",  "cor", "SQRT", UNARY,  PREFIX, Sqrt       },
    { DEG,          "^",        "^",   NULL,   BINARY, INFIX,  Deg        },
   
    { LN,           "лн",       "ln",  NULL,   UNARY,  PREFIX, Ln        },
    { LOG,          "лог",      "log", NULL,   BINARY, PREFIX, Log       },
   
    { SIN,          "син",      "sin", "SIN",  UNARY,  PREFIX, Sin       },
    { COS,          "кос",      "cos", "COS",  UNARY,  PREFIX, Cos       },
    { TAN,          "тан",      "tg",  "TG",   UNARY,  PREFIX, Tan       }
};

// const MathOperation MathOperations[MATH_OPERATIONS_NUM] = 
// {
//     { BOOL_EQ,      "==",       "==",  "JNE",  BINARY, INFIX,  IsEqual    },
//     { BOOL_NEQ,     "!=",       "!=",  "JE",   BINARY, INFIX,  IsNotEqual },
//     { BOOL_LOWER,   "<",        "\\<", "JA",   BINARY, INFIX,  IsBelow    },         // JA для JB специально 
//     { BOOL_GREATER, ">",        "\\>", "JB",   BINARY, INFIX,  IsAbove    },         // JB для JA специально   
//     { ADD,          "+",        "+",   "ADD",  BINARY, INFIX,  Add        },
//     { SUB,          "-",        "-",   "SUB",  BINARY, INFIX,  Sub        },
//     { MUL,          "*",        "*",   "MUL",  BINARY, INFIX,  Mul        },
//     { DIV,          "/",        "/",   "DIV",  BINARY, INFIX,  Div        },
//     { SQRT,         "корень",   "cor", "SQRT", UNARY,  PREFIX, Sqrt       },
//     { DEG,          "^",        "^",   NULL,   BINARY, INFIX,  Deg        },
   
//     { LN,           "лн",       "ln",  NULL,   UNARY,  PREFIX, Ln        },
//     { LOG,          "лог",      "log", NULL,   BINARY, PREFIX, Log       },
   
//     { SIN,          "син",      "sin", "SIN",  UNARY,  PREFIX, Sin       },
//     { COS,          "кос",      "cos", "COS",  UNARY,  PREFIX, Cos       },
//     { TAN,          "тан",      "tg",  "TG",   UNARY,  PREFIX, Tan       }
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
    { OPEN_BLOCK_BRACKET_P1,  "панки"       , "{1"  },    
    { OPEN_BLOCK_BRACKET_P2,  "хой"         , "{2"  },    
    { OPEN_BLOCK_BRACKET,     "панки хой"   , "{"   },  
    { CLOSE_BLOCK_BRACKET_P1, "попса"       , "}1"  },
    { CLOSE_BLOCK_BRACKET_P2, "отстой"      , "}2"  },
    { CLOSE_BLOCK_BRACKET,    "попса отстой", "}"   },
    { OPEN_EXPR_BRACKET,      "желаешь"     , "("   },  
    { CLOSE_EXPR_BRACKET,     "расскажу"    , ")"   }, 
    { EOT,                    "$"           , "EOT" },

    { DIGIT_ZERO,             "ноль"        , "0"   },
    { DIGIT_ONE,              "раз"         , "1"   },
    { DIGIT_TWO,              "два"         , "2"   },
    { DIGIT_TREE,             "три"         , "3"   },
    { DIGIT_FOUR,             "четыре"      , "4"   },
    { DIGIT_FIVE,             "пять"        , "5"   },
    { DIGIT_SIX,              "шесть"       , "6"   },
    { DIGIT_SEVEN,            "семь"        , "7"   },
    { DIGIT_EIGHT,            "восемь"      , "8"   },
    { DIGIT_NINE,             "девять"      , "9"   },
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
    { INT_INIT,         "целый",    "int"       },
    { DOUBLE_INIT,      "дабл",     "double"    },
    { NEW_EXPR,         "хой",      "new_line"  },
    { COMMA,            ",",        "param"     },
    { NEW_FUNC,         "",         "new_func"  },
    { ASSIGN,           "теперь" ,  "="         },
    { IF,               "коль",     "if"        },
    { WHILE,            "пока",     "while"     },
    { RETURN,           "лес",      "return"    },
    { SPU_IN,           "почитаю",  "scanf"     },
    { SPU_OUT,          "болтал",   "printf"    },
};

// const KeyWord KeyWords[KEY_WORDS_NUM] = 
// {
//     { VAR_T_INDICATOR,  "",      "var_t"     },
//     { FUNC_T_INDICATOR, "",      "func_t"    },
//     { FUNC_CALL,        "",      "call"      },
//     { INT_INIT,         "инт",   "int"       },
//     { DOUBLE_INIT,      "дабл",  "double"    },
//     { NEW_EXPR,         ";",     "new_line"  },
//     { COMMA,            ",",     "param"     },
//     { NEW_FUNC,         "",      "new_func"  },
//     { ASSIGN,           "=" ,    "="         },
//     { IF,               "если",  "if"        },
//     { WHILE,            "пока",  "while"     },
//     { RETURN,           "рет",   "return"    },
//     { SPU_IN,           "скан",  "scanf"     },
//     { SPU_OUT,          "принт", "printf"    },
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