#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <limits.h>
#include <string.h>

typedef struct _file_info{
	char file_path[PATH_MAX];
	int fd;
}file_info, *pfile_info;


pfile_info mkfuzzed_file(char* dir_path, int iter);
char *create_tmp_dir();
ssize_t my_fwrite(void * ptr, ssize_t n, FILE * stream);
ssize_t my_fread(void * ptr, ssize_t n, FILE * stream);
void write_ret_code(int retcode, int i);