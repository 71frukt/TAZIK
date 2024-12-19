#include <stdio.h>
#include <math.h>
#include <string.h>
#include <assert.h>

#include "operations.h"
#include "tree_debug.h"
#include "asm_operations.h"

extern FILE *LogFile;

const MathOperation *GetOperationBySymbol(char *sym, SymbolMode mode) // TODO: move asm from here
{
    assert(sym);

    for (size_t i = 0; i < OPERATIONS_NUM; i++)
    {
        const char *compare_sym = (mode == MY_CODE_MODE ? MathOperations[i].my_symbol : MathOperations[i].real_symbol);

        if (strcmp(sym, compare_sym) == 0)
            return &MathOperations[i];
    }

    return NULL;
}

const KeyWord *GetKeyWordBySymbol(char *sym, SymbolMode mode)
{
    assert(sym);

    for (size_t i = 0; i < KEY_WORDS_NUM; i++)
    {
        const char *compare_sym = (mode == MY_CODE_MODE ? KeyWords[i].my_symbol : KeyWords[i].real_symbol);

        if (compare_sym == NULL)
            continue;

        if (strcmp(sym, compare_sym) == 0)
            return &KeyWords[i];
    }

    return NULL;
}

const ManageElem *GetManageElemBySymbol(char *sym, SymbolMode mode)
{
    assert(sym);

    for (size_t i = 0; i < MANAGE_ELEMS_NUM; i++)
    {
        const char *compare_sym = (mode == MY_CODE_MODE ? Managers[i].my_symbol : Managers[i].real_symbol);

        if (strcmp(sym, compare_sym) == 0)
            return &Managers[i];
    }

    return NULL;
}

void PrintMathOpAsm(Node *math_op, FILE *dest_file)
{
    assert(math_op);
    assert(math_op->type == MATH_OP);
    assert(dest_file);

    PrintAsmCodeByNode(math_op->left,  dest_file);

    if (math_op->val.math_op->type == BINARY)
        PrintAsmCodeByNode(math_op->right, dest_file);

    fprintf(dest_file, "%s ", math_op->val.math_op->asm_symbol);

    if (!IsBool(math_op))
        fprintf(dest_file, "\n");
}

void PrintVarTAsm(Node *var_t_node, FILE *dest_file)
{
    assert(var_t_node);
    assert(var_t_node->type == KEY_WORD && var_t_node->val.key_word->name == VAR_T_INDICATOR);

    fprintf(dest_file, "%s [AX + %lld]\n", AsmOperations[PUSH_ASM].sym, var_t_node->left->val.prop_name->number);
}

void PrintChildrenAsm(Node *new_expr_node, FILE *dest_file)
{
    assert(new_expr_node);
    assert(dest_file);

    fprintf(dest_file, "\n");

    if (new_expr_node->left != NULL)
        PrintAsmCodeByNode(new_expr_node->left, dest_file);

    if (new_expr_node->right != NULL)
        PrintAsmCodeByNode(new_expr_node->right, dest_file);
}

void PopToEmptyRam(FILE *asm_file)                          // pop [BX++]   // BX - размер, AX - начало фрейма
{
    fprintf(asm_file, "\n%s [BX] \n", AsmOperations[POP_ASM].sym);

    fprintf(asm_file, "%s BX   \n"                     // BX ++
                      "%s 1    \n"
                      "%s      \n"
                      "%s BX   \n", 
        AsmOperations[PUSH_ASM].sym, AsmOperations[PUSH_ASM].sym, AsmOperations[ADD_ASM].sym, AsmOperations[POP_ASM].sym);
}

void PrintInitAsm(Node *init_node, FILE *dest_file)
{
    assert(init_node);
    assert(dest_file);
    assert(init_node->type == KEY_WORD && (init_node->val.key_word->name == INT_INIT || init_node->val.key_word->name == DOUBLE_INIT));

    Node *var_node = init_node->left->left;

    if(init_node->left->type == KEY_WORD && init_node->left->val.key_word->name == VAR_T_INDICATOR)
    {
        PrintAsmCodeByNode(init_node->right, dest_file);
        // fprintf(dest_file, "%s [AX + %lld]\n", AsmOperations[POP_ASM].sym, var_node->val.prop_name->number);
        PopToEmptyRam(dest_file);
    }

    else 
    {
        fprintf(dest_file, "%s:\n", init_node->left->left->val.prop_name->name);        // AX - start of frame, BX - cur size

        PopToEmptyRam(dest_file);       // pop [BX++]

        PrintAsmCodeByNode(init_node->right, dest_file);                                // BX = AX и pop AX печатаются перед Return
    }
}

