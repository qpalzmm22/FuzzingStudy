#ifndef _HAVE_COVERAGE_CALCULATOR_H_
#define _HAVE_COVERAGE_CALCULATOR_H_

#define MAX_BRANCH 32

typedef struct _branch_result{
    int line_num;
    int num_branch;
    int runs[MAX_BRANCH];
} b_result_t;


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <linux/limits.h>
#include <assert.h>
#include <regex.h>

#include "../include/gcov_creater.h"

char * 
trim(char* str);

void
print_coverage(int * coverage);

void
print_branch_coverage(b_result_t * p_result_t, int num_line_w_branches);

int 
read_gcov_coverage(char * c_file, int * coverage);

int 
read_gcov_coverage_with_bc_option(char * c_file, b_result_t * p_result_t);

int
remove_gcda(char *filepath);

char *
extract_program(char *filepath);

void 
execute_calc(char* filepath, char ** args, int argc, int * coverage);

void
execute_branch_cov(char* filepath, char ** args, int argc, b_result_t *p_result_t);

#endif /* !_HAVE_COVERAGE_CALCULATOR_H_ */