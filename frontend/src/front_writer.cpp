#include <stdio.h>

#include "../../tree/tree_lib.h"
#include "front_writer.h"

FILE *GetOutputFile(const int argc, const char *argv[])
{
    if (argc < 3)
        return fopen(BASE_SAVE_TREE_FILE_NAME, "w");
    
    else
        return fopen(argv[2], "w");
}

void WriteTreeData(Node *cur_node, FILE *dest_file)
{
    fprintf(dest_file, "%s ", NodeValToStr(cur_node));

    if (cur_node != NULL)
    {
        fprintf(dest_file, "( ");

        WriteTreeData(cur_node->left,  dest_file);

        fprintf(dest_file, "; ");

        WriteTreeData(cur_node->right, dest_file);

        fprintf(dest_file, ") ");
    }    
}