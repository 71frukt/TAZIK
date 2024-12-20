#include <stdio.h>

#include "back_writer.h"
#include "../../tree/tree_lib.h"
#include "../../tree_saver/tree_saver.h"

int main(const int argc, const char *argv[])
{
    fprintf(stderr, "BACKEND_START\n");

    FILE *source_file = GetInputSaveFile(argc, argv);

    Tree source_tree = {};
    TreeCtor(&source_tree, START_TREE_SIZE ON_TREE_DEBUG(, "source"));

    BuildTreeByFileData(source_file, &source_tree);

    TREE_DUMP(&source_tree);

    FILE *asm_file = GetOutputAsmFile(argc, argv);

    fprintf(asm_file,   "PUSH 0 \n"
                        "POP AX     \n"

                        "PUSH 0 \n"
                        "POP BX \n"

                        "PUSH AX    \n"
                        "PUSH 0 \n"

                        "CALL main: \n"
                        "SPU_OUT    \n"
                        "HLT    \n"
            );

    PrintAsmCodeByNode(source_tree.root_ptr, asm_file);

    fprintf(stderr, "BACKEND_END\n");

    return 0;
}