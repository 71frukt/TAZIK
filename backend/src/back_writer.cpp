#include <stdio.h>

#include "back_writer.h"

FILE *GetOutputFile(const int argc, const char *argv[])
{
    FILE *output_file = NULL;

    if (argc < 2)
        output_file = fopen(BASE_OUTPUT_FILE_NAME, "w");
    
    else
        output_file = fopen(argv[2], "w");

    return output_file;   
}