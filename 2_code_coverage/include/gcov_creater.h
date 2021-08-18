#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <linux/limits.h>

void exec_gcc_coverage();
void exec_bin(char *arg);
void exec_gcov();
void gcov_creater(char* filename, char *arg);