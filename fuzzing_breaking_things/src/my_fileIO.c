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