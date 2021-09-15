#ifndef HAVE_GCOV_CREATER_H 
#define HAVE_GCOV_CREATER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <assert.h>
#include <linux/limits.h>

void
exec_gcc_coverage(char *filepath, char* binpath);
void
exec_bin(char **argv, char * prog_path);
void
exec_gcov(char *filepath, char * gcpath);
void
gcov_creater(char * filename, char * gcpath, int argc, char **args);
void
gcov_branch_creater(char * filename, char * gcpath, int argc, char **args);
char *
get_bin_path(char * filepath);
void
exec_gcov_with_bc_option(char * filepath, char * gcpath);

#endif // !HAVE_GCOV_CREATER_H 