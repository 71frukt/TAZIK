#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cstring>  
#include <ctype.h>
#include <sys/stat.h>
#include <assert.h>

#include "tree.h"
#include "debug.h"
#include "graph.h"
#include "reader.h"
#include "operations.h"


void TreeCtor(Tree *tree, size_t start_capacity ON_LANG_DEBUG(, const char *name))
{ 
    assert(tree);
    assert(start_capacity > 0);

    tree->capacity = 0;
    tree->size     = 0;

    TreeRecalloc(tree, start_capacity);

    NamesTableCtor(START_NAMES_TABLE_CAPA, &tree->names_table);

    ON_LANG_DEBUG(
        tree->name = name;

        char path[PATH_NAME_LEN] = {};
        GetFilePath(name, LOGS_FOLDER GRAPH_FOLDER, path);

        mkdir(path);
    );
}

void TreeDtor(Tree *tree)
{
    assert(tree);

    LANG_DUMP(tree);

    NamesTableDtor(&tree->names_table);

    for (size_t i = 0; i < tree->alloc_marks.size; i++)
        free(tree->alloc_marks.data[i]);

    free(tree->node_ptrs);

    tree->capacity = 0;
    tree->size     = 0;



    ON_LANG_DEBUG( 
        tree->name = NULL;
        // remove(TMP_DOTFILE_NAME);
    );
}

void NamesTableCtor(size_t start_capa, NamesTable *table)
{
    table->capacity = start_capa;
    table->size     = 0;
    table->names    = (ProperName *) calloc(start_capa, sizeof(ProperName));
}

void NamesTableDtor(NamesTable *table)
{
    free(table->names);

    table->capacity = 0;
    table->size     = 0;
}

void TreeRecalloc(Tree *tree, size_t new_capacity)
{
    assert(tree);
    assert(new_capacity != 0 && "new_capacity = 0 in TreeRecalloc()");

    size_t prev_capacity = tree->capacity;
    tree->capacity       = new_capacity;

    tree->node_ptrs = (Node **) realloc(tree->node_ptrs, new_capacity * sizeof(Node *));
    Node *new_nodes = (Node *)   calloc(new_capacity - prev_capacity, sizeof(Node));

    assert(tree->node_ptrs);
    assert(new_nodes);

    tree->alloc_marks.data[tree->alloc_marks.size++] = new_nodes;

    for (size_t i = 0; i < new_capacity - prev_capacity; i++)
        tree->node_ptrs[prev_capacity + i] = new_nodes + i;
}

Node *NewNode(Tree *tree, NodeType type, NodeVal val, Node *left, Node *right)
{
    assert(tree);

    if (tree->size >= tree->capacity)
    {
        size_t new_capacity = tree->capacity * 2;
        TreeRecalloc(tree, new_capacity);
    }

    Node *new_node = tree->node_ptrs[tree->size];
    assert(new_node);
    
    new_node->type  = type;
    new_node->val   = val;
    new_node->left  = left;
    new_node->right = right;

    tree->size++;

    return new_node;
}

void RemoveNode(Tree *tree, Node **node)
{
    assert(tree);
    assert(node);
    assert(*node);

    // if ((*node)->type == CHANGE)
    //     RemoveSubtree(tree, &(*node)->val.change->target_node);

    (*node)->left    = NULL;
    (*node)->right   = NULL;
    (*node)->type    = POISON_TYPE;
    (*node)->val.num = 0;
    *node = NULL;
}

void RemoveSubtree(Tree *tree, Node **start_node)
{
    if ((*start_node)->left != NULL)
        RemoveSubtree(tree, &(*start_node)->left);
    if ((*start_node)->right != NULL)
        RemoveSubtree(tree, &(*start_node)->right);

    RemoveNode(tree, start_node);
}

