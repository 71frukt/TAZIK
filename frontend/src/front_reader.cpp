#include <stdio.h>
#include <assert.h>
#include <iostream>
#include <string.h>
#include <stdlib.h>

#include "../../tree/tree_lib.h"
#include "front_reader.h"
#include "semantic_anal.h"

FILE *GetInputCodeFile(const int argc, const char *argv[])
{
    FILE *input_file = NULL;

    if (argc < 2)
        input_file = fopen(BASE_INPUT_CODE_FILE_NAME, "r");
    
    else
        input_file = fopen(argv[1], "r");

    return input_file;   
}

void BuildTreeByCode(Tree *tree, FILE *source)
{
    MakeTokens(tree, source);

    tree->root_ptr = GetCode(tree);

    TREE_DUMP(tree);

    MakeNamesTablesForBlocks(tree, tree->root_ptr);

    TREE_DUMP(tree);
}

void MakeTokens(Tree *tree, FILE *source)
{
    size_t cur_line   = 0;
    size_t cur_column = 0;

    char cur_ch = (char) getc(source);
    ungetc(cur_ch, source);

    while (cur_ch != EOF)
    {
        cur_ch = (char) getc(source);

        if (cur_ch == EOF)
            break;
        
        if (cur_ch == '\n')
        {
            cur_line++;
            cur_column = 0;
            continue;
        }

        else if (isspace(cur_ch))
        {
            cur_column++;
            continue;
        }

        if (isdigit(cur_ch))
        {
            ungetc(cur_ch, source);
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

            ungetc(cur_ch, source);
            int shift = 0;

            fscanf(source, "%s%n", cur_token, &shift);
            cur_column += shift;

            Node *new_node = GetNamedToken(tree, cur_token);

            if (new_node == NULL)
                continue;

            new_node->born_column = cur_column;
            new_node->born_line   = cur_line;

            cur_ch = (char) getc(source);
            while (isspace(cur_ch))
            {
                if (cur_ch == '\n')
                {
                    cur_line++;
                    cur_column = 0;
                }

                cur_column++;
                cur_ch = (char) getc(source);
            }
            ungetc(cur_ch, source);

            char next_token[TOKEN_STR_LEN] = {};
            fscanf(source, "%[^\n]%n", next_token, &shift);
            cur_column += shift;

            if (new_node->type == MANAGER && new_node->val.manager->name == OPEN_BLOCK_BRACKET_P1)
            {
                if (strcmp(Managers[OPEN_BLOCK_BRACKET_P2].my_symbol, next_token) == 0)
                    new_node->val.manager = &Managers[OPEN_BLOCK_BRACKET];

                else
                    SYNTAX_ERROR(tree, new_node, "expected second part of open block token");
            }

            else if (new_node->type == MANAGER && new_node->val.manager->name == CLOSE_BLOCK_BRACKET_P1)
            {
                if (strcmp(Managers[CLOSE_BLOCK_BRACKET_P2].my_symbol, next_token) == 0)
                    new_node->val.manager = &Managers[CLOSE_BLOCK_BRACKET];

                else
                    SYNTAX_ERROR(tree, new_node, "expected second part of close block token");
            }

            else
            {
                fseek(source, -shift, SEEK_CUR);
                cur_column -= shift;
            }
        }
    }

    NewNode(tree, MANAGER, {.manager = &Managers[EOT]}, NULL, NULL);
fprintf(stderr, "tree size = %lld\n\n", tree->size);
    TREE_DUMP(tree);
}

