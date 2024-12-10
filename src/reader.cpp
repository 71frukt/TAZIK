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
    GetBlock(tree);
}

void MakeTokens(Tree *tree, FILE *source)
{
    size_t cur_line   = 0;
    size_t cur_column = 0;

    while (true)
    {
        fprintf(stderr, "col = %lld, line = %lld\n", cur_column, cur_line);
        fprintf(stderr, "real col = %ld\n\n", ftell(source));

        char start_ch = (char) getc(source);
        
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

            NewNode(tree, NUM, {.num = val}, NULL, NULL);
        }

        else
        {
            char cur_token[TOKEN_STR_LEN] = {};

            ungetc(start_ch, source);
            int shift = 0;

            fscanf(source, "%s%n", cur_token, &shift);
            cur_column += shift;

            if (strcmp(cur_token, EOT_SYMBOL) == 0)
            {
                NewNode(tree, EOT, {}, NULL, NULL);
                break;
            }

            GetNamedToken(tree, cur_token);
        }
    }
}

void GetNamedToken(Tree *tree, char *token_name)
{
    fprintf(stderr, "token_name = '%s'\n", token_name);

    if (IsEngLetter(*token_name))                                               // это имя функции или переменной
    {
        ProperName  *cur_name_ptr = FindNameInTable(&tree->names_table, token_name);

        if (cur_name_ptr == NULL)
            cur_name_ptr = NewNameInTable(&tree->names_table, token_name);

        NewNode(tree, VAR, {.prop_name = cur_name_ptr->name}, NULL, NULL);      // TODO: добавить функции, не только переменные
    }

    else                                                                        // имя операнда или рандомный вкид
    {
        const Operation   *cur_op    = GetOperationBySymbol  (token_name);
        const Initializer *init      = GetInitBySymbol       (token_name);
        const ManageElem  *manage_el = GetManageElemBySymbol (token_name);

        if (cur_op != NULL)
        {
            NewNode(tree, OP, {.op = cur_op}, NULL, NULL);
        }

        else if (init != NULL)
        {
            NewNode(tree, INIT, {.init = init}, NULL, NULL);
        }

        else if (manage_el != NULL)
        {
            NewNode(tree, MANAGER, {.manager = manage_el}, NULL, NULL);
        }

        else if (strcmp(token_name, NEW_EXPR_SYMBOL) == 0)
        {
            NewNode(tree, NEW_EXPR, {}, NULL, NULL);
        }
    }
}

bool IsEngLetter(char ch)
{
    return ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z'));
}

Node *GetBlock(Tree *dest_tree)
{
    assert(dest_tree);
    LANG_DUMP(dest_tree);

    size_t ip = 0;

    Node **tokens  = dest_tree->node_ptrs;
    Node *expr = GetInit(dest_tree, &ip);

    if (tokens[ip]->type != NEW_EXPR)
        SYNTAX_ERROR(dest_tree, ip, NodeValToStr(tokens[ip]), NEW_EXPR_SYMBOL);

    Node *cur_line = tokens[ip++];
    cur_line->left = expr;
    dest_tree->root_ptr = cur_line;

    while (tokens[ip]->type != EOT)
    {
        Node *new_expr = GetInit(dest_tree, &ip);

        if (tokens[ip]->type != NEW_EXPR)
            SYNTAX_ERROR(dest_tree, ip, NodeValToStr(tokens[ip]), NEW_EXPR_SYMBOL);

        Node *new_line = tokens[ip++];
        new_line->left = new_expr;

        cur_line->right = new_line;
        cur_line        = new_line;
    }

    RemoveNode(dest_tree, &tokens[ip]);

    return dest_tree->root_ptr;
}

Node *GetInit(Tree *dest_tree, size_t *ip)
{
    assert(dest_tree);

    Node **tokens  = dest_tree->node_ptrs;

    if (tokens[*ip]->type != INIT)
        return GetSum(dest_tree, ip);

    Node *init_node = tokens[(*ip)++];

    if (tokens[*ip]->type != VAR)
        SYNTAX_ERROR(dest_tree, *ip, NodeValToStr(tokens[*ip]), "type of VAR");

    init_node->left = GetVarOrFunc(dest_tree, ip);

    if (tokens[*ip]->type != OP || tokens[*ip]->val.op->num != ASSIGN)
        SYNTAX_ERROR(dest_tree, *ip, NodeValToStr(tokens[*ip]), "assign node");

    RemoveNode(dest_tree, &tokens[(*ip)++]);    // при инициализации нода присваивания в дереве не нужна
    init_node->right = GetSum(dest_tree, ip);

    return init_node;
}

