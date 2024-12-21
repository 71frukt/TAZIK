#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cstring>  
#include <ctype.h>
#include <sys/stat.h>
#include <assert.h>

#include "tree.h"
#include "tree_debug.h"
#include "tree_graph.h"
#include "operations.h"
#include "../../frontend/lib/semantic_anal.h"

void TreeCtor(Tree *tree, size_t start_capacity ON_TREE_DEBUG(, const char *name))
{ 
    assert(tree);
    assert(start_capacity > 0);

    tree->capacity = 0;
    tree->size     = 0;

    TreeRecalloc(tree, start_capacity);

    NamesTableCtor(START_NAMES_TABLE_CAPA, &tree->names_table);

    ON_TREE_DEBUG(
        tree->name = name;

        char path[PATH_NAME_LEN] = {};
        GetFilePath(name, LOGS_FOLDER GRAPH_FOLDER, path);

        mkdir(path);
    );
}

void TreeDtor(Tree *tree)
{
    assert(tree);

    TREE_DUMP(tree);

    NamesTableDtor(&tree->names_table);

    for (size_t i = 0; i < tree->alloc_marks.size; i++)
        free(tree->alloc_marks.data[i]);

    free(tree->node_ptrs);

    tree->capacity = 0;
    tree->size     = 0;



    ON_TREE_DEBUG( 
        tree->name = NULL;
        // remove(TMP_DOTFILE_NAME);
    );
}

void NamesTableCtor(size_t start_capa, NamesTable *table)
{
    assert(table);

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
    static char res_str[TOKEN_LEN] = {};

    if (node == NULL)
        sprintf(res_str, "NULL");

    else if (node->type == NUM)
        sprintf(res_str, TREE_ELEM_PRINT_SPECIFIER, node->val.num);
    
    else if (node->type == VAR || node->type == FUNC || node->type == VAR_OR_FUNC)
        sprintf(res_str, "%s", node->val.prop_name->name);

    else if (node->type == MATH_OP)
        sprintf(res_str, "%s", node->val.math_op->real_symbol);

    else if (node->type == MANAGER)
    {
        sprintf(res_str, "%s", node->val.manager->real_symbol);
    }

    else if (node->type == KEY_WORD)
        sprintf(res_str, "%s", node->val.key_word->real_symbol);

    // else if (node->type == TYPE_INDICATOR)
    // {
    //     if (node->val.type_indicator == FUNC_TYPE)
    //         sprintf(res_str, "%s", FUNC_TYPE_SYMBOL);
        
    //     else 
    //         sprintf(res_str, "%s", VAR_TYPE_SYMBOL);
    // }

    else if (node->type == NEW_BLOCK)
        sprintf(res_str, NEW_BLOCK_SYM);

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

    else if (coping_node->type == MATH_OP)
    {
        pasted_node = NewNode(dest_tree, MATH_OP, coping_node->val, NULL, NULL);

        pasted_node->left  = TreeCopyPaste(source_tree, dest_tree, coping_node->left);

        // if (GetOperationByNode(pasted_node)->type == BINARY)
        if (pasted_node->val.math_op->type == BINARY)
            pasted_node->right = TreeCopyPaste(source_tree, dest_tree, coping_node->right);

        else (pasted_node->right = pasted_node->left);
    }

    return pasted_node;
}

