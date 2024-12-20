#include <stdio.h>
#include <math.h>
#include <string.h>
#include <assert.h>

#include "operations.h"
#include "tree_debug.h"
#include "asm_operations.h"

extern FILE *LogFile;

const MathOperation *GetOperationBySymbol(char *sym, SymbolMode mode) // TODO: move asm from here
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

        if (compare_sym == NULL)
            continue;

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

Node *CalculateNode(Tree *tree, Node *op_node)
{
    assert(tree);
    assert(op_node);
    assert(op_node->type == MATH_OP);
    assert(op_node->left != NULL && (op_node->val.math_op->type == UNARY || op_node->right != NULL));

    if (op_node->left->type != NUM || op_node->right->type != NUM)
    {
        fprintf(stderr, "Trying to calculate op with not-a-number args\n");
        return op_node;
    }

    assert(op_node->left);
    assert(op_node->right);

    TreeElem_t res_val = op_node->val.math_op->op_func(op_node->left, op_node->right);

    RemoveNode(tree, &op_node->left);
    RemoveNode(tree, &op_node->right);

    op_node->type = NUM;
    op_node->val.num = res_val;

    return op_node;
}

TreeElem_t IsEqual(Node *arg1, Node *arg2)
{
    return arg1->val.num == arg2->val.num;
}

TreeElem_t IsNotEqual(Node *arg1, Node *arg2)
{
    return arg1->val.num != arg2->val.num;
}

TreeElem_t IsBelow(Node *arg1, Node *arg2)
{
    return arg1->val.num < arg2->val.num;
}

TreeElem_t IsAbove(Node *arg1, Node *arg2)
{
    return arg1->val.num > arg2->val.num;
}

TreeElem_t Add(Node *arg1, Node *arg2)
{
    return arg1->val.num + arg2->val.num;
}

TreeElem_t Sub(Node *arg1, Node *arg2)
{
    return arg1->val.num - arg2->val.num;
}

TreeElem_t Mul(Node *arg1, Node *arg2)
{
    return arg1->val.num * arg2->val.num;
}

TreeElem_t Div(Node *arg1, Node *arg2)
{
    return arg1->val.num / arg2->val.num;
}

TreeElem_t Deg(Node *arg1, Node *arg2)
{
    return (TreeElem_t) pow(arg1->val.num, arg2->val.num);
}

TreeElem_t Sin(Node *arg1, Node *arg2)
{
    return (TreeElem_t) sin(arg1->val.num);
}

TreeElem_t Cos(Node *arg1, Node *arg2)
{
    return (TreeElem_t) cos(arg1->val.num);
}

TreeElem_t Tan(Node *arg1, Node *arg2)
{
    return (TreeElem_t) tan(arg1->val.num);
}

TreeElem_t Ln(Node *arg1, Node *arg2)
{
    return (TreeElem_t) log(arg1->val.num);
}

TreeElem_t Log(Node *arg1, Node *arg2)
{
    return (TreeElem_t) (log(arg2->val.num) / log(arg1->val.num));
}
