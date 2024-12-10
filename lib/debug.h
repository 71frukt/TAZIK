#ifndef LANG_DEBUG_H
#define LANG_DEBUG_H

#include "tree.h"

#ifdef LANG_DEBUG
#define ON_LANG_DEBUG(...)  __VA_ARGS__
#define LANG_DUMP(tree)  LangDump(tree, __FILE__, __LINE__, __func__)

#else
#define ON_LANG_DEBUG(...)
#define LANG_DUMP(tree)
#endif

#define LOGS_FOLDER      "logs/"
#define GRAPH_FOLDER     "graphs/"

#define LOGFILE_NAME     "tree_logfile.html"
const int PATH_NAME_LEN   = 100;

FILE *OpenLogFile  ();
void  CloseLogFile ();
char *GetFilePath  (const char *name, const char *folder, char *path);
void  LangDump     (Tree *tree, const char *file, int line, const char *func);

#endif