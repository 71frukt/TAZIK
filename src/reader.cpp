#include <stdio.h>
#include <assert.h>
#include <iostream>
#include <string.h>

#include "tree.h"
#include "debug.h"
#include "operations.h"
#include "reader.h"

FILE *GetInputFile(const int argc, const char *argv[])
{
    FILE *input_file = NULL;

    if (argc < 2)
        input_file = fopen(BASE_INPUT_FILE_NAME, "r");
    
    else
        input_file = fopen(argv[1], "w");

    return input_file;   
}

void BuildTree(Tree *tree, FILE *source)
{
    MakeTokens(tree, source);
    GetFuncInit(tree);
}

void MakeTokens(Tree *tree, FILE *source)
{
    size_t cur_line   = 0;
    size_t cur_column = 0;

    char start_ch = (char) getc(source);
    ungetc(start_ch, source);

    while (start_ch != EOF)
    {
        fprintf(stderr, "col = %lld, line = %lld\n", cur_column, cur_line);
        fprintf(stderr, "real col = %ld\n\n", ftell(source));

        start_ch = (char) getc(source);
        
        if (start_ch == '\n')
        {
            cur_line++;
            cur_column = 0;
            continue;
        }

        else if (isspace(start_ch))
        {
            cur_column++;
            continue;
        }

        if (isdigit(start_ch))
        {
            ungetc(start_ch, source);
            int shift = 0;

            TreeElem_t val = POISON_VAL;

            fscanf(source, TREE_ELEM_SCANF_SPECIFIER "%n", &val, &shift);
            cur_column += shift;

            Node *new_node = NewNode(tree, NUM, {.num = val}, NULL, NULL);
            new_node->born_column = cur_column;
            new_node->born_line   = cur_line;
        }

        else
        {
            char cur_token[TOKEN_STR_LEN] = {};

            ungetc(start_ch, source);
            int shift = 0;

            fscanf(source, "%s%n", cur_token, &shift);
            cur_column += shift;

            // if (strcmp(cur_token, EOT_SYMBOL) == 0)
            // {
            //     NewNode(tree, EOT, {}, NULL, NULL);
            //     break;
            // }

            Node *new_node = GetNamedToken(tree, cur_token);

            if (new_node != NULL)
            {
                new_node->born_column = cur_column;
                new_node->born_line   = cur_line;
            }
        }
    }

    LANG_DUMP(tree);
}

Node *GetNamedToken(Tree *tree, char *token_name)
{
    fprintf(stderr, "token_name = '%s'\n", token_name);

    if (IsEngLetter(*token_name))                                                       // это имя функции или переменной
    {
        ProperName  *cur_name_ptr = FindNameInTable(&tree->names_table, token_name);

        if (cur_name_ptr == NULL)
            cur_name_ptr = NewNameInTable(&tree->names_table, token_name);

        return NewNode(tree, VAR_OR_FUNC, {.prop_name = cur_name_ptr->name}, NULL, NULL);      // TODO: добавить функции, не только переменные
    }

    else                                                                                // имя операнда или рандомный вкид
    {
        const Operation   *cur_op    = GetOperationBySymbol  (token_name);
        const KeyWord     *key_word  = GetKeyWordBySymbol    (token_name);
        const ManageElem  *manage_el = GetManageElemBySymbol (token_name);

        if (cur_op != NULL)
        {
            return NewNode(tree, OP, {.op = cur_op}, NULL, NULL);
        }

        else if (key_word != NULL)
        {
            return NewNode(tree, KEY_WORD, {.key_word = key_word}, NULL, NULL);
        }

        else if (manage_el != NULL)
        {
            return NewNode(tree, MANAGER, {.manager = manage_el}, NULL, NULL);
        }
    }

    return NULL;
}

bool IsEngLetter(char ch)
{
    return ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z'));
}

