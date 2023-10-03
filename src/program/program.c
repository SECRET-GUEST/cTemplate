#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include "program/program.h"
#include "handlers/error_handler.h"
#include "translate/translator.h"



// EXAMPLE FUNCTION COLLECTING EXTENSION WITH DYNAMIC MEMORY ALLOCATION
int initialize_sorter(const char *inputDirectory, const char *outputDirectory) {


    // Try creating the output directory; return error if unsuccessful (except if directory already exists)
    if (mkdir(outputDirectory, 0777) == -1 && errno != EEXIST) {
        display_error_message(translate("program.txt10")); // Unable to create output directory
        return -1;
    }

    char **extensions = NULL;
    int count = 0;

    // Collect extensions
    if (Function1(inputDirectory, &extensions, &count) == -1) return -1;

    // Free dynamically allocated memory for extensions
    for (int i = 0; i < count; i++) 
        free(extensions[i]);
    free(extensions);
    
    return 0;
}

