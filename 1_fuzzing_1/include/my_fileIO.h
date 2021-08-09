#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <limits.h>
#include <string.h>

typedef struct _file_info{
	char file_path[PATH_MAX];
	int fd;
}file_info, *pfile_info;

char *create_tmp_dirs();
ssize_t my_fwrite(void * ptr, ssize_t n, FILE * stream);
ssize_t my_fread(void * ptr, ssize_t n, FILE * stream);
ssize_t my_read(int fd, char* buf);
ssize_t my_write(int fd, char *buf, ssize_t n);
void write_ret_code(int retcode, int i);
pfile_info mkfuzzed_file(char* dir_path, int iter);
void make_out_files(char *dir_name, int i, char* out_buff, ssize_t out_n, char* err_buff, ssize_t err_n);