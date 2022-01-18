#include <xc.h>
#include <string.h>
#include <ctype.h>

static size_t xc_string_separator_count(char* str, const char k)
{
    size_t count = 1;

    while (1) {
        char c = *str;
        if (c == '\0') break;
        count += (c == k);
        str++;
    }

    return count;
}

char* xc_string_trim(char* str)
{
    char* ch = str;
    char* buff = malloc(strlen(ch) + 1);

    int mark = 0;
    while (1) {
        char c = *ch;
        ch++;

        if (c == ' ') {
            while (*ch == ' ') {
                ch++;
            }
            if (*ch == '\n') continue;
        }

        if (c == '\n') {
            if (!mark) continue;
            while (*ch == '\n' || *ch == ' ') {
                ch++;
            }
            if (*ch == '\0') {
                buff[mark++] = *ch;
                break;
            }
        }
        buff[mark++] = c;
        if (c == '\0') break;
    }

    return buff;
}

char** xc_string_separate(char* str, size_t* size, const char k)
{
    const size_t count = xc_string_separator_count(str, k);
    *size = count;

    char** strs = malloc(count * sizeof(char*));
    char* ch = str;
    
    for (size_t i = 0; i < count; i++) {
        
        char* bch = ch;

        int acc = 0;
        while (1) {
            char c = *ch;
            ch++;
            if (c == k || c == '\0') break;
            acc++;
        }

        strs[i] = malloc(acc + 1);
        memcpy(strs[i], bch, acc);
        strs[i][acc] = '\0';
    }

    return strs;
}

int xc_string_is_int(const char* str)
{
    size_t size = strlen(str);
    for (size_t i = 0; i < size; i++) {
        if (!isdigit(str[i])) return 0;
    }
    return 1;
}

void xc_strings_free(char** strs, const size_t count)
{
    for (size_t i = 0; i < count; i++) {
        free(strs[i]);
    }
    free(strs);
}