#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "back_reader.h"
#include "../../tree/tree_lib.h"

FILE *GetInputFile(const int argc, const char *argv[])
{
    FILE *input_file = NULL;

    if (argc < 2)
        input_file = fopen(BASE_SAVE_TREE_FILE_NAME, "r");
    
    else
        input_file = fopen(argv[1], "r");

    return input_file;   
}

void BuildTreeByFileData(FILE *source, Tree *tree)
{
    assert(source);
    assert(tree);

    tree->root_ptr = MakeAndLinkNodes(source, tree);

    TREE_DUMP(tree);

    MakeNamesTablesForBlocks(tree, tree->root_ptr);
}

Node *MakeAndLinkNodes(FILE *source, Tree *tree)
{
    char cur_token[TOKEN_LEN] = {};

    if (fscanf(source, "%s", cur_token) == 0 || strcmp(cur_token, "NULL") == 0)
        return NULL;

fprintf(stderr, "token = '%s'\n", cur_token);

    Node *new_node = NewNode(tree, POISON_TYPE, {}, NULL, NULL);

    GetNodeInfoBySymbol(cur_token, tree, new_node, REAL_CODE_MODE);

    if (fscanf(source, "%s", cur_token) != 1 || strcmp(cur_token, "(") != 0)
        fprintf(stderr, "Expected '(' after '%s'\n", cur_token);

    new_node->left = MakeAndLinkNodes(source, tree);

    if (fscanf(source, "%s", cur_token) != 1 || strcmp(cur_token, ";") != 0)
        fprintf(stderr, "Expected ';' after '%s'\n", cur_token);

    new_node->right = MakeAndLinkNodes(source, tree);

    if (fscanf(source, "%s", cur_token) != 1 || strcmp(cur_token, ")") != 0)
        fprintf(stderr, "Expected ')' after '%s'\n", cur_token);

    if (new_node->type == KEY_WORD && new_node->val.key_word->name == FUNC_T_INDICATOR)
        new_node->left->type = FUNC;
    
    else if (new_node->type == KEY_WORD && new_node->val.key_word->name == VAR_T_INDICATOR)
        new_node->left->type = VAR;

    return new_node;
}