size_t GetTreeHeight(Node *cur_node)
{
    if (cur_node->type != MATH_OP)
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

ProperName *FindNameInBlock(Node *cur_block, char *name)
{
    assert(name);

    if (cur_block == NULL)
        return NULL;
    
    ProperName *res_name = FindNameInTable(&cur_block->val.block.names_table, name);

    if (res_name == NULL && cur_block->val.block.prev_block != NULL)
        return FindNameInBlock(cur_block->val.block.prev_block, name);
    
    else
        return res_name;
}

ProperName *NewNameInTable(NamesTable *table, char *name)
{
    assert(table);
    assert(name);

    fprintf(stderr, "table size = %lld, capa = %lld, name = '%s'\n", table->size, table->capacity, name);


    if (table->capacity == 0)
        NamesTableCtor(START_NAMES_TABLE_CAPA, table);

    fprintf(stderr, "name = '%s'\n", name);
    if (table->size >= table->capacity)
    {
        table->capacity *= 2;
        table->names = (ProperName *) realloc(table->names, table->capacity);
    }

    table->names[table->size].number = table->size;
    strncpy(table->names[table->size].name, name, TOKEN_LEN);

    fprintf(stderr, "NAME SSS = '%s', name = '%s'\n", table->names[table->size].name, name);

    table->names[table->size].is_init = false;                  // �������� �� ��������� ����� ���������� ������

    return &table->names[table->size++];
}

void GetBlockNamesTable(Tree *tree, Node *block, Node *cur_node)
{
    assert(block);
    assert(block->type == NEW_BLOCK);

    NamesTable *table = &block->val.block.names_table;

    if (cur_node == NULL)
        return;

    if (cur_node->type == NEW_BLOCK)
        MakeNamesTablesForBlocks(tree, cur_node);

    else if (cur_node->type == VAR)
    {
        ProperName *cur_name = FindNameInBlock(block, cur_node->val.prop_name->name);
        fprintf(stderr, "use of inited var named '%s', num = %lld\n", cur_name->name, cur_name->number);

        if (cur_name == NULL)
        {
            char error[ERROR_NAME_LEN] = {};
            SYNTAX_ERROR(tree, cur_node, error);
        }

        cur_node->val.prop_name = cur_name;
    }

    else if (IsInitialise(cur_node))
    {
        Node *named_node = cur_node->left->left;
        assert(named_node->type == VAR || named_node->type == FUNC);

        ProperName *cur_name = FindNameInBlock(block, named_node->val.prop_name->name);
        // fprintf(stderr, "use of inited var named '%s', num = %lld\n", cur_name->name, cur_name->number);

        if (cur_name != NULL)
        {
            char error[ERROR_NAME_LEN] = {};

            if (named_node->type == VAR)
                sprintf(error, "Redeclared of variable '%s'", cur_name->name);

            else
                sprintf(error, "Redeclared of function '%s'", cur_name->name);

            SYNTAX_ERROR(tree, named_node, error);
        }

        if (named_node->type == VAR)
        {
            fprintf(stderr, "i init the var named '%s'\n", named_node->val.prop_name->name);
                        TREE_DUMP(tree);

            named_node->val.prop_name = NewNameInTable(table, named_node->val.prop_name->name);

            fprintf(stderr, "i donedone init the var named '%s'\n", named_node->val.prop_name->name);
            TREE_DUMP(tree);
        }

        else
            named_node->val.prop_name = NewNameInTable(&tree->names_table, named_node->val.prop_name->name);

        named_node->val.prop_name->is_init = true;

        GetBlockNamesTable(tree, block, cur_node->right);
    }

    else  //if (cur_node->type == KEY_WORD && cur_node->val.key_word->name == NEW_EXPR)
    {
        GetBlockNamesTable(tree, block, cur_node->left);
        GetBlockNamesTable(tree, block, cur_node->right);
    }


}

void MakeNamesTablesForBlocks(Tree *tree, Node *cur_node)
{
    assert(tree);

    if (cur_node == NULL)
        return;

    if (cur_node->type == KEY_WORD && cur_node->val.key_word->name == NEW_FUNC)
    {
        Node *block_node = cur_node->left->right;

        block_node->val.block.shift = 0;

        Node *func_node = cur_node->left->left->left;
        func_node->val.prop_name->is_init = true;
        func_node->val.prop_name->args_count = GetCountOfArgs(cur_node->left->left);

        fprintf(stderr, "count of args = %lld\n\n", func_node->val.prop_name->args_count);

        TREE_DUMP(tree);
        fprintf(stderr, "\nBEFORE GetBlockNamesTable!!!\n\n\n\n\n\n");
        GetBlockNamesTable(tree, block_node, cur_node->left->left->right);
        fprintf(stderr, "\nAFTER GetBlockNamesTable!!!\n\n\n\n\n\n");
        TREE_DUMP(tree);

        MakeNamesTablesForBlocks(tree, block_node);

        fprintf(stderr, "size of names table = %lld\n", block_node->val.block.names_table.size);
        for (size_t i = 0; i < block_node->val.block.names_table.size; i++)
        {
            fprintf(stderr, "init node = '%s', num = %lld\n", block_node->val.block.names_table.names[i].name, block_node->val.block.names_table.names[i].number);
        }

        TREE_DUMP(tree);
        MakeNamesTablesForBlocks(tree, cur_node->right);
        TREE_DUMP(tree);
    }

    else if (cur_node->type == NEW_BLOCK)
    {
        cur_node->val.block.prev_block = tree->cur_block;

        if (tree->cur_block == NULL)
            cur_node->val.block.shift = 0;
        else
            cur_node->val.block.shift = tree->cur_block->val.block.shift + tree->cur_block->val.block.names_table.size;

        tree->cur_block = cur_node;

        GetBlockNamesTable(tree, cur_node, cur_node->left);

        for (size_t i = 0; i < cur_node->val.block.names_table.size; i++)
        {
            fprintf(stderr, "init node = '%s', num = %lld\n", cur_node->val.block.names_table.names[i].name, cur_node->val.block.names_table.names[i].number + cur_node->val.block.shift);
        }

        tree->cur_block = tree->cur_block->val.block.prev_block;  
    }

    else
    {
        MakeNamesTablesForBlocks(tree, cur_node->left);
        MakeNamesTablesForBlocks(tree, cur_node->right);
    }
}

Node *GetNodeInfoBySymbol(char *sym, Tree *tree, Node *cur_node, SymbolMode mode)
{
    const MathOperation *math_op  = GetOperationBySymbol  (sym, mode);
    const KeyWord       *key_word = GetKeyWordBySymbol    (sym, mode);
    const ManageElem    *manager  = GetManageElemBySymbol (sym, mode);

    if (math_op != NULL)
    {
        cur_node->type        = MATH_OP;
        cur_node->val.math_op = math_op;
    }

    else if (key_word != NULL)
    {
        cur_node->type         = KEY_WORD;
        cur_node->val.key_word = key_word;

    }
    else if (manager != NULL)
    {
        cur_node->type        = MANAGER;
        cur_node->val.manager = manager;
    }

    else if (strcmp(sym, NEW_BLOCK_SYM) == 0)
    {
        cur_node->type = NEW_BLOCK;
    }

    else if (strcmp(sym, "NULL") == 0)
    {
        cur_node = NULL;
    }

    else if (isdigit(*sym))
    {
        cur_node->type = NUM;
        if(sscanf(sym, TREE_ELEM_SCANF_SPECIFIER, &cur_node->val.num) != 1)
            fprintf(stderr, "invalid val of number in GetNodeInfoBySymbol()\n");
    }

    else
    {
        cur_node->type = VAR_OR_FUNC;

        ProperName *prop_name = FindNameInTable(&tree->names_table, sym);

        if (prop_name == NULL)
            cur_node->val.prop_name  = NewNameInTable(&tree->names_table, sym);

        else cur_node->val.prop_name = prop_name;
    }

    return cur_node;
}

size_t GetCountOfArgs(Node *func_node)
{
    size_t res_count = 0;

    Node *cur_comma = func_node->right;

    while (cur_comma != NULL)
    {
        res_count++;
        cur_comma = cur_comma->right;
    }

    return res_count;
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

        // if (cur_node->type == MATH_OP && GetOperationByNode(cur_node)->type == BINARY)
        if (cur_node->type == MATH_OP && cur_node->val.math_op->type == BINARY)       // ����� �� ��������� ���� � �� �� ����� ������ ��� ������� ��������
            right_subtree_cont_type = SubtreeContainsType(cur_node->right, type);

        return (left_subtree_cont_type || right_subtree_cont_type);
    }
}

