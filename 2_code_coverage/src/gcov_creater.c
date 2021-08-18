#include "../include/gcov_creater.h"


char *g_filepath;

void
exec_gcc_coverage()
{
    switch(fork()){
        case -1 :
            perror("Error in fork");
            exit(1);
            break;
        case 0 : // child

            execl("/usr/bin/gcc", "/usr/bin/gcc", "--coverage", g_filepath, NULL);

            perror("Error in execl");
            exit(1);

        default : // parent
            wait(0);       
    }
    
}

// Takes only one argument
void 
exec_bin(char *arg)
{
    char *p_bin_path;

    if((p_bin_path = realpath("a.out", 0x0)) == 0x0){
        perror("Error in real_path");
        exit(1);
    }
    switch(fork()){
        case -1 :
            perror("Error in fork");
            exit(1);
            break;
        case 0 : // child
            // char bin_path[PATH_MAX];

            execl(p_bin_path, p_bin_path, arg, NULL);
            perror("Error in execl");
            exit(1);
            
        default : // parent
            wait(0);
    }
    free(p_bin_path);
}

void
exec_gcov()
{
    switch(fork()){
        case -1 :
            perror("Error in fork");
            exit(1);
            break;
        case 0 : // child

            execl("/usr/bin/gcov", "/usr/bin/gcov", g_filepath, NULL);
            perror("Error in execl");
            exit(1);
            
        default : // parent
            wait(0);
    }
}

void
gcov_creater(char * filename, char *arg)
{
    char buff[PATH_MAX];
    if((g_filepath = realpath(filename, 0x0)) == 0x0){
        perror("Error in real_path");
        exit(1);
    }

    exec_gcc_coverage();

    exec_bin(arg);

    exec_gcov();
    free(g_filepath);
}
