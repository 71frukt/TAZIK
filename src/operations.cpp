#include <stdio.h>
#include <math.h>
#include <string.h>
#include <assert.h>

#include "operations.h"
#include "debug.h"

extern FILE *LogFile;

// const Operation *GetOperationByNode(Node *node)
// {
//     assert(node);

//     if (node->type != OP)
//         return NULL; 

//     for (size_t i = 0; i < OPERATIONS_NUM; i++)
//     {
//         if (node->val.op == Operations[i].num)
//             return &Operations[i];
//     }

//     return NULL;
// }

// const Operation *GetOperationByNum(int num)
// {
//     for (size_t i = 0; i < OPERATIONS_NUM; i++)
//     {
//         if (num == Operations[i].num)
//             return &Operations[i];
//     }

//     return NULL;
// }

const Operation *GetOperationBySymbol(char *sym)
{
    assert(sym);

    for (size_t i = 0; i < OPERATIONS_NUM; i++)
    {
        if (strcmp(sym, Operations[i].symbol) == 0)
            return &Operations[i];
    }

    return NULL;
}

const ManageElem *GetManageElemBySymbol(char *sym)
{
    assert(sym);

    for (size_t i = 0; i < MANAGE_ELEMS_NUM; i++)
    {
        if (strcmp(sym, ManagerElems[i].symbol) == 0)
            return &ManagerElems[i];
    }

    return NULL;
}


TreeElem_t Add(TreeElem_t arg1, TreeElem_t arg2)
{
    return arg1 + arg2;
}

TreeElem_t Sub(TreeElem_t arg1, TreeElem_t arg2)
{
    return arg1 - arg2;
}

TreeElem_t Mul(TreeElem_t arg1, TreeElem_t arg2)
{
    return arg1 * arg2;
}

TreeElem_t Div(TreeElem_t arg1, TreeElem_t arg2)
{
    return arg1 / arg2;
}

TreeElem_t Deg(TreeElem_t arg1, TreeElem_t arg2)
{
    return (TreeElem_t) pow(arg1, arg2);
}

TreeElem_t Sin(TreeElem_t arg1, TreeElem_t arg2)
{
    // assert(arg1 == arg2);
    return (TreeElem_t) sin(arg1);
}

TreeElem_t Cos(TreeElem_t arg1, TreeElem_t arg2)
{
    // assert(arg1 == arg2);
    return (TreeElem_t) cos(arg1);
}

TreeElem_t Tan(TreeElem_t arg1, TreeElem_t arg2)
{
    // assert(arg1 == arg2);
    return (TreeElem_t) tan(arg1);
}

TreeElem_t Ln(TreeElem_t arg1, TreeElem_t arg2)
{
    // assert(arg1 == arg2);
    return (TreeElem_t) log(arg1);
}

TreeElem_t Log(TreeElem_t arg1, TreeElem_t arg2)
{
    return (TreeElem_t) (log(arg2) / log(arg1));
}