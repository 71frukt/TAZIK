#include <stdio.h>

#include "tree.h"
#include "operations.h"
#include "debug.h"
#include "reader.h"

int main(const int argc, const char *argv[])
{
    fprintf(stderr, "START\n");

    FILE *input_file = GetInputFile(argc, argv);

    Tree code = {};
    TreeCtor(&code, START_TREE_SIZE ON_LANG_DEBUG(, "expr_tree"));

    BuildTree(&code, input_file);

    TreeDtor(&code);

    fprintf(stderr, "END");
    return 0;
}