#include <xc.h>
#include <string.h>

static char** strlines;
static char** tokens;
static size_t tokencount, strlinecount;
static size_t current_line;

static void xc_keyword_func()
{
    // Rules
    if (xc_func_is_open()) {
        xc_abort_line("Cannot declare function within a function", current_line);
    }
    if (tokencount < 3) {
        xc_abort_line("Invalid function declaration. Missing type or name", current_line);
    }
    int argc = tokencount - 3;
    if (argc % 2 != 0) {
        xc_abort_line("Missing function input argument type or name", current_line);
    }
    if (!xc_token_is_type(tokens[1])) {
        xc_abort_line("Type for function input argument is invalid or missing", current_line);
    }

    //Ok
    xc_func_open(tokens[1], tokens[2]);
    for (int i = 0; i < argc; i += 2) {
        xc_func_arg_push(tokens[4 + i], tokens[3 + 1]);
    }
}

static void xc_keyword_return()
{
    //Rules
    if (!xc_func_is_open()) {
        xc_abort_line("Cannot return from outside of a function", current_line);
    }
    if (tokencount == 1) {
        xc_func_t* func = xc_func_current();
        if (func->ret.type != VOID) {
            xc_abort_line("Non void function must return a value", current_line);
        }
    } 

    //Ok
    xc_func_close(tokens[1]);
}

static void xc_declaration()
{
    //Rules
    if (tokencount < 2) {
        xc_abort_line("Missing variable name", current_line);
    }
    if (tokencount > 2) {
        xc_abort_line("Declarations cannot assign values yet", current_line);
    }
    if (!xc_func_is_open()) {
        xc_abort_line("Variables and declarations are not allowed outside functions", current_line);
    }

    //Ok
    int val = 0;
    xc_func_var_push(tokens[1], tokens[0], &val);
}

static int xc_eval_first()
{
    if (!strcmp(tokens[0], "func")) {
        xc_keyword_func();
        return 1;
    } 
    else if (!strcmp(tokens[0], "return")) {
        xc_keyword_return();
        return 1;
    }
    else if (xc_token_is_type(tokens[0])) {
        xc_declaration();
        return 1;
    }
    else if (xc_func_search_var(xc_func_current(), tokens[0])) {
        //xc_assignation();
        return 1;
    }
    else if (xc_search_func(tokens[0])) {
        //xc_call_function();
        return 1;
    }
    return 0;
}

static void xc_eval_line()
{
    tokens = xc_string_separate(strlines[current_line], &tokencount, ' ');
    
    xc_eval_first();
    
    xc_strings_free(tokens, tokencount);
}

void xc_eval(char* buffer)
{
    strlines = xc_string_separate(buffer, &strlinecount, '\n');
    for (size_t i = 0; i < strlinecount; i++) {
        current_line = i;
        xc_eval_line();
    }
    xc_strings_free(strlines, strlinecount);
}