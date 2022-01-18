#include <xc.h>

char* xc_file_read(const char* filename)
{
    FILE* file = fopen(filename, "rb");
    if (!file) {
        printf("File '%s' could not be opened\n", filename);
        return NULL;
    }
    fseek(file, 0, SEEK_END);
    size_t len = ftell(file);
    char* str = malloc(len + 1);
    fseek(file, 0, SEEK_SET);
    fread(str, len, 1, file);
    str[len] = '\0';
    fclose(file);
    return str;
}