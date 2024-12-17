#include <stdio.h>
#include <math.h>
#include <string.h>
#include <assert.h>

#include "operations.h"
#include "tree_debug.h"
#include "asm_operations.h"

extern FILE *LogFile;

const MathOperation *GetOperationBySymbol(char *sym, SymbolMode mode)
{
    assert(sym);

    for (size_t i = 0; i < OPERATIONS_NUM; i++)
    {
        const char *compare_sym = (mode == MY_CODE_MODE ? MathOperations[i].my_symbol : MathOperations[i].real_symbol);

        if (strcmp(sym, compare_sym) == 0)
            return &MathOperations[i];
    }

    return NULL;
}

const KeyWord *GetKeyWordBySymbol(char *sym, SymbolMode mode)
{
    assert(sym);

    for (size_t i = 0; i < KEY_WORDS_NUM; i++)
    {
        const char *compare_sym = (mode == MY_CODE_MODE ? KeyWords[i].my_symbol : KeyWords[i].real_symbol);
fprintf(stderr, "compare with '%s'\n", compare_sym);
        if (strcmp(sym, compare_sym) == 0)
            return &KeyWords[i];
    }

    return NULL;
}

const ManageElem *GetManageElemBySymbol(char *sym, SymbolMode mode)
{
    assert(sym);

    for (size_t i = 0; i < MANAGE_ELEMS_NUM; i++)
    {
        const char *compare_sym = (mode == MY_CODE_MODE ? Managers[i].my_symbol : Managers[i].real_symbol);

        if (strcmp(sym, compare_sym) == 0)
            return &Managers[i];
    }

    return NULL;
}

void PrintAsmForMathOP(Node *math_op, FILE *dest_file)
{
    assert(math_op);
    assert(math_op->type == MATH_OP);
    assert(dest_file);

    PrintAsmCodeForArg(math_op->left,  dest_file);
    PrintAsmCodeForArg(math_op->right, dest_file);

    fprintf(dest_file, "%s\n", math_op->val.math_op->asm_symbol);
}

void PrintAsmCodeForArg(Node *arg, FILE *dest_file)
{
    if (arg->type == NUM)
        fprintf(dest_file, "%s " TREE_ELEM_PRINT_SPECIFIER "\n", AsmOperations[PUSH_ASM].sym, arg->val.num);
    
    else if (arg->type == KEY_WORD && arg->val.key_word->name == VAR_T_INDICATOR)
        fprintf(dest_file, "%s [%lld]\n", AsmOperations[PUSH_ASM].sym, arg->left->val.prop_name->number);

    else
        PrintAsmCodeByNode(arg, dest_file);
}

void PrintAsmCodeByNode(Node *node, FILE *dest_file)
{
    assert(node);
    assert(dest_file);

    if (node->type == MATH_OP)
        PrintAsmForMathOP(node, dest_file);
    
    else
        return;        // TODO: доделать для ключевых слов
}
