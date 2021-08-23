#include "../include/gcov_creater.h"

// #define DEBUG

void
exec_gcc_coverage(char *filepath)
{
    switch(fork()){
        case -1 :
            perror("Error in fork");
            exit(1);
        case 0 : // child

            execl("/usr/bin/gcc", "/usr/bin/gcc", "--coverage", filepath, NULL);

            perror("Error in execl");
            exit(1);

        default : // parent
            wait(0);       
    }
    
}

// Takes only one argument
void 
exec_bin(char **argv)
{
    char *p_bin_path;

    // a.out not good ???
    if((p_bin_path = realpath("a.out", 0x0)) == 0x0){
        perror("Error in real_path");
        exit(1);
    }
    switch(fork()){
        case -1 :
            perror("Error in fork");
            exit(1);

        case 0 : // child

            execv(p_bin_path, argv);
            perror("Error in execl");
            exit(1);
            
        default : // parent
            wait(0);
    }
    free(p_bin_path);
}

void
exec_gcov(char * filepath)
{
    switch(fork()){
        case -1 :
            perror("Error in fork");
            exit(1);
        case 0 : // child

            execl("/usr/bin/gcov", "/usr/bin/gcov",  filepath, NULL);
            perror("Error in execl");
            exit(1);
            
        default : // parent
            wait(0);
    }
}

void
exec_gcov_with_bc_option(char * filepath)
{
    switch(fork()){
        case -1 :
            perror("Error in fork");
            exit(1);
        case 0 : // child

            execl("/usr/bin/gcov", "/usr/bin/gcov", "-b", "-c", filepath, NULL);
            perror("Error in execl");
            exit(1);
            
        default : // parent
            wait(0);
    }
}



void
gcov_creater(char * filename, int argc, char **args)
{
    char * filepath;
    if((filepath = realpath(filename, 0x0)) == 0x0){
        perror("Error in real_path");
        exit(1);
    }

    char ** argv = (char**) malloc(sizeof(char*) * (argc + 1));
    argv[0] = filepath;

    for(int i = 1 ; i < argc; i++){
        argv[i] = args[i - 1];
    }
    argv[argc] = 0x0;

    exec_gcc_coverage(filepath);

    exec_bin(argv);
    free(argv);

    exec_gcov(filepath);
    free(filepath);
}


void
gcov_branch_creater(char * filename, int argc, char **args)
{
    char * filepath;
    if((filepath = realpath(filename, 0x0)) == 0x0){
        perror("Error in real_path");
        exit(1);
    }

    char ** argv = (char**) malloc(sizeof(char*) * (argc + 1));
    argv[0] = filepath;

    for(int i = 1 ; i < argc; i++){
        argv[i] = args[i - 1];
    }
    argv[argc] = 0x0;

    exec_gcc_coverage(filepath);

    exec_bin(argv);
    free(argv);

    exec_gcov_with_bc_option(filepath);
    free(filepath);
}



#ifdef DEBUG
int
main()
{
    char filename[] = "cgi_decode_ex.c";

    char *args[] = {"Send+mail+to+me+fuzzingbook.org"};
    int argc = 2;

    gcov_creater(filename, argc, args);

    return 0;
}
#endif /* DEBUG */