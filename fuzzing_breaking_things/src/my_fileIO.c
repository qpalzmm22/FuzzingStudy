#include "../include/my_fileIO.h"
#include "../include/fuzzer.h"


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

pfile_info
mkfuzzed_file(char* dir_path, int iter)
{
	char file_name[32];
	char file_path[PATH_MAX];
	printf("i : %d\n", iter);
	sprintf(file_name, "input%d.txt", iter);
	sprintf(file_path, "%s/%s", dir_path, file_name);
	printf("%s\n", file_path);

	
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
#ifdef DEBUG	
	fp = fopen(file_path, "r");
	
	if(fp == 0x0){
		perror("Error Opening File");
		exit(1);
	}

	char buffer[101];
	if (my_fread(buffer, fuzzed_len, fp) != fuzzed_len ){
		fprintf(stderr, "ERROR in my_fread");
	}
	printf("%s\n",buffer);
	printf("fd : %d\n", fd);

	fclose(fp);

	printf("filename : %s\n",pfile_info->file_path);
	printf("fd : %d\n", pfile_info->fd);
#endif
	return pfile_info;
}

char 
*create_tmp_dir()
{
	char template[] = "tmp_XXXXXX";
	//char *p_dir_name = mkdtemp(template);
	char * p_dir_name = malloc( 32 * sizeof(char));
	
	strcpy(p_dir_name, mkdtemp(template));	

	return p_dir_name;
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