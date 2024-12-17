#include <stdio.h>

#include "back_reader.h"
#include "../../tree/tree_lib.h"

int main(const int argc, const char *argv[])
{
    fprintf(stderr, "BACKEND_START\n");

    FILE *source_file = GetInputFile(argc, argv);

    Tree source_tree = {};
    TreeCtor(&source_tree, START_TREE_SIZE ON_TREE_DEBUG(, "source"));

    source_tree.root_ptr = BuildTreeByFileData(source_file, &source_tree);

    TREE_DUMP(&source_tree);

    fprintf(stderr, "BACKEND_END\n");

    return 0;
}