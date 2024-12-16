#include <stdio.h>
#include <math.h>
#include <string.h>
#include <assert.h>

#include "operations.h"
#include "tree_debug.h"

extern FILE *LogFile;

const MathOperation *GetOperationBySymbol(char *sym)
{
    assert(sym);

    for (size_t i = 0; i < OPERATIONS_NUM; i++)
    {
        if (strcmp(sym, MathOperations[i].my_symbol) == 0)
            return &MathOperations[i];
    }

    return NULL;
}

const KeyWord *GetKeyWordBySymbol(char *sym)
{
    assert(sym);

    for (size_t i = 0; i < KEY_WORDS_NUM; i++)
    {
        if (strcmp(sym, KeyWords[i].my_symbol) == 0)
            return &KeyWords[i];
    }

    return NULL;
}

const ManageElem *GetManageElemBySymbol(char *sym)
{
    assert(sym);

    for (size_t i = 0; i < MANAGE_ELEMS_NUM; i++)
    {
        if (strcmp(sym, Managers[i].my_symbol) == 0)
            return &Managers[i];
    }

    return NULL;
}