Node *GetSum(Tree *dest_tree, size_t *ip)
{
    Node **tokens  = dest_tree->node_ptrs;
    Node *res_node = GetMul(dest_tree, ip);

    while (tokens[*ip]->type == OP && (tokens[*ip]->val.op->num == ADD || tokens[*ip]->val.op->num == SUB))
    {
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
    Node *res_node = GetFunc(dest_tree, ip);

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

Node *GetFunc(Tree *dest_tree, size_t *ip)              // f(..) или f(.. , ..)
{
    Node **tokens = dest_tree->node_ptrs;

    if (tokens[*ip]->type != OP)
        return GetInitInBrackets(dest_tree, ip);

    else
    {
        Node *op_node = tokens[(*ip)++];

        if (op_node->val.op->form == INFIX)
            SYNTAX_ERROR(dest_tree, *ip, NodeValToStr(op_node), "prefix form of operand");

        if (op_node->val.op->type == UNARY)
        {
            Node *arg = GetInitInBrackets(dest_tree, ip);
            op_node->left  = arg;
            op_node->right = arg;

            return op_node;
        }

        else
        {
            bool is_open_bracket = tokens[*ip]->type == MANAGER && tokens[*ip]->val.manager->name == OPEN_BRACKET;
            if (!is_open_bracket)
                SYNTAX_ERROR(dest_tree, *ip, NodeValToStr(tokens[*ip]), Managers[OPEN_BRACKET].symbol);
            RemoveNode(dest_tree, &tokens[*ip]);
            (*ip)++;

            Node *arg_1 = GetSum(dest_tree, ip);

            bool is_comma =  tokens[*ip]->type == MANAGER && tokens[*ip]->val.manager->name == COMMA;
            if (!is_comma)
                SYNTAX_ERROR(dest_tree, *ip, NodeValToStr(tokens[*ip]), Managers[COMMA].symbol);
            RemoveNode(dest_tree, &tokens[*ip]);
            (*ip)++;

            Node *arg_2 = GetSum(dest_tree, ip);


           bool is_close_bracket = tokens[*ip]->type == MANAGER && tokens[*ip]->val.manager->name == CLOSE_BRACKET;
            if (!is_close_bracket)
                SYNTAX_ERROR(dest_tree, *ip, NodeValToStr(tokens[*ip]), Managers[CLOSE_BRACKET].symbol);
            RemoveNode(dest_tree, &tokens[*ip]);
            (*ip)++;

            op_node->left  = arg_1;
            op_node->right = arg_2;

            return op_node;
        }        
    }
}

Node *GetInitInBrackets(Tree *dest_tree, size_t *ip)
{
    Node **tokens = dest_tree->node_ptrs;

    if (tokens[*ip]->type == MANAGER && tokens[*ip]->val.manager->name == OPEN_BRACKET)
    {
        RemoveNode(dest_tree, &tokens[*ip]);
        (*ip)++;

        Node *res_node = GetSum(dest_tree, ip);

        bool is_close_bracket = tokens[*ip]->type == MANAGER && tokens[*ip]->val.manager->name == CLOSE_BRACKET;

        if (!is_close_bracket)
            SYNTAX_ERROR(dest_tree, *ip, NodeValToStr(tokens[*ip]), Managers[CLOSE_BRACKET].symbol);

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

    if (tokens[*ip]->type == VAR)           // TODO
    {
        return NewNode(dest_tree, NAMED_NODE_TYPE, {.named_node_type = VAR_TYPE}, tokens[(*ip)++], NULL);
    }

    else
    {
        return GetNumber(dest_tree, ip);
    }
}

Node *GetNumber(Tree *dest_tree, size_t *ip)
{
    Node **tokens = dest_tree->node_ptrs;

    if (tokens[*ip]->type == VAR || tokens[*ip]->type == NUM)
    {
        return tokens[(*ip)++];
    }

    else
    {
        SYNTAX_ERROR(dest_tree, *ip, NodeValToStr(tokens[*ip]), "type of NUM or VAR");
        return NULL;
    }
}

void SyntaxError(Tree *tree, size_t ip, const char *cur_token, const char *expected_token, const char *file, int line, const char *func)
{
    LANG_DUMP(tree);

    fprintf(stderr, "SyntaxError called in %s:%d %s(), ip = %lld\n"
                    "Current token = '%s', but expected %s      \n", file, line, func, ip, cur_token, expected_token);
    abort();
}