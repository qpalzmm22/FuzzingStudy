#include "../include/gcov_creater.h"

// #define DEBUG

void
exec_gcc_coverage(char *file_path, char* bin_path)
{
    switch(fork()){
        case -1 :
            perror("Error in fork");
            exit(1);
        case 0 : // child


            execl("/usr/bin/gcc", "/usr/bin/gcc", "-o", bin_path, "--coverage", file_path, NULL);

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
        perror("Error in real_path in exec_bin");
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
exec_gcov(char * file_path, char * dir_path)
{
    switch(fork()){
        case -1 :
            perror("Error in fork");
            exit(1);
        case 0 : // child
            if(chdir(dir_path) != 0){
                perror("Error in chdir in exec_gcov_with_bc_option");
                exit(1);
            }



            execl("/usr/bin/gcov", "/usr/bin/gcov", "-o", dir_path, file_path, NULL);
            perror("Error in execl");
            exit(1);
            
        default : // parent
            wait(0);
    }
}

void
exec_gcov_with_bc_option(char * file_path, char * dir_path)
{
    switch(fork()){
        case -1 :
            perror("Error in fork");
            exit(1);
        case 0 : // child
            if(chdir(dir_path) != 0){
                perror("Error in chdir in exec_gcov_with_bc_option");
                exit(1);
            }

            close(STDOUT_FILENO);
            close(STDERR_FILENO);

            execl("/usr/bin/gcov", "/usr/bin/gcov", "-o", dir_path, "-b", "-c", file_path, NULL);
            perror("Error in execl");
            exit(1);
            
        default : // parent
            wait(0);
    }
}

void
gcov_creater(char * filename, char * dir_path, int argc, char **args)
{
    char * file_path;
    if((file_path = realpath(filename, 0x0)) == 0x0){
        perror("Error in real_path in gcov_creater");
        exit(1);
    }

    char ** argv = (char**) malloc(sizeof(char*) * (argc + 1));
    argv[0] = file_path;

    for(int i = 1 ; i < argc; i++){
        argv[i] = args[i - 1];
    }
    argv[argc] = 0x0;
    char * bin_path = get_bin_path(file_path);

    exec_gcc_coverage(file_path, bin_path);

    exec_bin(argv, bin_path);
    free(bin_path);
    free(argv);

    exec_gcov(file_path, dir_path);
    free(file_path);
}

char *
get_bin_path(char * file_path)
{
    char *bin_path = 0x0;
    int len = strlen(file_path);

    for(int i = len - 1 ; i >= 0 ; i--){
        if(file_path[i] == '.'){
            bin_path = (char*) calloc(i + 1, sizeof(char));

            assert(bin_path);
            strncpy(bin_path, file_path, i );
            bin_path[i - 1] = 0x0;  // null padding
            return bin_path; 
        }
    }
    return 0x0;
}


void
gcov_branch_creater(char * filename, char * dir_path, int argc, char **args)
{
    char * file_path;
    if((file_path = realpath(filename, 0x0)) == 0x0){
        perror("Error in real_path");
        exit(1);
    }
    

    char ** argv = (char**) malloc(sizeof(char*) * (argc + 1));
    argv[0] = file_path;

    for(int i = 1 ; i < argc; i++){
        argv[i] = args[i - 1];
    }
    argv[argc] = 0x0;

    char * bin_path = get_bin_path(file_path);
    exec_gcc_coverage(file_path, bin_path);

    exec_bin(argv, bin_path);
    free(bin_path);
    free(argv);

    exec_gcov_with_bc_option(file_path, dir_path);
    free(file_path);
}

void
exec_and_gcov(char * filename, char * dir_path, char * bin_path, int argc, char **args)
{
    char rel_path[PATH_MAX];
    char file_path[PATH_MAX];
    sprintf(rel_path, "./%s/%s", dir_path, filename);


    if(realpath(rel_path, file_path) == 0x0){
        perror("Error in real_path in exec_and_gcov");
        exit(1);
    }

    char ** argv = (char**) malloc(sizeof(char*) * (argc + 1));
    argv[0] = bin_path;

    for(int i = 1 ; i < argc; i++){
        argv[i] = args[i - 1];
    }
    argv[argc] = 0x0;

    exec_bin(argv, bin_path);
    free(argv);

    exec_gcov_with_bc_option(file_path, dir_path);
}



#ifdef DEBUG
int
main()
{
    char filename[] = "target/cgi_decode_ex.c";

    char *args[] = {"Send+mail+to+me+fuzzingbook.org"};
    char *dir_path = "./";
    int argc = 2;

    gcov_creater(filename, dir_path, argc, args);

    return 0;
}
#endif /* DEBUG */