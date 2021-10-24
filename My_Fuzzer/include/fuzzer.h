
#ifndef _HAVE_FUZZER_H_
#define _HAVE_FUZZER_H_

#define MAX_LOG_LEN 1024

#include "types.h"
#include "config.h"
#include "fuzz_input_maker.h"
#include "gcov_creater.h"
#include "coverage_calculator.h"
#include "utility.h"
#include "power_schedule.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <errno.h>

char * create_tmp_dirs();

int union_branch_cov(unsigned short b_hash);

void make_input_files(char* str, int len, char *in_data_path);
int exec_process(char * str, int len, int itr, char *out_path, char *err_path);

void make_argv();

int default_oracle(int exit_code, char* input, int input_len, char* stdout_buff, char* stderr_buff);
void init_fuzzer(pConfig_t config);

void print_result(void);
void write_log(char * log_dir_path, double time);

void signal_handler(int sig);
void fuzz_main(pConfig_t config);


#endif /* !_HAVE_FUZZER_H_ */
