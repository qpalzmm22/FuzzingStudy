#ifndef HAVE_GCOV_CREATER_H 
#define HAVE_GCOV_CREATER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <linux/limits.h>

void exec_gcc_coverage(char *filepath);
void exec_bin(char **argv);
void exec_gcov(char *filepath);
void gcov_creater(char * filename, int argc, char **args);
void gcov_branch_creater(char * filename, int argc, char **args);
void exec_gcov_with_bc_option(char * filepath);

#endif // !HAVE_GCOV_CREATER_H 