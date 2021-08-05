
#include "../include/my_fileIO.h"
#include "../include/fuzzer.h"

#define PROG_NAME "bc"

#define READEND 0
#define WRITEEND 1


#define DEBUG

char * fuzzer(int max_length, int char_start, int char_range)
{
	char * rand_arr;
	unsigned int len;	

	len = rand() % (max_length) + 1; 

	rand_arr = (char*) malloc(sizeof(char) * (len + 1));

	for(int i = 0; i < len; i++){
		rand_arr[i] = char_start + rand() % (char_range + 1);
	}
	rand_arr[len] = '\0';

	return rand_arr;
}

// link the stdin, stderr, stdout and execute bc
void make_out_files(pfile_info p_file_info, int i)
{
	
	char out_file_name[32]; // len = 19
	char err_file_name[32];
	sprintf(out_file_name, "%s%d", "result/outputs/output", i);
	sprintf(err_file_name, "%s%d", "result/errors/error", i);
	
	int devnull = open("/dev/null", O_RDONLY);
	dup2(devnull, 0);
	
	int out_fd = open(out_file_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if(out_fd < 0){
		perror("ERROR in creating output.txt");
	}
	int err_fd = open(err_file_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if(err_fd < 0){
		perror("ERROR in creating error.txt");	
	}

	// Redirect them
	dup2(out_fd, 1);
	dup2(err_fd, 2);
	
	execlp(PROG_NAME, PROG_NAME, p_file_info->file_path, NULL);
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

