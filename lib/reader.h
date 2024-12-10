#ifndef READER_H
#define READER_H

#include "tree.h"

#define BASE_INPUT_FILE_NAME   "source_file.txt"

const size_t TOKEN_STR_LEN = 50;
FILE *GetInputFile  (const int argc, const char *argv[]);
void  BuildTree     (Tree *tree, FILE *source);
void  MakeTokens    (Tree *tree, FILE *source);
void  GetNamedToken (Tree *tree, char *token_name);

Node *GetBlock          (Tree *dest_tree);
Node *GetInit           (Tree *dest_tree, size_t *ip);
Node *GetSum            (Tree *dest_tree, size_t *ip);
Node *GetMul            (Tree *dest_tree, size_t *ip);
Node *GetPow            (Tree *dest_tree, size_t *ip);
Node *GetFunc           (Tree *dest_tree, size_t *ip);
Node *GetInitInBrackets (Tree *dest_tree, size_t *ip);
Node *GetVarOrFunc      (Tree *dest_tree, size_t *ip);
Node *GetNumber         (Tree *dest_tree, size_t *ip);


bool IsEngLetter (char ch);

void SyntaxError(Tree *tree, size_t ip, const char *cur_token, const char *expected_token, const char *file, int line, const char *func);

#define SYNTAX_ERROR(tree, ip, cur_token, expected_token)  SyntaxError(tree, ip, cur_token, expected_token, __FILE__, __LINE__, __func__)

#endif