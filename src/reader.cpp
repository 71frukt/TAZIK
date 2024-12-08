#include <stdio.h>
#include <assert.h>
#include <iostream>

#include "tree.h"
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
}

void MakeTokens(Tree *tree, FILE *source)
{
    setlocale(LC_ALL, "Russian");

    size_t cur_line   = 0;
    size_t cur_column = 0;

    char cur_token[TOKEN_STR_LEN] = {};

    while (true)
    {
        char start_ch = (char) getc(source);

        if (start_ch == EOF)
            break;
        
        else if (start_ch == '\n')
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

        if (IsEngLetter(start_ch))                                          // это имя функции или переменной
        {
            Node *new_node = NewNode(tree, VAR, {}, NULL, NULL);            // TODO: добавить функции, не только переменные

            ungetc(start_ch, source);
            int shift = 0;

            fscanf(source, "%s%n", cur_token, &shift);
            cur_column += shift;

            ProperName *cur_name_ptr = FindNameInTable(&tree->names_table, cur_token);

            if (cur_name_ptr == NULL)
                cur_name_ptr = NewNameInTable(&tree->names_table, cur_token);

            new_node->val.var = cur_name_ptr->name;
        }

        else                                                                // имя операнда или рандомный вкид
        {
            ungetc(start_ch, source);
            int shift = 0;

            fscanf(source, "%s%n", cur_token, &shift);
            cur_column += shift;

            const Operation  *cur_op    = GetOperationBySymbol  (cur_token);
            const ManageElem *manage_el = GetManageElemBySymbol (cur_token);

            assert(!(cur_op != NULL && manage_el != NULL));

            if (cur_op != NULL)
            {
                NewNode(tree, OP, {.op = cur_op}, NULL, NULL);
                continue;
            }

            else if (manage_el != NULL)
            {
                NewNode(tree, OP, {.manager = manage_el}, NULL, NULL);
                continue;
            }
        }
    }
}

bool IsRusLetter(char ch)
{
    return ((ch >= 'А' && ch <= 'Я') || (ch >= 'а' && ch <= 'я'));
}

bool IsEngLetter(char ch)
{
    return ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z'));
}