Node *GetNamedToken(Tree *tree, char *token_name)
{
    fprintf(stderr, "cur_token = '%s'\n", token_name);

    if (IsEngLetter(*token_name))                                                       // ��� ��� ������� ��� ����������
    {
        ProperName *cur_name_ptr = FindNameInTable(&tree->names_table, token_name);

        if (cur_name_ptr == NULL)
        {
            cur_name_ptr = NewNameInTable(&tree->names_table, token_name);
        }

        return NewNode(tree, VAR_OR_FUNC, {.prop_name = cur_name_ptr}, NULL, NULL);
    }

    else                                                                                // ��� �������� ��� ��������� ����
    {
        const MathOperation *cur_op    = GetOperationBySymbol  (token_name, MY_CODE_MODE);
        const KeyWord       *key_word  = GetKeyWordBySymbol    (token_name, MY_CODE_MODE);
        const ManageElem    *manage_el = GetManageElemBySymbol (token_name, MY_CODE_MODE);

        if (cur_op != NULL)
        {
            return NewNode(tree, MATH_OP, {.math_op = cur_op}, NULL, NULL);
        }

        else if (key_word != NULL)
        {
            return NewNode(tree, KEY_WORD, {.key_word = key_word}, NULL, NULL);
        }

        else if (manage_el != NULL)
        {
            if (manage_el->name >= DIGIT_ZERO && manage_el->name <= DIGIT_NINE)
            {
                return NewNode(tree, NUM, {.num = *manage_el->real_symbol - '0'}, NULL, NULL);
            }

            else
                return NewNode(tree, MANAGER, {.manager = manage_el}, NULL, NULL);
        }

        // Node *new_node = NewNode(tree, POISON_TYPE, {}, NULL, NULL);
        // return GetNodeInfoBySymbol(token_name, tree, new_node, MY_CODE_MODE);
    }

    return NULL;
}

bool IsEngLetter(char ch) // isalpha
{
    return ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z'));
}

bool IsCapitalLetter(char letter)
{
    return (letter >= '�' && letter <= '�' );
}

Node *GetCode(Tree *dest_tree)
{
    assert(dest_tree);

    size_t ip = 0;

    Node **tokens  = dest_tree->node_ptrs;
    Node *res_code = GetFuncInit(dest_tree, &ip);
    Node *cur_node = res_code;

    while ( !(tokens[ip]->type == MANAGER && tokens[ip]->val.manager->name == EOT) )
    {
        fprintf(stderr, "new func: ip = %lld\n", ip);

        Node *new_func = GetFuncInit(dest_tree, &ip);

        cur_node->right = new_func;
        cur_node = new_func;
    }
    
    if (tokens[ip]->type != MANAGER || tokens[ip]->val.manager->name != EOT)
        SYNTAX_ERROR(dest_tree, tokens[ip], "EOT");

    RemoveNode(dest_tree, &tokens[ip]);

    return res_code;    
}

Node *GetFuncInit(Tree *dest_tree, size_t *ip)
{
    fprintf(stderr, "In GetFuncInit()\n");

    assert(dest_tree);
    assert(ip);
    TREE_DUMP(dest_tree);

    Node **tokens = dest_tree->node_ptrs;

    if (!IsInitialise(tokens[*ip]) || tokens[*ip + 1]->type != VAR_OR_FUNC)
        SYNTAX_ERROR(dest_tree, tokens[*ip], "init of func");
  
    Node *func_init = tokens[(*ip)++];
    Node *func_node = tokens[(*ip)++];
    func_node->type = FUNC;

    if (tokens[*ip]->type != MANAGER || tokens[*ip]->val.manager->name != OPEN_EXPR_BRACKET)
        SYNTAX_ERROR(dest_tree, tokens[*ip], "open bracket in declaring arguments");

    RemoveNode(dest_tree, &tokens[(*ip)++]);

    if (!IsInitialise(tokens[*ip]) || tokens[*ip + 1]->type != VAR_OR_FUNC)
        SYNTAX_ERROR(dest_tree, tokens[*ip], "init of arguments");

    Node *arg = GetExprSequence(dest_tree, ip);

    func_init->left = NewNode(dest_tree, KEY_WORD, {.key_word = &KeyWords[FUNC_T_INDICATOR]}, func_node, arg);

    if (tokens[*ip]->type != MANAGER || tokens[*ip]->val.manager->name != CLOSE_EXPR_BRACKET)
        SYNTAX_ERROR(dest_tree, tokens[*ip], "close bracket in declaring arguments");

    RemoveNode(dest_tree, &tokens[(*ip)++]);

    func_init->right = GetBlock(dest_tree, ip);

    return NewNode(dest_tree, KEY_WORD, {.key_word = &KeyWords[NEW_FUNC]}, func_init, NULL);

}

