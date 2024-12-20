#ifndef SIMPLIFIER_H
#define SIMPLIFIER_H

#include <stdio.h>

#include "../../tree/tree_lib.h"

void  SimplifyTree      (Tree *code_tree);

Node *SimplifyConstants (Tree *code_tree, Node *cur_node, bool *was_simplified);
Node *DelNeutralElements(Tree *code_tree, Node *cur_node, bool *was_simplified);

Node *CalculateNode     (Tree *tree,      Node *op_node);

Node *SimpleBoolEqVars  (Tree *tree,      Node *cur_node, bool *was_simplified);
Node *SimpleAddVars     (Tree *tree, Node *cur_node, bool *was_simplified);
Node *SimpleSubVars     (Tree *tree, Node *cur_node, bool *was_simplified);
Node *SimpleMulVars     (Tree *tree, Node *cur_node, bool *was_simplified);
Node *SimpleDivVars     (Tree *tree, Node *cur_node, bool *was_simplified);
Node *SimpleDegVars     (Tree *tree, Node *cur_node, bool *was_simplified);

struct MathOpSimplifierInfo
{
    MathOperation_enum num;
    Node* (*simpl_vars_func) (Tree *tree, Node *cur_node, bool *was_simplified);
};

const MathOpSimplifierInfo MathOpSimplifierInfos[MATH_OPERATIONS_NUM] =
{
    { BOOL_EQ,      SimpleBoolEqVars   },
    { BOOL_NEQ,     DelNeutralElements },
    { BOOL_LOWER,   DelNeutralElements },
    { BOOL_GREATER, DelNeutralElements },
    { ADD,          SimpleAddVars      },
    { SUB,          SimpleSubVars      },
    { MUL,          SimpleMulVars      },
    { DIV,          SimpleDivVars      },
    { DEG,          SimpleDegVars      },

    { LN,           DelNeutralElements },
    { LOG,          DelNeutralElements },

    { SIN,          DelNeutralElements },
    { COS,          DelNeutralElements },
};

#endif