void PrintCallAsm(Node *call_node, FILE *dest_file)
{
    fprintf(stderr, "Start of PrintCallAsm()\n");

    fprintf(dest_file, "%s AX \n", AsmOperations[PUSH_ASM].sym);

    PrintAsmCodeByNode(call_node->left->right, dest_file);

    fprintf(dest_file, "%s %s: \n", AsmOperations[JMP_ASM].sym, call_node->left->left->val.prop_name->name);

    fprintf(stderr, "End of PrintCallAsm()\n");
}

void PrintAssignAsm(Node *assign_node, FILE *dest_file)
{
    assert(assign_node);
    assert(dest_file);
    assert(assign_node->left->type == KEY_WORD && assign_node->left->val.key_word->name == VAR_T_INDICATOR &&
           assign_node->left->left->type == VAR);

    PrintAsmCodeByNode(assign_node->right, dest_file);
    fprintf(dest_file, "%s [%lld]\n", AsmOperations[POP_ASM].sym, assign_node->left->val.prop_name->number);
}

void PrintIfAsm(Node *if_node, FILE *dest_file)
{
    assert(if_node);
    assert(dest_file);
    assert(if_node->left->type == MATH_OP && IsBool(if_node->left));

    static size_t if_marks_count = 0;

    size_t cur_mark_num = if_marks_count++;
    PrintMathOpAsm(if_node->left, dest_file);
    fprintf(dest_file, "if_mark_%lld:\n", cur_mark_num);

    PrintAsmCodeByNode(if_node->right, dest_file);

    fprintf(dest_file, "if_mark_%lld:\n", cur_mark_num);
}

void PrintWhileAsm(Node *while_node, FILE *dest_file)
{
    assert(while_node);
    assert(dest_file);
    assert(while_node->left->type == MATH_OP && IsBool(while_node->left));

    static size_t while_marks_count = 0;

    size_t cur_mark_num = while_marks_count++;
    fprintf(dest_file, "while_start_mark_%lld:\n", cur_mark_num);

    PrintMathOpAsm(while_node->left, dest_file);
    fprintf(dest_file, "while_end_mark_%lld:\n", cur_mark_num);

    PrintAsmCodeByNode(while_node->right, dest_file);

    fprintf(dest_file,  "%s while_start_mark_%lld:  \n"
                        "while_end_mark_%lld:       \n", AsmOperations[JMP_ASM].sym, cur_mark_num, cur_mark_num);
}

void PrintReturnAsm(Node *ret_node, FILE *dest_file)
{
    assert(ret_node);
    assert(dest_file);

    PrintAsmCodeByNode(ret_node->left, dest_file);

    fprintf(dest_file, "%s AX   \n"     // AX - start of frame, BX - cur size
                       "%s BX   \n"     // BX = AX
                       "%s AX   \n",    // pop AX
            AsmOperations[PUSH_ASM].sym, AsmOperations[POP_ASM].sym, AsmOperations[POP_ASM].sym);

    fprintf(dest_file, "%s\n", AsmOperations[RET_ASM].sym);
}

void PrintAsmCodeByNode(Node *node, FILE *dest_file)
{
    assert(node);
    assert(dest_file);

    if (node->type == NUM)
        fprintf(dest_file, "%s " TREE_ELEM_PRINT_SPECIFIER "\n", AsmOperations[PUSH_ASM].sym, node->val.num);

    if (node->type == MATH_OP)
        PrintMathOpAsm(node, dest_file);
    
    else if (node->type == KEY_WORD)
    {
        node->val.key_word->PrintAsmCodeFunc(node, dest_file);
    }

    else if (node->type == NEW_BLOCK)
        PrintAsmCodeByNode(node->left, dest_file);

    else
        PrintChildrenAsm(node, dest_file);
}