Node *GetBlock(Tree *dest_tree, size_t *ip)
{
                    fprintf(stderr, "In GetBlock()\n");

    assert(dest_tree);
    assert(ip);
    // TREE_DUMP(dest_tree);

    Node **tokens = dest_tree->node_ptrs;

    if (tokens[*ip]->type != MANAGER || tokens[*ip]->val.manager->name != OPEN_BLOCK_BRACKET)
        SYNTAX_ERROR(dest_tree, tokens[*ip], "open block bracket");

    RemoveNode(dest_tree, &tokens[(*ip)++]);

    Node *res_block = GetIf(dest_tree, ip);
    Node *cur_top = res_block;

    while (tokens[*ip]->type != MANAGER || tokens[*ip]->val.manager->name != CLOSE_BLOCK_BRACKET)
    {
        Node *new_top = GetIf(dest_tree, ip);

        cur_top->right = new_top;
        cur_top = new_top;
    }

    Node *new_block_node = NewNode(dest_tree, NEW_BLOCK, {.block = {}}, res_block, NULL);
    RemoveNode(dest_tree, &tokens[(*ip)++]);

    return new_block_node;
}

Node *GetIf(Tree *dest_tree, size_t *ip)
{
                fprintf(stderr, "In GetIf()\n");

    assert(dest_tree);
    assert(ip);

    Node **tokens = dest_tree->node_ptrs;

    if (tokens[*ip]->type != KEY_WORD || tokens[*ip]->val.key_word->name != IF)
        return GetWhile(dest_tree, ip);

    Node *if_node = tokens[(*ip)++];

    if (tokens[*ip]->type != MANAGER || tokens[*ip]->val.manager->name != OPEN_EXPR_BRACKET)
        SYNTAX_ERROR(dest_tree, tokens[*ip], "open bracket for condition");
    RemoveNode(dest_tree, &tokens[(*ip)++]);

    if_node->left = GetBool(dest_tree, ip);

    if (tokens[*ip]->type != MANAGER || tokens[*ip]->val.manager->name != CLOSE_EXPR_BRACKET)
        SYNTAX_ERROR(dest_tree, tokens[*ip], "close bracket for condition");
    RemoveNode(dest_tree, &tokens[(*ip)++]);

    if_node->right = GetBlock(dest_tree, ip);

    return NewNode(dest_tree, KEY_WORD, {.key_word = &KeyWords[NEW_EXPR]}, if_node, NULL);
}

Node *GetWhile(Tree *dest_tree, size_t *ip)
{
            fprintf(stderr, "In GetWhile()\n");

    assert(dest_tree);
    assert(ip);

    Node **tokens = dest_tree->node_ptrs;

    if (tokens[*ip]->type != KEY_WORD || tokens[*ip]->val.key_word->name != WHILE)
        return GetExpr(dest_tree, ip);

    Node *while_node = tokens[(*ip)++];

    if (tokens[*ip]->type != MANAGER || tokens[*ip]->val.manager->name != OPEN_EXPR_BRACKET)
        SYNTAX_ERROR(dest_tree, tokens[*ip], "open bracket for condition");
    RemoveNode(dest_tree, &tokens[(*ip)++]);

    while_node->left = GetBool(dest_tree, ip);

    if (tokens[*ip]->type != MANAGER || tokens[*ip]->val.manager->name != CLOSE_EXPR_BRACKET)
        SYNTAX_ERROR(dest_tree, tokens[*ip], "close bracket for condition");
    RemoveNode(dest_tree, &tokens[(*ip)++]);

    while_node->right = GetBlock(dest_tree, ip);

    return NewNode(dest_tree, KEY_WORD, {.key_word = &KeyWords[NEW_EXPR]}, while_node, NULL);
}

