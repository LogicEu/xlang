#include <xc.h>
#include <string.h>

char* arg_reg_table[6] = {
    "%edi",
    "%esi",
    "%edx",
    "%ecx",
    "%ebx",
    "%eax"
};

const char* asm_str_mac = 
    "\t.section\t__TEXT,__text,regular,pure_instructions\n"
    "\t.macosx_version_min 10, 13\n";

const char* asm_str_func_setup = 
    "\t.cfi_startproc\n"
    "\tpushq\t%rbp\n"
    "\t.cfi_def_cfa_offset 16\n"
    "\t.cfi_offset %rbp, -16\n"
    "\tmovq\t%rsp, %rbp\n"
    "\t.cfi_def_cfa_register %rbp\n";

const char* asm_str_func_end =
    "\tpopq\t%rbp\n"
    "\tretq\n"
    "\t.cfi_endproc\n\n";

const char* asm_str_table = ".subsections_via_symbols\n";

static void xc_file_write_func_open(FILE* file, const char* funcname)
{
    char name[strlen(funcname) + 2];
    strcpy(name, "_");
    strcat(name, funcname);
    fprintf(file, "\t.globl\t%s\n\t.p2align\t4, 0x90\n%s:\n%s", name, name, asm_str_func_setup);
}

static void xc_file_write_func_close(FILE* file)
{
    fprintf(file, "%s", asm_str_func_end);
}

void xc_file_write_func(FILE* file, xc_func_t* func)
{
    xc_file_write_func_open(file, func->name);

    int main = (!strcmp(func->name, "main"));
    int stack;
    if (main) {
        stack = xc_stack_main();
        fprintf(file, "\tsubq\t$%d, %%rsp\n", stack);
    } 
    
    int acc = 0;
    xc_var_t* var = func->args;
    for (int i = 0; i < func->argcount; i++) {
        acc += var->type;
        fprintf(file, "\tmovl\t%s, -%d(%%rbp)\n", arg_reg_table[i], acc);
        var++;
    }

    var = func->vars;
    for (int i = 0; i < func->varcount; i++) {
        acc += var->type;
        fprintf(file, "\tmovl\t$0, -%d(%%rbp)\n", acc);
        var++;
    } 

    var = &func->ret;
    acc += var->type;
    fprintf(file, "\tmovl\t-%d(rbp), %%eax\n", acc);

    if (main) {
        fprintf(file, "\taddq\t$%d, %%rsp\n", stack);
    } 

    xc_file_write_func_close(file);
}

void xc_file_write_open(FILE* file)
{
    fprintf(file, "%s", asm_str_mac);
}

void xc_file_write_close(FILE* file)
{
    fprintf(file, "%s", asm_str_table);
}