Node *GetFuncInit(Tree *dest_tree)
{
    assert(dest_tree);
    LANG_DUMP(dest_tree);

    size_t ip = 0;
    Node **tokens = dest_tree->node_ptrs;

    if (!IsInitialise(tokens[ip]) || tokens[ip + 1]->type != VAR_OR_FUNC)
        SYNTAX_ERROR(dest_tree, tokens[ip], "init of func");
  
    Node *func_init = tokens[ip++];
    Node *func_node = tokens[ip++];
    func_node->type = FUNC;

    if (tokens[ip]->type != MANAGER || tokens[ip]->val.manager->name != OPEN_EXPR_BRACKET)
        SYNTAX_ERROR(dest_tree, tokens[ip], "open bracket in declaring arguments");

    RemoveNode(dest_tree, &tokens[ip++]);

    if (!IsInitialise(tokens[ip]) || tokens[ip + 1]->type != VAR_OR_FUNC)
        SYNTAX_ERROR(dest_tree, tokens[ip], "init of arguments");

    Node *arg = tokens[ip++];               // int/double
    arg->left = tokens[ip++];               // name of var
    arg->left->type = VAR;

    func_init->left = NewNode(dest_tree, TYPE_INDICATOR, {.type_indicator = FUNC_TYPE}, func_node, arg);

    if (tokens[ip]->type != MANAGER || tokens[ip]->val.manager->name != CLOSE_EXPR_BRACKET)
        SYNTAX_ERROR(dest_tree, tokens[ip], "close bracket in declaring arguments");

    RemoveNode(dest_tree, &tokens[ip++]);

    func_init->right = GetBlock(dest_tree, &ip);

    if (tokens[ip]->type != MANAGER || tokens[ip]->val.manager->name != EOT)
        SYNTAX_ERROR(dest_tree, tokens[ip], "EOT");

    RemoveNode(dest_tree, &tokens[ip]);

    return func_init;
}

Node *GetBlock(Tree *dest_tree, size_t *ip)
{
    assert(dest_tree);
    LANG_DUMP(dest_tree);
fprintf(stderr, "new block\n");
    Node **tokens  = dest_tree->node_ptrs;

    if (tokens[*ip]->type != MANAGER || tokens[*ip]->val.manager->name != OPEN_BLOCK_BRACKET)
        SYNTAX_ERROR(dest_tree, tokens[*ip], "open block bracket");

    RemoveNode(dest_tree, &tokens[(*ip)++]);
    Node *this_block = GetVarInit(dest_tree, ip);
    Node *cur_top  = this_block;
    Node *cur_line = NULL;
    Node *cur_expr = NULL;

    // if (tokens[*ip]->type == KEY_WORD && tokens[*ip]->val.key_word->name == NEW_BLOCK)
    // {
    //     cur_top->left = GetBlock(dest_tree, ip);
    // }

    // else
    // {
        cur_expr = GetVarInit(dest_tree, ip);

        if (tokens[*ip]->type != KEY_WORD || tokens[*ip]->val.key_word->name != NEW_EXPR)
            SYNTAX_ERROR(dest_tree, tokens[*ip], KeyWords[NEW_EXPR].symbol);

        cur_line = tokens[(*ip)++];
        cur_line->left = cur_expr;
        cur_top->left  = cur_line;
        // dest_tree->root_ptr = cur_line;
    // }


    while (tokens[*ip]->type != MANAGER || tokens[*ip]->val.manager->name != CLOSE_BLOCK_BRACKET)
    {
        // Node *new_block = NULL;

        // if (tokens[*ip]->type == KEY_WORD && tokens[*ip]->val.key_word->name == NEW_BLOCK)
        // {
        //     new_block = GetBlock(dest_tree, ip);
        //     cur_top->right  = new_block;
        //     cur_top = new_block;
        // }
        
        // else
        // {
            Node *new_expr = GetVarInit(dest_tree, ip);

            if (tokens[*ip]->type != KEY_WORD || tokens[*ip]->val.key_word->name != NEW_EXPR)
                SYNTAX_ERROR(dest_tree, tokens[*ip], KeyWords[NEW_EXPR].symbol);

            Node *new_line = tokens[(*ip)++];
            new_line->left  = new_expr;

            cur_top->right = new_line;
            cur_top        = new_line;
            
        // }
    }

    RemoveNode(dest_tree, &tokens[(*ip)++]);

    return this_block;
}

Node *GetVarInit(Tree *dest_tree, size_t *ip)
{
    assert(dest_tree);

    Node **tokens  = dest_tree->node_ptrs;

    if (!IsInitialise(tokens[*ip]))
        return GetIf(dest_tree, ip);

    Node *init_node = tokens[(*ip)++];

    if (tokens[*ip]->type != VAR_OR_FUNC)
        SYNTAX_ERROR(dest_tree, tokens[*ip], "type of VAR");

    init_node->left = NewNode(dest_tree, TYPE_INDICATOR, {.type_indicator = VAR_TYPE}, tokens[(*ip)++], NULL);

    if (!(tokens[*ip]->type == KEY_WORD && tokens[*ip]->val.key_word->name == ASSIGN))
        SYNTAX_ERROR(dest_tree, tokens[*ip], "assign node");

    RemoveNode(dest_tree, &tokens[(*ip)++]);    // при инициализации нода присваивания в дереве не нужна

    init_node->right = GetSum(dest_tree, ip);
    
    return init_node;
}

