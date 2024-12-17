#include <stdio.h>
#include <math.h>
#include <string.h>
#include <assert.h>

#include "operations.h"
#include "tree_debug.h"

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