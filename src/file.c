#include "file.h"

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

int logl_file_read_text(const char *path, char **text)
{
    FILE *file;

    if (fopen_s(&file, path, "rb"))
        return 1;

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    *text = malloc(file_size + 1);                          // alloc size for file content + zero
    size_t objects_read = fread(*text, 1, file_size, file); // set file content
    (*text)[file_size] = 0;                                 // set zero

    assert(objects_read == file_size);

    fclose(file);

    return 0;
}
