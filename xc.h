#ifndef THE_X_COMPILER
#define THE_X_COMPILER

#ifdef __cplusplus
extern "C" {
#endif

/******************
 * THE X COMPILER *
 ******************/

#include <utopia/utopia.h>
#include <xstring/xstring.h>
#include <stdlib.h>
#include <stdio.h>

typedef enum xc_types_enum {
    VOID = 0,
    I8 = 1,
    I16 = 2,
    I32 = 4,
    I64 = 8
} xc_types_enum;

typedef struct xc_var_t {
    xc_types_enum type;
    char* name;
} xc_var_t;

typedef enum xc_def_type_t {
    FUNC_DEF,
    VAR_DEF,
    VAR_ASSIGNATION,
    RETURN
} xc_def_type_t;

typedef struct xc_def_stat_t {
    xc_def_type_t deftype;
    char** tokens;
    size_t tokencount;
} xc_def_stat_t;

typedef struct xc_def_func_t {
    xc_def_stat_t* statements;
    size_t statcount;
} xc_def_func_t;

typedef struct xc_func_t {
    char* name;
    xc_var_t ret, *vars, *args;
    int argcount, varcount;
} xc_func_t;

xc_types_enum xc_token_to_type(const char* token);
int xc_token_is_type(const char* token);

xc_var_t xc_var_new(const char* name, const char* typestr);
xc_var_t xc_var_val(const char* name, const char* typestr, void* val);
void xc_var_free(xc_var_t* var);

void xc_func_open(const char* typestr, const char* funcname);
void xc_func_close(const char* retvar);
void xc_func_arg_push(const char* name, const char* typestr);
void xc_func_var_push(const char* name, const char* typestr, void* val);
void xc_func_stack_init();
int xc_func_is_open();
xc_var_t* xc_func_search_var(xc_func_t* func, const char* name);
xc_func_t* xc_search_func(const char* name);
xc_func_t* xc_func_current();
int xc_token_is_func(const char* name);

char* xc_file_read(const char* filename);

void xc_file_write_open(FILE* file);
void xc_file_write_close(FILE* file);

int xc_error(const char* str);
int xc_error_line(const char* str, const size_t line);
void xc_abort(const char* line);
void xc_abort_line(const char* str, const size_t line);

int xc_stack_main();

void xc_eval(char* buffer);

int xc_compile(const char* filein, const char* fileout);

#ifdef __cplusplus
}
#endif
#endif