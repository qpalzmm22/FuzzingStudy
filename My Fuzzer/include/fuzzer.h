#include "types.h"



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>

char *create_tmp_dirs();

ssize_t my_fwrite(void * ptr, ssize_t n, FILE * stream);
ssize_t my_fread(void * ptr, ssize_t n, FILE * stream);

void make_input_files(char* str, int len, char *file_path);

int exec_process(char * str, int len, int itr);
void init_fuzzer();

void fuzz_loop();