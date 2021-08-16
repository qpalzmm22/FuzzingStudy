
#ifndef _HAVE_FUZZER_H_
#define _HAVE_FUZZER_H_

#include "config.h"


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>

char *create_tmp_dirs();

int my_fwrite(void * ptr, int n, FILE * stream);
int my_fread(void * ptr, int n, FILE * stream);

void make_input_files(char* str, int len, char *in_data_path);
int exec_process(char * str, int len, int itr, char *out_path, char *err_path);

void make_argv();

int default_oracle(int exit_code, char* input, int input_len, char* stdout_buff, char* stderr_buff);
void init_fuzzer(int (*oracle)(int, char*, int, char*, char*));

void alarm_handler(int sig);
void fuzz_loop();


#endif /* !_HAVE_FUZZER_H_ */
