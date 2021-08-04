// #3
#include "../include/fuzzer.h"
#include "../include/my_fileIO.h"

#define PROG_NAME "bc"

//#define DEBUG

char * fuzzer(int max_length, int char_start, int char_range)
{
	char * rand_arr;
	unsigned int len;	

	len = rand() % (max_length + 1); 

	rand_arr = (char*) malloc(sizeof(char) * (len + 1));

	for(int i = 0; i < len; i++){
		rand_arr[i] = char_start + rand() % (char_range + 1);
	}
	rand_arr[len] = '\0';

	return rand_arr;
}

// returns fd of fuzzed input
pfile_info mkfuzzed_tmp()
{
	char basename[] = "input.txt";
	char template[] = "temp_XXXXXX";
	char deliminator[] = "/";
	char *p_dir_name = (char*) malloc( PATH_MAX * sizeof(char));
	
	strcpy(p_dir_name, mkdtemp(template));
	
	strcat(p_dir_name, deliminator);
	strcat(p_dir_name, basename);
	printf("%s\n", p_dir_name);
	
	FILE *fp = fopen(p_dir_name, "w+");
	
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
	strcpy(pfile_info->file_path, p_dir_name);
	pfile_info->fd = fd;
#ifdef DEBUG	
	fp = fopen(p_dir_name, "r");
	
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
	free(p_dir_name);
	return pfile_info;
}


// link the stdin, stderr, stdout and execute bc
// stdin << fuzzer
// stdout << temp/fuzz_stdout
// stderr << temp/fuzz_stderr
void do_work()
{
	pfile_info p_file_info = mkfuzzed_tmp();
	
	//dup2( in_fd, stdin);
	char in_path[PATH_MAX];
	
	strcpy(in_path, p_file_info->file_path);
	free(p_file_info);

	char out_path[PATH_MAX];
	char err_path[PATH_MAX];
	fprintf(out_path, "%s/%s", in_path, "/../output");
	fprintf(err_path, "%s/%s", in_path, "/../error");
	

	int fd = create_write_file();
	dup2(fd, stderr);
	execlp(PROG_NAME, PROG_NAME, in_path, NULL);
}



// creates subprocess
void create_subprocess()
{
	int pid = fork();
	switch( pid ){
		case -1 : 
			perror("Fork Error");
			break;

		case 0 : // child process
			do_work();
			break;

		default :
			wait(0);
			printf("Done!\n");
	}
}