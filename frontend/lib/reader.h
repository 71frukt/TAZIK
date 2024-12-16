#ifndef READER_H
#define READER_H

#include "../../tree/tree_lib.h"

#define BASE_INPUT_FILE_NAME   "source_file.txt"

const size_t TOKEN_STR_LEN = 50;
FILE *GetInputFile  (const int argc, const char *argv[]);
void  BuildTree     (Tree *tree, FILE *source);
void  MakeTokens    (Tree *tree, FILE *source);
Node *GetNamedToken (Tree *tree, char *token_name);

Node *GetCode           (Tree *dest_tree);
Node *GetFuncInit       (Tree *dest_tree, size_t *ip);
Node *GetBlock          (Tree *dest_tree, size_t *ip);
Node *GetIf             (Tree *dest_tree, size_t *ip);
Node *GetWhile          (Tree *dest_tree, size_t *ip);
Node *GetBool           (Tree *dest_tree, size_t *ip);
Node *GetExpr           (Tree *dest_tree, size_t *ip);
Node *GetVarInit        (Tree *dest_tree, size_t *ip);
Node *GetSum            (Tree *dest_tree, size_t *ip);
Node *GetMul            (Tree *dest_tree, size_t *ip);
Node *GetPow            (Tree *dest_tree, size_t *ip);
Node *GetOp             (Tree *dest_tree, size_t *ip);
Node *GetSumInBrackets  (Tree *dest_tree, size_t *ip);
Node *GetVarOrFunc      (Tree *dest_tree, size_t *ip);
Node *GetNumber         (Tree *dest_tree, size_t *ip);


bool IsEngLetter (char ch);

void SyntaxError(Tree *tree, Node *cur_node, const char *expected_token, const char *file, int line, const char *func);

#define SYNTAX_ERROR(tree, cur_token, expected_token)  SyntaxError(tree, cur_token, expected_token, __FILE__, __LINE__, __func__)

#endif