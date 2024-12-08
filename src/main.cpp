#include <stdio.h>

#include "tree.h"
#include "operations.h"
#include "debug.h"
#include "reader.h"

int main(const int argc, const char *argv[])
{

    FILE *input_file = GetInputFile(argc, argv);

    Tree code = {};
    BuildTree(&code, input_file);

    return 0;
}