Node *GetIf(Tree *dest_tree, size_t *ip)
{
    Node **tokens  = dest_tree->node_ptrs;

    if (tokens[*ip]->type != KEY_WORD || tokens[*ip]->val.key_word->name != IF)
        return GetSum(dest_tree, ip);

    Node *if_node = tokens[(*ip)++];

    if (tokens[*ip]->type != MANAGER || tokens[*ip]->val.manager->name != OPEN_EXPR_BRACKET)
        SYNTAX_ERROR(dest_tree, tokens[*ip], "open bracket for condition");
    RemoveNode(dest_tree, &tokens[(*ip)++]);

    if_node->left = GetSum(dest_tree, ip);

    if (tokens[*ip]->type != MANAGER || tokens[*ip]->val.manager->name != CLOSE_EXPR_BRACKET)
        SYNTAX_ERROR(dest_tree, tokens[*ip], "close bracket for condition");
    RemoveNode(dest_tree, &tokens[(*ip)++]);

    if_node->right = GetBlock(dest_tree, ip);

    return if_node;
}

Node *GetSum(Tree *dest_tree, size_t *ip)
{
    Node **tokens  = dest_tree->node_ptrs;
fprintf(stderr, "first arg, ip = %lld\n", *ip);
    Node *res_node = GetMul(dest_tree, ip);
    while (tokens[*ip]->type == OP && (tokens[*ip]->val.op->num == ADD || tokens[*ip]->val.op->num == SUB))
    {
fprintf(stderr, "second arg\n");


        Node *arg_1 = res_node;
        res_node    = tokens[(*ip)++];
        Node *arg_2 = GetMul(dest_tree, ip);

        res_node->left  = arg_1;
        res_node->right = arg_2;
    }

    return res_node;
}

Node *GetMul(Tree *dest_tree, size_t *ip)
{
    Node **tokens  = dest_tree->node_ptrs;
    Node *res_node = GetPow(dest_tree, ip);

    while (tokens[*ip]->type == OP && (tokens[*ip]->val.op->num == MUL || tokens[*ip]->val.op->num == DIV))
    {
        Node *arg_1 = res_node;
        res_node    = tokens[(*ip)++];
        Node *arg_2 = GetPow(dest_tree, ip);

        res_node->left  = arg_1;
        res_node->right = arg_2;
    }

    return res_node;
}

Node *GetPow(Tree *dest_tree, size_t *ip)
{
    Node **tokens  = dest_tree->node_ptrs;
    Node *res_node = GetOp(dest_tree, ip);

    Node *cur_op = tokens[*ip];

    if (cur_op->type != OP)
        return res_node;

    if (cur_op->val.op->num == DEG)
    {
        Node *basis = res_node;
        res_node = tokens[(*ip)++];
        Node *degree = GetPow(dest_tree, ip);
        
        res_node->left  = basis;
        res_node->right = degree;

        return res_node;
    }

    else
        return res_node;
}

