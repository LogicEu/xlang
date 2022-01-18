#include <xc.h>

int xc_error(const char* str)
{
    printf("%s\n", str);
    return EXIT_FAILURE;
}

int xc_error_line(const char* str, const size_t line)
{
    printf("Line %ld: %s\n", line + 1, str);
    return EXIT_FAILURE;
}

void xc_abort(const char* str)
{
    printf("%s\n", str);
    exit(EXIT_FAILURE);
}

void xc_abort_line(const char* str, const size_t line)
{
    printf("Line %ld: %s\n", line + 1, str);
    exit(EXIT_FAILURE);
}