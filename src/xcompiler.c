#include <xc.h>
#include <string.h>
#include <stdarg.h>

#define FILE_STR_SIZE 1024

static int flag_S = 0;
static int main_stack_len = 0;

int xc_stack_main()
{
    return main_stack_len;
}

static void xc_init()
{
    xc_func_stack_init();
}

static char* xc_input_raw(const char* filein)
{
    char* filebuffer = xc_file_read(filein);
    if (!filebuffer) {
        xc_abort("Aborting");
    }
    return filebuffer;
}

static char* xc_preprocess(char* rawbuffer)
{
    return xc_string_trim(rawbuffer);
}

int xc_compile(const char* filein, const char* fileout)
{
    xc_init();
    char* rawbuffer = xc_input_raw(filein);
    char* buffer = xc_preprocess(rawbuffer);
    xc_eval(buffer);

    xc_func_t* main_func = xc_search_func("main");
    if (main_func == NULL) {
        return xc_error("Missing main function");
    } else {
        int stack = 0;
        for (int i = 0; i < main_func->varcount; i++) {
            stack += main_func->vars[i].type;
        }
        stack += main_func->ret.type;

        int div = stack / 16;
        int rem = (stack % 16 != 0);
        main_stack_len = (div + rem) * 16;
    }

    // Output
    /*char fileasm[strlen(fileout) + 3];
    strcpy(fileasm, fileout);
    strcat(fileasm, ".s");

    FILE* file = fopen(fileasm, "wb");
    if (!file) {
        printf("Could not write file '%s'\n", fileasm);
        return EXIT_FAILURE;
    }
    
    xc_file_write_open(file);

    for (size_t i = 0; i < linecount; i++) {
        if (strstr(lines[i], "func")) {
            char funcname[FILE_STR_SIZE], ret_type[32];
            sscanf(lines[i], "func %s %s\n", ret_type, funcname);
            xc_file_write_func_open(file, funcname);
        }
    }
    fprintf(file, "\tmovl\t$%d, %%eax\n", x_return);
    fprintf(file, "\tmovl\t$0, -4(%%rbp)\n");
    xc_file_write_func_close(file);
    xc_file_write_close(file);
    fclose(file);

    char gcc_compile[FILE_STR_SIZE];
    sprintf(gcc_compile, "gcc %s -o %s", fileasm, fileout);
    system(gcc_compile);

    if (!flag_S) {
        char rm_clean[FILE_STR_SIZE];
        sprintf(rm_clean, "rm %s", fileasm);
        system(rm_clean);
    }

    xc_strings_free(lines, linecount);*/

    return EXIT_SUCCESS;
}

int main(int argc, char** argv)
{
    if (argc < 2) {
        return xc_error("Missing input file");
    }

    char input_file[FILE_STR_SIZE] = "?", output_file[FILE_STR_SIZE] = "a.out";

    for (int i = 1; i < argc; ++i) {
        if (!strcmp(argv[i], "-version")) {
            printf("xc\tthe X compiler\t\tv0.1.0\n");
            return EXIT_SUCCESS;
        }
        else if (!strcmp(argv[i], "-o")) {
            if (argc < i + 1) {
                return xc_error("Missing output file");
            }
            strcpy(output_file, argv[++i]);
        }
        else if (!strcmp(argv[i], "-S")) {
            ++flag_S;
        }
        else strcpy(input_file, argv[i]);
    }

    if (!strcmp(input_file, "?")) {
        return xc_error("Missing input file\n");
    }

    return xc_compile(input_file, output_file);
}