Node *GetOp(Tree *dest_tree, size_t *ip)              // f(..) или f(.. , ..)
{
    Node **tokens = dest_tree->node_ptrs;

    if (tokens[*ip]->type != OP)
        return GetSumInBrackets(dest_tree, ip);

    Node *op_node = tokens[(*ip)++];

    if (op_node->val.op->form == INFIX)
        SYNTAX_ERROR(dest_tree, op_node, "prefix form of operand");

    if (op_node->val.op->type == UNARY)
    {
        Node *arg = GetSumInBrackets(dest_tree, ip);
        op_node->left  = arg;
        op_node->right = arg;

        return op_node;
    }

    else
    {
        bool is_OPEN_EXPR_BRACKET = tokens[*ip]->type == MANAGER && tokens[*ip]->val.manager->name == OPEN_EXPR_BRACKET;
        if (!is_OPEN_EXPR_BRACKET)
            SYNTAX_ERROR(dest_tree, tokens[*ip], Managers[OPEN_EXPR_BRACKET].symbol);
        RemoveNode(dest_tree, &tokens[*ip]);
        (*ip)++;

        Node *arg_1 = GetSum(dest_tree, ip);

        bool is_comma =  tokens[*ip]->type == MANAGER && tokens[*ip]->val.manager->name == COMMA;
        if (!is_comma)
            SYNTAX_ERROR(dest_tree, tokens[*ip], Managers[COMMA].symbol);
        RemoveNode(dest_tree, &tokens[*ip]);
        (*ip)++;

        Node *arg_2 = GetSum(dest_tree, ip);

        bool is_CLOSE_EXPR_BRACKET = tokens[*ip]->type == MANAGER && tokens[*ip]->val.manager->name == CLOSE_EXPR_BRACKET;
        if (!is_CLOSE_EXPR_BRACKET)
            SYNTAX_ERROR(dest_tree, tokens[*ip], Managers[CLOSE_EXPR_BRACKET].symbol);
        RemoveNode(dest_tree, &tokens[*ip]);
        (*ip)++;

        op_node->left  = arg_1;
        op_node->right = arg_2;

        return op_node;       
    }
}

Node *GetSumInBrackets(Tree *dest_tree, size_t *ip)
{
    Node **tokens = dest_tree->node_ptrs;

    if (tokens[*ip]->type == MANAGER && tokens[*ip]->val.manager->name == OPEN_EXPR_BRACKET)
    {
        RemoveNode(dest_tree, &tokens[*ip]);
        (*ip)++;

        Node *res_node = GetSum(dest_tree, ip);

        bool is_CLOSE_EXPR_BRACKET = tokens[*ip]->type == MANAGER && tokens[*ip]->val.manager->name == CLOSE_EXPR_BRACKET;

        if (!is_CLOSE_EXPR_BRACKET)
            SYNTAX_ERROR(dest_tree, tokens[*ip], Managers[CLOSE_EXPR_BRACKET].symbol);

        RemoveNode(dest_tree, &tokens[*ip]);
        (*ip)++;
        
        return res_node;
    }
    
    else
        return GetVarOrFunc(dest_tree, ip);
}

Node *GetVarOrFunc(Tree *dest_tree, size_t *ip)
{
    Node **tokens = dest_tree->node_ptrs;

    if (tokens[*ip]->type == VAR_OR_FUNC)
    {
        Node *cur_node = tokens[(*ip)++];
        cur_node->type = VAR;
        TypeIndicator type = VAR_TYPE;
        Node *arg = NULL;

        if (tokens[*ip]->type == MANAGER && tokens[*ip]->val.manager->name == OPEN_EXPR_BRACKET)     // тогда это функция
        {
            RemoveNode(dest_tree, &tokens[(*ip)++]);

            cur_node->type = FUNC;
            type = FUNC_TYPE;

            arg = GetSum(dest_tree, ip);

            if (tokens[*ip]->type != MANAGER || tokens[*ip]->val.manager->name != CLOSE_EXPR_BRACKET)
                SYNTAX_ERROR(dest_tree, tokens[*ip], Managers[CLOSE_EXPR_BRACKET].symbol);

            RemoveNode(dest_tree, &tokens[(*ip)++]);
        }

        return NewNode(dest_tree, TYPE_INDICATOR, {.type_indicator = type}, cur_node, arg);
    }

    else
    {
        return GetNumber(dest_tree, ip);
    }
}

Node *GetNumber(Tree *dest_tree, size_t *ip)
{
    Node **tokens = dest_tree->node_ptrs;

    if (tokens[*ip]->type == NUM)
    {
        return tokens[(*ip)++];
    }

    else
    {
        SYNTAX_ERROR(dest_tree, tokens[*ip], "type of NUM or VAR");
        return NULL;
    }
}

void SyntaxError(Tree *tree, Node *cur_node, const char *expected_token, const char *file, int line, const char *func)
{
    LANG_DUMP(tree);

    fprintf(stderr, "SyntaxError called in %s:%d %s()\n"
                    "Maybe forgot to put %s here ( position %lld:%lld )",
                    file, line, func, expected_token, cur_node->born_line, cur_node->born_column);

                    // Syntax error: forgot to put ) here (file ...,line ...)
                        // int main (int argc, char *argv[]   {
                                                        //  ^ 
    abort();
}