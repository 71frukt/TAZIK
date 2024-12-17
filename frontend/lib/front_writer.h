#ifndef WRITER_H
#define WRITER_H

#define BASE_SAVE_TREE_FILE_NAME  "../../res_tree.txt"

FILE *GetOutputFile (const int argc, const char *argv[]);
void  WriteTreeData (Node *cur_node, FILE *dest_file);

#endif