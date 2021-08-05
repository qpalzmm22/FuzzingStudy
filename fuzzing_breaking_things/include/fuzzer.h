#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <limits.h>
#include <sys/wait.h>
#include <fcntl.h>

void make_out_files(pfile_info p_file_info, int i);
char * fuzzer(int max_length, int char_start, int char_range);
