#ifndef BACK_WRITER_H
#define BACK_WRITER_H

#include <stdio.h>

#define BASE_OUTPUT_FILE_NAME  "../../executable.asm"

FILE *GetOutputFile(const int argc, const char *argv[]);

#endif