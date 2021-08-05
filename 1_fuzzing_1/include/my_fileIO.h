#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <limits.h>
#include <string.h>

typedef struct _file_info{
	char file_path[PATH_MAX];
	int fd;
}file_info, *pfile_info;

char *create_tmp_dir();
ssize_t my_fwrite(void * ptr, ssize_t n, FILE * stream);
ssize_t my_fread(void * ptr, ssize_t n, FILE * stream);
void write_ret_code(int retcode, int i);
pfile_info mkfuzzed_file(char* dir_path, int iter);
void make_out_files(pfile_info p_file_info, int i);