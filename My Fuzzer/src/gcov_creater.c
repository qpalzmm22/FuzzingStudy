#include "../include/gcov_creater.h"

#define DEBUG

void
exec_gcc_coverage(char *filepath, char* binpath)
{
    switch(fork()){
        case -1 :
            perror("Error in fork");
            exit(1);
        case 0 : // child

            execl("/usr/bin/gcc", "/usr/bin/gcc", "-o", binpath, "--coverage", filepath, NULL);

            perror("Error in execl");
            exit(1);

        default : // parent
            wait(0);       
    }
    
}

// Takes only one argument
void 
exec_bin(char **argv, char * prog_path)
{
    char *p_bin_path;

    // a.out not good ???
    if((p_bin_path = realpath(prog_path, 0x0)) == 0x0){
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
exec_gcov(char * filepath, char * gcpath)
{
    switch(fork()){
        case -1 :
            perror("Error in fork");
            exit(1);
        case 0 : // child

            execl("/usr/bin/gcov", "/usr/bin/gcov", "-o", gcpath, filepath, NULL);
            perror("Error in execl");
            exit(1);
            
        default : // parent
            wait(0);
    }
}

void
exec_gcov_with_bc_option(char * filepath, char * gcpath)
{
    switch(fork()){
        case -1 :
            perror("Error in fork");
            exit(1);
        case 0 : // child

            execl("/usr/bin/gcov", "/usr/bin/gcov", "-o", gcpath, "-b", "-c", filepath, NULL);
            perror("Error in execl");
            exit(1);
            
        default : // parent
            wait(0);
    }
}



void
gcov_creater(char * filename, char * gcpath, int argc, char **args)
{
    char * filepath;
    if((filepath = realpath(filename, 0x0)) == 0x0){
        perror("Error in real_path");
        exit(1);
    }
    printf("%s\n", filepath);

    char ** argv = (char**) malloc(sizeof(char*) * (argc + 1));
    argv[0] = filepath;

    for(int i = 1 ; i < argc; i++){
        argv[i] = args[i - 1];
    }
    argv[argc] = 0x0;
    char * bin_path = get_bin_path(filepath);

    exec_gcc_coverage(filepath, bin_path);

    exec_bin(argv, bin_path);
    free(bin_path);
    free(argv);

    exec_gcov(filepath, gcpath);
    free(filepath);
}

char *
get_bin_path(char * filepath)
{
    char *bin_path = 0x0;
    int len = strlen(filepath);

    for(int i = len - 1 ; i >= 0 ; i--){
        if(filepath[i] == '.'){
            bin_path = (char*) calloc(i + 1, sizeof(char));

            assert(bin_path);
            strncpy(bin_path, filepath, i );
            bin_path[i - 1] = 0x0;  // null padding
            return bin_path; 
        }
    }
    return 0x0;
}


void
gcov_branch_creater(char * filename, char * gcpath,int argc, char **args)
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

    char * bin_path = get_bin_path(filepath);
    exec_gcc_coverage(filepath, bin_path);

    exec_bin(argv, bin_path);
    free(bin_path);
    free(argv);

    exec_gcov_with_bc_option(filepath, gcpath);
    free(filepath);
}



#ifdef DEBUG
int
main()
{
    char filename[] = "target/cgi_decode_ex.c";

    char *args[] = {"Send+mail+to+me+fuzzingbook.org"};
    char *gcpath = "./";
    int argc = 2;

    gcov_creater(filename, gcpath, argc, args);

    return 0;
}
#endif /* DEBUG */