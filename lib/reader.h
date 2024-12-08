#ifndef READER_H
#define READER_H

#define BASE_INPUT_FILE_NAME   "source_file.txt"

const size_t TOKEN_STR_LEN = 50;

FILE *GetInputFile  (const int argc, const char *argv[]);
void  BuildTree     (Tree *tree, FILE *source);
void  MakeTokens    (Tree *tree, FILE *source);

bool IsRusLetter (char ch);
bool IsEngLetter (char ch);
#endif