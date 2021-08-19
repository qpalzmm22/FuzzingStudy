#ifndef HAVE_GCOV_CREATER_H 
#define HAVE_GCOV_CREATER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <linux/limits.h>

void exec_gcc_coverage();
void exec_bin(char **argv);
void exec_gcov();
void gcov_creater(char * filename, int argc, char **args);

#endif // !HAVE_GCOV_CREATER_H 