#ifndef TREE_DEBUG_H
#define TREE_DEBUG_H

#include "tree.h"

#ifdef TREE_DEBUG
#define ON_TREE_DEBUG(...)  __VA_ARGS__
#define TREE_DUMP(tree)  TreeDump(tree, __FILE__, __LINE__, __func__)

#else
#define ON_TREE_DEBUG(...)
#define TREE_DUMP(tree)
#endif

#define LOGS_FOLDER      "logs/"
#define GRAPH_FOLDER     "graphs/"

#define LOGFILE_NAME     "tree_logfile.html"
const int PATH_NAME_LEN   = 100;

FILE *OpenLogFile  ();
void  CloseLogFile ();
char *GetFilePath  (const char *name, const char *folder, char *path);
void  TreeDump     (Tree *tree, const char *file, int line, const char *func);

#endif