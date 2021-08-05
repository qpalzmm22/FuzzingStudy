
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

	len = rand() % (max_length + 1); 

	rand_arr = (char*) malloc(sizeof(char) * (len + 1));

	for(int i = 0; i < len; i++){
		rand_arr[i] = char_start + rand() % (char_range + 1);
	}
	rand_arr[len] = '\0';

	return rand_arr;
}

// link the stdin, stderr, stdout and execute bc
// stdin << fuzzer
// stdout << temp/fuzz_stdout
// stderr << temp/fuzz_stderr
void make_out_files(pfile_info p_file_info, int i)
{
	// create output file
	// create error file
	
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


// creates subprocess
void create_subprocess(pfile_info p_file_info, int i)
{
	int status;
	int pid = fork();
	switch( pid ){
		case -1 : 
			perror("Fork Error");
			break;

		case 0 : // child process
			make_out_files(p_file_info, i);
			break;

		default :
			// return the return code.
			wait(&status);
			write_ret_code(status, i);
#ifdef DEBUG
			printf("created pid[%d]\n", pid);
#endif	
	}
}

