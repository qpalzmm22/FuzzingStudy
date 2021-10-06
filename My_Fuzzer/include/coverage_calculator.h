#ifndef _HAVE_COVERAGE_CALCULATOR_H_
#define _HAVE_COVERAGE_CALCULATOR_H_

#include "gcov_creater.h"
#include "types.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <linux/limits.h>
#include <assert.h>
#include <regex.h>
#include <dirent.h>


void
free_N(void ** pp_alloc, int num);

char * 
trim(char* str);

void
print_coverage(int * coverage);

void
print_branch_coverage(cov_info_t ** pb_info, int num_line_w_branches);


int 
read_gcov_coverage_with_bc_option(char * c_file, cov_info_t * pb_info);

int
get_file_names(char *src_dirpath, char ** src_array);

int
remove_gcda(char *filepath);

int
gcov_multiple(char ** src_array, int num_files, char * src_dir_path, cov_info_t ** pcov_info);

char *
extract_filename(char *filepath);

#endif /* !_HAVE_COVERAGE_CALCULATOR_H_ */