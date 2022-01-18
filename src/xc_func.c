#include <xc.h>
#include <string.h>

static xc_func_t* current_func = NULL;
static xc_stack_t* functions = NULL;
static int func_is_open = 0;

static xc_func_t xc_func_new(const char* typestr, const char* funcname)
{
    xc_func_t func;
    func.name = malloc(strlen(funcname) + 1);
    strcpy(func.name, funcname);
    func.ret = xc_var_new("ret", typestr);
    func.varcount = func.argcount = 0;
    func.vars = func.args = NULL;
    return func;
}

xc_func_t* xc_search_func(const char* name)
{
    xc_func_t* func = functions->data;
    for (unsigned int i = 0; i < functions->used; i++) {
        if (!strcmp(func->name, name)) return func;
        func++;
    }
    return NULL;
}

xc_func_t* xc_func_current()
{
    return current_func;
}

int xc_token_is_func(const char* name)
{
    return (xc_search_func(name) != NULL);
}

void xc_func_open(const char* typestr, const char* funcname)
{
    func_is_open++;
    *current_func = xc_func_new(typestr, funcname);
}

void xc_func_arg_push(const char* name, const char* typestr)
{
    current_func->argcount++;
    if (current_func->args) {
        current_func->args = realloc(current_func->args, current_func->argcount * sizeof(xc_var_t));
    } else current_func->args = malloc(current_func->argcount * sizeof(xc_var_t));
    current_func->args[current_func->argcount - 1] = xc_var_new(name, typestr);
}

void xc_func_var_push(const char* name, const char* typestr, void* val)
{
    current_func->varcount++;
    if (current_func->vars) {
        current_func->vars = realloc(current_func->vars, current_func->varcount * sizeof(xc_var_t));
    } else current_func->vars = malloc(current_func->varcount * sizeof(xc_var_t));
    current_func->vars[current_func->varcount - 1] = xc_var_val(name, typestr, val);
}

xc_var_t* xc_func_search_var(xc_func_t* func, const char* name)
{
    for (int i = 0; i < func->argcount; i++) {
        if (!strcmp(name, func->args[i].name)) return &func->args[i];
    }
    for (int i = 0; i < func->varcount; i++) {
        if (!strcmp(name, func->vars[i].name)) return &func->vars[i];
    }
    return NULL;
}

void xc_func_close(const char* retvar)
{
    /*xc_var_t* varptr = xc_func_search_var(current_func, retvar);
    if (!varptr) {
        int n = atoi(retvar);
        memcpy(current_func->ret.val, &n, current_func->ret.type);
    } else memcpy(current_func->ret.val, varptr->val, current_func->ret.type);*/

    xc_stack_push(functions, current_func);
    func_is_open--;
}

void xc_func_stack_init()
{
    current_func = malloc(sizeof(xc_func_t));
    functions = xc_stack_new(16, sizeof(xc_func_t));
}

int xc_func_is_open()
{
    return func_is_open;
}