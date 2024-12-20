#ifndef BACK_WRITER_H
#define BACK_WRITER_H

#include <stdio.h>

#include "../../tree/tree_lib.h"

#define BASE_OUTPUT_ASM_FILE_NAME  "../../executable.asm"

#ifndef COMMENT_SYMBOL
#define COMMENT_SYMBOL  ';'
#endif

FILE *GetOutputAsmFile(const int argc, const char *argv[]);

void PrintAsmCodeByNode (Node *node,    FILE *dest_file);
void PrintMathOpAsm     (Node *math_op, FILE *dest_file);
void PopToEmptyRam      (FILE *asm_file);

//---------------------------------------------------------------------------------------------------------------//

struct MathOpAsmInfo
{
    MathOperation_enum num;
    const char *asm_symbol;
};

const MathOpAsmInfo MathOpAsmInfos[MATH_OPERATIONS_NUM] = 
{
    { BOOL_EQ,      "JNE" },
    { BOOL_NEQ,     "JE"  },
    { BOOL_GREATER, "JB"  },
    { BOOL_LOWER,   "JA"  },
    { ADD,          "ADD" },
    { SUB,          "SUB" },
    { MUL,          "MUL" },
    { DIV,          "DIV" },
    { DEG,          NULL  },
  
    { LN,           NULL  },
    { LOG,          NULL  },
  
    { SIN,          "SIN" },
    { COS,          "COS" },
    { TAN,          "TG"  }
};

//---------------------------------------------------------------------------------------------------------------//


void PrintVarTAsm        (Node *var_t_node,    FILE *dest_file);
void PrintInitAsm        (Node *init_node,     FILE *dest_file);
void PrintAssignAsm      (Node *assign_node,   FILE *dest_file);
void PrintChildrenAsm    (Node *new_expr_node, FILE *dest_file);
void PrintIfAsm          (Node *if_node,       FILE *dest_file);
void PrintWhileAsm       (Node *while_node,    FILE *dest_file);
void PrintReturnAsm      (Node *ret_node,      FILE *dest_file);
void PrintCallAsm        (Node *call_node,     FILE *dest_file);
void PrintPassArgsInCall (Node *comma_node,    FILE *dest_file);

struct KeyWordAsmInfo
{
    KeyWord_enum name;
    void (*PrintAsmCodeFunc)(Node *node, FILE *dest_file);
};

const KeyWordAsmInfo KeyWordAsmInfos[KEY_WORDS_NUM] = 
{
    { VAR_T_INDICATOR,  PrintVarTAsm     },
    { FUNC_T_INDICATOR, NULL             },
    { FUNC_CALL,        PrintCallAsm     },
    { INT_INIT,         PrintInitAsm     },
    { DOUBLE_INIT,      PrintInitAsm     },
    { NEW_EXPR,         PrintChildrenAsm },
    { COMMA,            PrintChildrenAsm },
    { NEW_FUNC,         PrintChildrenAsm },
    { ASSIGN,           PrintAssignAsm   },
    { IF,               PrintIfAsm       },
    { WHILE,            PrintWhileAsm    },
    { RETURN,           PrintReturnAsm   }
};

#endif