void SyntaxError(Tree *tree, Node *cur_node, const char *error, const char *file, int line, const char *func)
{
    assert(tree);
    assert(cur_node);
    assert(error);
    assert(file);
    assert(func);

    TREE_DUMP(tree);

    fprintf(stderr, "SyntaxError called in %s:%d %s()\n"
                    "Error: %s   ( position %lld:%lld )",
                    file, line, func, error, cur_node->born_line, cur_node->born_column);

                    // Syntax error: forgot to put ) here (file ...,line ...)   // TODO ??
                        // int main (int argc, char *argv[]   {
                                                        //  ^ 
    abort();
}

bool OpNodeIsCommutativity(Node *op_node)
{
    assert(op_node);

    if (op_node->type == MATH_OP && (op_node->val.math_op->num == ADD || op_node->val.math_op->num == MUL))
        return true;
    
    else return false;
}

bool IsInitialise(Node *node)
{
    assert(node);
    return (node->type == KEY_WORD && (node->val.key_word->name == INT_INIT || node->val.key_word->name == DOUBLE_INIT));
}

bool IsBool(Node *node)
{
    const MathOperation *math_op = node->val.math_op;
    return (node->type == MATH_OP && (math_op->num == BOOL_EQ || math_op->num == BOOL_NEQ || math_op->num == BOOL_GREATER || math_op->num == BOOL_LOWER));
}