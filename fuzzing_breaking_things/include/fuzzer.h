#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <limits.h>

//#define FUZZER
//#define MKDTMP
#define CRTSUBPROC

typedef struct _file_info{
	char file_path[PATH_MAX];
	int fd;
}file_info, *pfile_info;

char * fuzzer(int max_length, int char_start, int char_range);
pfile_info mkfuzzed_tmp();
void do_work();
void create_subprocess();