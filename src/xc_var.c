#include <xc.h>
#include <string.h>

xc_var_t xc_var_new(const char* name, const char* typestr)
{
    xc_var_t var;
    var.name = malloc(strlen(name) + 1);
    strcpy(var.name, name);
    var.type = xc_token_to_type(typestr);
    return var;
}

xc_var_t xc_var_val(const char* name, const char* typestr, void* val)
{
    xc_var_t var;
    var.name = malloc(strlen(name) + 1);
    strcpy(var.name, name);
    var.type = xc_token_to_type(typestr);
    return var;
}

void xc_var_free(xc_var_t* var)
{
    free(var->name);
}