Node *GetExpr(Tree *dest_tree, size_t *ip)              // ��, ��� ���������� ;
{
        fprintf(stderr, "In GetExpr()\n");

    assert(dest_tree);
    assert(ip);

    Node **tokens = dest_tree->node_ptrs;

    Node *expr = GetVarInit(dest_tree, ip);

    if (tokens[*ip]->type != KEY_WORD || tokens[*ip]->val.key_word->name != NEW_EXPR)
        SYNTAX_ERROR(dest_tree, tokens[*ip], "end of expr my_symbol");

    Node *res_line = tokens[(*ip)++];
    res_line->left = expr;

    return res_line;
}

Node *GetVarInit(Tree *dest_tree, size_t *ip)
{
    fprintf(stderr, "In GetVarInit()\n");

    assert(dest_tree);
    assert(ip);
    
    Node **tokens = dest_tree->node_ptrs;

    if (!IsInitialise(tokens[*ip]))
        return GetReturn(dest_tree, ip);

    Node *init_node = tokens[(*ip)++];
    Node *var_node  = GetVarOrFunc(dest_tree, ip);
fprintf(stderr, "Gett var. need arg\n");
    if (var_node->type != KEY_WORD || var_node->val.key_word->name != VAR_T_INDICATOR)
        SYNTAX_ERROR(dest_tree, tokens[*ip], "type of VAR");

    init_node->left = var_node;

    if (!(tokens[*ip]->type == KEY_WORD && tokens[*ip]->val.key_word->name == ASSIGN))
        return init_node;

    RemoveNode(dest_tree, &tokens[(*ip)++]);    // ��� ������������� ���� ������������ � ������ �� �����

    init_node->right = GetBool(dest_tree, ip);
    
    return init_node;
}

Node *GetExprSequence(Tree *dest_tree, size_t *ip)
{
    fprintf(stderr, "In GetExprSequence()\n");

    assert(dest_tree);
    assert(ip);

    Node **tokens   = dest_tree->node_ptrs;
    Node *arg       = GetVarInit(dest_tree, ip);
    Node *cur_comma = NewNode(dest_tree, KEY_WORD, {.key_word = &KeyWords[COMMA]}, arg, NULL);
    Node *res_comma = cur_comma;
TREE_DUMP(dest_tree);
    while (tokens[*ip]->type == KEY_WORD && tokens[*ip]->val.key_word->name == COMMA)
    {
        Node *new_comma = tokens[(*ip)++];
        Node *new_arg = GetVarInit(dest_tree, ip);

        cur_comma->right = new_comma;
        cur_comma = new_comma;
        
        new_comma->left = new_arg;
    }

    return res_comma;
}

Node *GetReturn(Tree *dest_tree, size_t *ip)
{
    fprintf(stderr, "In GetReturn()\n");

    assert(dest_tree);
    assert(ip);

    Node **tokens = dest_tree->node_ptrs;

    if (tokens[*ip]->type != KEY_WORD || tokens[*ip]->val.key_word->name != RETURN)
        return GetAssign(dest_tree, ip);

    Node *ret_node = tokens[(*ip)++];
    ret_node->left = GetBool(dest_tree, ip);

    return ret_node;
}

Node *GetAssign(Tree *dest_tree, size_t *ip)
{
    fprintf(stderr, "In GetAssign()\n");

    assert(dest_tree);
    assert(ip);

    Node **tokens = dest_tree->node_ptrs;

    if (tokens[(*ip) + 1]->type != KEY_WORD || tokens[(*ip) + 1]->val.key_word->name != ASSIGN)
        return GetBool(dest_tree, ip);

    Node *var_node  = GetVarOrFunc(dest_tree, ip);
fprintf(stderr, "ip = %lld\n", *ip);
    if (var_node->type != KEY_WORD || var_node->val.key_word->name != VAR_T_INDICATOR)
        SYNTAX_ERROR(dest_tree, var_node, "an attempt to assign a non-variable");

    Node *init_node = tokens[(*ip)++];
    Node *expr_node = GetBool(dest_tree, ip);

    init_node->left  = var_node;
    init_node->right = expr_node;

    return init_node;
}

