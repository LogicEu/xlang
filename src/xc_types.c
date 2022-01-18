#include <xc.h>
#include <string.h>

xc_types_enum xc_token_to_type(const char* token)
{
    if (!strcmp(token, "i8")) return I8;
    if (!strcmp(token, "i16")) return I16;
    if (!strcmp(token, "i32")) return I32;
    if (!strcmp(token, "i64")) return I64;
    else return 0;
}

int xc_token_is_type(const char* token)
{
    return (xc_token_to_type(token));
}