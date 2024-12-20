#ifndef SIMPLIFIER_H
#define SIMPLIFIER_H

#include <stdio.h>

#include "../../tree/tree_lib.h"

void SimplifyTree(Tree *code_tree);
Node *SimplifyConstants(Tree *code_tree, Node *cur_node, bool *was_simplified);


#endif