// #include <stdio.h>
// 
// #include "tree.h"
// #include "operations.h"
// #include "debug.h"
#include "front_reader.h"
#include "front_writer.h"

#include "../../tree/tree_lib.h"

int main(const int argc, const char *argv[])
{
    fprintf(stderr, "START\n");

    FILE *input_file = GetInputFile(argc, argv);

    Tree code = {};
    TreeCtor(&code, START_TREE_SIZE ON_TREE_DEBUG(, "expr_tree"));

    BuildTreeByCode(&code, input_file);

    fclose(input_file);

    FILE *output_file = GetOutputFile(argc, argv);
    WriteTreeData(code.root_ptr, output_file);
    fclose(output_file);

    TreeDtor(&code);

    fprintf(stderr, "END");
    return 0;
}