char *NodeValToStr(Node *node)
{
    assert(node);

    static char res_str[TOKEN_LEN] = {};

    if (node->type == NUM)
        sprintf(res_str, TREE_ELEM_PRINT_SPECIFIER, node->val.num);
    
    else if (node->type == VAR || node->type == FUNC || node->type == VAR_OR_FUNC)
        sprintf(res_str, "%s", node->val.prop_name);

    else if (node->type == OP)
        sprintf(res_str, "%s", node->val.op->symbol);

    else if (node->type == MANAGER)
    {
        sprintf(res_str, "%s", node->val.manager->symbol);
    }

    else if (node->type == KEY_WORD)
        sprintf(res_str, "%s", node->val.key_word->symbol);

    else if (node->type == TYPE_INDICATOR)
    {
        if (node->val.type_indicator == FUNC_TYPE)
            sprintf(res_str, "%s", FUNC_TYPE_SYMBOL);
        
        else 
            sprintf(res_str, "%s", VAR_TYPE_SYMBOL);
    }

    else if (node->type == POISON_TYPE)
        sprintf(res_str, "%s", POISON_SYMBOL);

    return res_str;
}

Node *TreeCopyPaste(Tree *source_tree, Tree *dest_tree, Node *coping_node)
{
    Node *pasted_node = NULL;

    if (coping_node->type == NUM || coping_node->type == VAR)
    {
        pasted_node = NewNode(dest_tree, coping_node->type, coping_node->val, NULL, NULL);
    }

    else if (coping_node->type == OP)
    {
        pasted_node = NewNode(dest_tree, OP, coping_node->val, NULL, NULL);

        pasted_node->left  = TreeCopyPaste(source_tree, dest_tree, coping_node->left);

        // if (GetOperationByNode(pasted_node)->type == BINARY)
        if (pasted_node->val.op->type == BINARY)
            pasted_node->right = TreeCopyPaste(source_tree, dest_tree, coping_node->right);

        else (pasted_node->right = pasted_node->left);
    }

    return pasted_node;
}

size_t GetTreeHeight(Node *cur_node)
{
    if (cur_node->type != OP)
        return 1;

    size_t left_height  = GetTreeHeight(cur_node->left);
    size_t right_height = GetTreeHeight(cur_node->right);

    size_t max_height = (left_height > right_height ? left_height : right_height);

    return (max_height + 1);
}

ProperName *FindNameInTable(NamesTable *table, char *name)
{
    for (size_t i = 0; i < table->size; i++)
    {
        if (strcmp(name, table->names[i].name) == 0)
            return &table->names[i];
    }

    return NULL;
}

ProperName *NewNameInTable(NamesTable *table, char *name)
{
    table->size++;

    if (table->size >= table->capacity)
    {
        table->capacity *= 2;
        table->names = (ProperName *) realloc(table->names, table->capacity);
    }

    strncpy(table->names[table->size].name, name, TOKEN_LEN);

    return &table->names[table->size];
}

bool SubtreeContainsType(Node *cur_node, NodeType type)
{
    if (cur_node == NULL)
        return false;

    else if (cur_node->type == type)
        return true;

    else
    {
        bool left_subtree_cont_type = SubtreeContainsType(cur_node->left, type);

        bool right_subtree_cont_type = false;

        // if (cur_node->type == OP && GetOperationByNode(cur_node)->type == BINARY)
        if (cur_node->type == OP && cur_node->val.op->type == BINARY)       // чтобы не проходить одну и ту же ветку дважды для унарной операции
            right_subtree_cont_type = SubtreeContainsType(cur_node->right, type);

        return (left_subtree_cont_type || right_subtree_cont_type);
    }
}

bool OpNodeIsCommutativity(Node *op_node)
{
    assert(op_node);

    if (op_node->type == OP && (op_node->val.op->num == ADD || op_node->val.op->num == MUL))
        return true;
    
    else return false;
}

bool IsInitialise(Node *node)
{
    return (node->type == KEY_WORD && (node->val.key_word->name == INT_INIT || node->val.key_word->name == DOUBLE_INIT));
}