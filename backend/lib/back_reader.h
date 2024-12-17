#ifndef BACK_READER_H
#define BACK_READER_H

#include <stdio.h>
#include "../../tree/tree_lib.h"

#ifndef BASE_SAVE_TREE_FILE_NAME
#define BASE_SAVE_TREE_FILE_NAME  "../../res_tree.txt"
#endif

FILE *GetInputFile        (const int argc, const char *argv[]);
Node *BuildTreeByFileData (FILE *source, Tree *tree);

#endif