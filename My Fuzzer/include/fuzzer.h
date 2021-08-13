
#ifndef _HAVE_FUZZER_H_
#define _HAVE_FUZZER_H_

#include "config.h"


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>

char *create_tmp_dirs();

int my_fwrite(void * ptr, int n, FILE * stream);
int my_fread(void * ptr, int n, FILE * stream);

void make_input_files(char* str, int len, char *file_path);

int exec_process(char * str, int len, int itr);
void init_fuzzer();

void fuzz_loop();

#endif /* !_HAVE_FUZZER_H_ */
