#ifndef _HAVE_COVERAGE_CALCULATOR_H_
#define _HAVE_COVERAGE_CALCULATOR_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <linux/limits.h>
#include <assert.h>

#include "../include/gcov_creater.h"

char * 
trim(char* str);

void
print_coverage(int * coverage);

int 
read_gcov_coverage(char * c_file, int * coverage);

int
remove_gcda(char *filepath);

char *
extract_program(char *filepath);

void 
execute_calc();

#endif /* !_HAVE_COVERAGE_CALCULATOR_H_ */