Node *GetBool(Tree *dest_tree, size_t *ip)
{
    fprintf(stderr, "In GetBool()\n");

    assert(dest_tree);
    assert(ip);

    Node **tokens  = dest_tree->node_ptrs;
    Node *res_node = GetSum(dest_tree, ip);

    while (tokens[*ip]->type == MATH_OP && IsBool(tokens[*ip]))
    {
        Node *arg_1 = res_node;
        res_node    = tokens[(*ip)++];
        Node *arg_2 = GetSum(dest_tree, ip);

        res_node->left  = arg_1;
        res_node->right = arg_2;
    }

    return res_node;
}

Node *GetSum(Tree *dest_tree, size_t *ip)
{
    fprintf(stderr, "In GetSum()\n");

    assert(dest_tree);
    assert(ip);

    Node **tokens  = dest_tree->node_ptrs;
    Node *res_node = GetMul(dest_tree, ip);

    while (tokens[*ip]->type == MATH_OP && (tokens[*ip]->val.math_op->num == ADD || tokens[*ip]->val.math_op->num == SUB))
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
    fprintf(stderr, "In GetMul()\n");

    assert(dest_tree);
    assert(ip);

    Node **tokens  = dest_tree->node_ptrs;
    Node *res_node = GetPow(dest_tree, ip);

    while (tokens[*ip]->type == MATH_OP && (tokens[*ip]->val.math_op->num == MUL || tokens[*ip]->val.math_op->num == DIV))
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
    fprintf(stderr, "In GetPow()\n");

    assert(dest_tree);
    assert(ip);

    Node **tokens  = dest_tree->node_ptrs;
    Node *res_node = GetOp(dest_tree, ip);

    Node *cur_op = tokens[*ip];

    if (cur_op->type != MATH_OP)
        return res_node;

    if (cur_op->val.math_op->num == DEG)
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

Node *GetOp(Tree *dest_tree, size_t *ip)              // f(..) ��� f(.. , ..)
{
    fprintf(stderr, "In GetOp()\n");

    assert(dest_tree);
    assert(ip);

    Node **tokens = dest_tree->node_ptrs;

    if (tokens[*ip]->type != MATH_OP)
        return GetSumInBrackets(dest_tree, ip);


    Node *op_node = tokens[(*ip)++];

    if (op_node->val.math_op->form == INFIX)
        SYNTAX_ERROR(dest_tree, op_node, "prefix form of operand");

    if (op_node->val.math_op->type == UNARY)
    {
        Node *arg = GetSumInBrackets(dest_tree, ip);
        op_node->left  = arg;
        op_node->right = NULL;

        return op_node;
    }

    else
    {
        bool is_open_expr_bracket = tokens[*ip]->type == MANAGER && tokens[*ip]->val.manager->name == OPEN_EXPR_BRACKET;
        if (!is_open_expr_bracket)
            SYNTAX_ERROR(dest_tree, tokens[*ip], Managers[OPEN_EXPR_BRACKET].my_symbol);
        RemoveNode(dest_tree, &tokens[*ip]);
        (*ip)++;

        Node *arg_1 = GetSum(dest_tree, ip);

        bool is_comma = tokens[*ip]->type == KEY_WORD && tokens[*ip]->val.key_word->name == COMMA;
        if (!is_comma)
            SYNTAX_ERROR(dest_tree, tokens[*ip], Managers[COMMA].my_symbol);
        RemoveNode(dest_tree, &tokens[*ip]);
        (*ip)++;

        Node *arg_2 = GetSum(dest_tree, ip);

        bool is_close_expr_bracket = tokens[*ip]->type == MANAGER && tokens[*ip]->val.manager->name == CLOSE_EXPR_BRACKET;
        if (!is_close_expr_bracket)
            SYNTAX_ERROR(dest_tree, tokens[*ip], Managers[CLOSE_EXPR_BRACKET].my_symbol);
        RemoveNode(dest_tree, &tokens[*ip]);
        (*ip)++;

        op_node->left  = arg_1;
        op_node->right = arg_2;

        return op_node;       
    }
}

Node *GetSumInBrackets(Tree *dest_tree, size_t *ip)
{
    fprintf(stderr, "In GetSumInBrackets()\n");

    assert(dest_tree);
    assert(ip);

    Node **tokens = dest_tree->node_ptrs;

    if (tokens[*ip]->type == MANAGER && tokens[*ip]->val.manager->name == OPEN_EXPR_BRACKET)
    {
        RemoveNode(dest_tree, &tokens[*ip]);
        (*ip)++;

        Node *res_node = GetSum(dest_tree, ip);

        bool is_close_expr_bracket = tokens[*ip]->type == MANAGER && tokens[*ip]->val.manager->name == CLOSE_EXPR_BRACKET;

        if (!is_close_expr_bracket)
            SYNTAX_ERROR(dest_tree, tokens[*ip], Managers[CLOSE_EXPR_BRACKET].my_symbol);

        RemoveNode(dest_tree, &tokens[*ip]);
        (*ip)++;
        
        return res_node;
    }
    
    else
        return GetVarOrFunc(dest_tree, ip);
}

Node *GetVarOrFunc(Tree *dest_tree, size_t *ip)
{
    assert(dest_tree);
    assert(ip);

    Node **tokens = dest_tree->node_ptrs;

    fprintf(stderr, "In getVarOrFunc()\n");

    if (tokens[*ip]->type == VAR_OR_FUNC)
    {
        fprintf(stderr, "1\n");

        Node *cur_node = tokens[(*ip)++];
        cur_node->type = VAR;
        Node *arg = NULL;

        if (tokens[*ip]->type == MANAGER && tokens[*ip]->val.manager->name == OPEN_EXPR_BRACKET)     // ����� ��� ����� �������
        {
            fprintf(stderr, "3\n");

            RemoveNode(dest_tree, &tokens[(*ip)++]);

            cur_node->type = FUNC;
            arg = GetExprSequence(dest_tree, ip);
            fprintf(stderr, "3.1\n");

            if (tokens[*ip]->type != MANAGER || tokens[*ip]->val.manager->name != CLOSE_EXPR_BRACKET)
                SYNTAX_ERROR(dest_tree, tokens[*ip], Managers[CLOSE_EXPR_BRACKET].my_symbol);

            RemoveNode(dest_tree, &tokens[(*ip)++]);

            Node *new_func = NewNode(dest_tree, KEY_WORD, {.key_word = &KeyWords[FUNC_T_INDICATOR]}, cur_node, arg);

            return NewNode(dest_tree, KEY_WORD, {.key_word = &KeyWords[FUNC_CALL]}, new_func, NULL);
        }

        else
        {
            fprintf(stderr, "4\n");
            return NewNode(dest_tree, KEY_WORD, {.key_word = &KeyWords[VAR_T_INDICATOR]}, cur_node, arg);
        }
    }

    else
    {
        fprintf(stderr, "2\n");
        return GetScanf(dest_tree, ip);
    }
}

Node *GetScanf(Tree *dest_tree, size_t *ip)
{
    fprintf(stderr, "GetScanf()\n");

    assert(dest_tree);
    assert(ip);

    Node **tokens = dest_tree->node_ptrs;

    if (tokens[*ip]->type == KEY_WORD && tokens[*ip]->val.key_word->name == SPU_IN)
        return tokens[(*ip)++];

    else 
        return GetPrintf(dest_tree, ip);
}

Node *GetPrintf(Tree *dest_tree, size_t *ip)
{
    fprintf(stderr, "GetPrintf()\n");


    assert(dest_tree);
    assert(ip);

    Node **tokens = dest_tree->node_ptrs;

    if (tokens[*ip]->type == KEY_WORD && tokens[*ip]->val.key_word->name == SPU_OUT)
    {
        Node *spu_out_node = tokens[(*ip)++];

        spu_out_node->left = GetSum(dest_tree, ip);

        return spu_out_node;
    }

    else
        return GetNumber(dest_tree, ip);
}

Node *GetNumber(Tree *dest_tree, size_t *ip)
{
    assert(dest_tree);
    assert(ip);

    Node **tokens = dest_tree->node_ptrs;

    if (tokens[*ip]->type == NUM)
    {
        return tokens[(*ip)++];
    }

    else
    {
        SYNTAX_ERROR(dest_tree, tokens[*ip], "expected type of NUM or VAR");
        return NULL;
    }
}