#include <stdio.h>
#include <assert.h>

#include "simplifier.h"

void SimplifyTree(Tree *code_tree)
{
    TREE_DUMP(code_tree);

    bool was_simplified = false;

    do
    {
        was_simplified = false;
        fprintf(stderr, "simplify\n");
        SimplifyConstants(code_tree, code_tree->root_ptr, &was_simplified);
    }
    while (was_simplified);

    TREE_DUMP(code_tree);
}

Node *SimplifyConstants(Tree *code_tree, Node *cur_node, bool *was_simplified)
{
    assert(code_tree);
    assert(was_simplified);

    if (cur_node == NULL || cur_node->type == NUM || cur_node->type == VAR)
        return cur_node;

    cur_node->left  = SimplifyConstants(code_tree, cur_node->left,  was_simplified);
    cur_node->right = SimplifyConstants(code_tree, cur_node->right, was_simplified);

    
    if (cur_node->type == MATH_OP)
    {
        bool args_are_num = (cur_node->left->type == NUM && (cur_node->val.math_op->type == UNARY || cur_node->right->type == NUM));

        if (args_are_num)
        {
            *was_simplified = true;
            CalculateNode(code_tree, cur_node);
        }
    }

    return cur_node;
}

Node *DelNetral