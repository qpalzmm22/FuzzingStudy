#include "../include/my_fileIO.h"
#include "../include/fuzzer.h"

#define PROG_NAME "bc"

ssize_t
my_read(int fd, char* buf){

	ssize_t s ;
	ssize_t tot_size = 0 ;

	while((s = read(fd, buf, 8192)) > 0){
		tot_size += s;
	}

	return tot_size;
}

ssize_t
my_write(int fd, char *buf, ssize_t n) {
	
	ssize_t sent = 0;

	while(sent < n){
		sent += write(fd, buf + sent, n - sent);
	}

	return sent;
}

// fwrite() safe version
ssize_t
my_fwrite(void * ptr, ssize_t n, FILE * stream)
{
	ssize_t sent = 0;

	while(sent < n){
		sent += fwrite(ptr + sent, n - sent, 1, stream);
	}

	return sent;
}

// fread() safe version
ssize_t
my_fread(void * ptr, ssize_t n, FILE * stream)
{
	ssize_t received = 0;

	while(received < n){
		received += fread(ptr + received , n - received, 1, stream);
	}

	return received;
}

char 
*create_tmp_dirs()
{
	char template[] = "tmp_XXXXXX";
	//char *p_dir_name = mkdtemp(template);
	char * dir_name = malloc( 32 * sizeof(char));
	
	strcpy(dir_name, mkdtemp(template));	

	char in_dir_path[32];
	char out_dir_path[32];
	char err_dir_path[32];

	// make dirs
	sprintf(in_dir_path, "%s%s", dir_name, "/inputs");
	sprintf(out_dir_path, "%s%s", dir_name, "/outputs");
	sprintf(err_dir_path, "%s%s", dir_name, "/errors");

	if(mkdir(in_dir_path, S_IRWXU) != 0 ){
		perror("ERROR in creating in directory");
	} 
	if(mkdir(out_dir_path, S_IRWXU) != 0 ){
		perror("ERROR in creating out directory");
	} 
	if(mkdir(err_dir_path, S_IRWXU) != 0 ){
		perror("ERROR in creating err directory");
	} 


	return dir_name;
}

void write_ret_code(int retcode, int i){

	char out_file_name[32]; // len = 19
	sprintf(out_file_name, "%s%d", "result/outputs/output", i);

	char return_string[32];
	sprintf(return_string, "Return code : %d\n", retcode);

	FILE * fp = fopen(out_file_name, "a");
	
	if(fp == 0x0){
		perror("Error opening file");
		exit(1);
	}
	fprintf(fp, "%s",return_string);

	fclose(fp);
}

// make output, error files
void make_out_files(char *dir_name, int i, char* out_buff, ssize_t out_n, char* err_buff, ssize_t err_n)
{
	char out_file_name[32]; // len = 19
	char err_file_name[32];
	sprintf(out_file_name, "%s%s%d", dir_name, "/outputs/output", i);
	sprintf(err_file_name, "%s%s%d", dir_name, "/errors/error", i);
	
	int out_fd = open(out_file_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if(out_fd < 0){
		perror("ERROR in creating output.txt");
	}
	if( my_write(out_fd, out_buff, out_n) != out_n){
		fprintf(stderr, "Error in making out files\n");
	}

	close(out_fd);

	int err_fd = open(err_file_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if(err_fd < 0){
		perror("ERROR in creating error.txt");	
	}

	if( my_write(err_fd, err_buff, err_n) != err_n){
		fprintf(stderr, "Error in making out files\n");
	}

	close(err_fd);
}


pfile_info
mkfuzzed_file(char* dir_path, int iter)
{
	char file_path[PATH_MAX];
	sprintf(file_path, "%s%s%d", dir_path, "/inputs/input", iter);

	
	FILE *fp = fopen(file_path, "w+");
	
	if(fp == 0x0){
		perror("Error opening file");
		exit(1);
	}

	// Needs to be freed
	char * rand_arr = fuzzer(100, 32, 32);

	// fputs(rand_arr, fp);
	// [!] Does not consider rand_arr obtaining \x00;
	ssize_t fuzzed_len = strlen(rand_arr);
	if (my_fwrite(rand_arr, fuzzed_len, fp) != fuzzed_len ){
		fprintf(stderr, "ERROR in my_fwrite");
	}
	 
	free(rand_arr);
	int fd = fileno(fp);

	fclose(fp);

	file_info * pfile_info = malloc(sizeof(file_info));
	strcpy(pfile_info->file_path, file_path);
	pfile_info->fd = fd;


	return pfile_info;
}

