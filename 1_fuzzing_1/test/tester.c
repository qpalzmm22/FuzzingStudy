#include <stdio.h>
#include <stdlib.h>
#include <time.h>


#include "../include/my_fileIO.h"
#include "../include/fuzzer.h"

#define PROG_NAME "bc"

//#define FUZZER
//#define CRTSUBPROC
#define lONGFUZZ

#define READEND 0
#define WRITEEND 1


// creates subprocess
int create_subproc_work(char* dir_name, pfile_info p_file_info, int i)
{
	int flag = 0;
	int status;
	int pipe_in[2];
	int pipe_out[2];
	int pipe_err[2];

	pipe(pipe_in);
	pipe(pipe_out);
	pipe(pipe_err);


	int pid = fork();
	switch( pid ){
		case -1 : 
			perror("Fork Error");
			break;

		case 0 : // child process
			dup2(pipe_in[READEND], STDIN_FILENO) ;
			close(pipe_in[READEND]) ;
			close(pipe_in[WRITEEND]);

			close(pipe_out[READEND]); 
			close(pipe_err[READEND]);

			dup2(pipe_out[WRITEEND], STDOUT_FILENO);
			dup2(pipe_err[WRITEEND], STDERR_FILENO);

			execlp(PROG_NAME, PROG_NAME, p_file_info->file_path, NULL);
			
			break;

		default : // parent
			close(pipe_in[WRITEEND]);
			close(pipe_out[WRITEEND]);
			close(pipe_err[WRITEEND]);

			// Receive inputs from user
			// [!] Buf size fix

			char out_file_name[32]; // len = 19
			char err_file_name[32];
			sprintf(out_file_name, "%s%s%d", dir_name, "/outputs/output", i);
			sprintf(err_file_name, "%s%s%d", dir_name, "/errors/error", i);

			FILE *fp =  fopen(out_file_name, "wb");
			if(fp == 0){
				perror("ERROR in creating output.txt");
				exit(1);
			}

			char out_buff[1024];
			ssize_t s;

			while((s = read(pipe_out[READEND], out_buff, 1024)) > 0){
				if(fwrite(out_buff, 1, s, fp) < s){
					fprintf(stderr, "Error in making out files\n");
				}
			}
			
			close(pipe_out[READEND]);
			fclose(fp);

			fp =  fopen(err_file_name, "wb");
			if(fp == 0){
				perror("ERROR in creating output.txt");
				exit(1);
			}
			char err_buff[1024];

			while((s = read(pipe_err[READEND], err_buff, 1024)) > 0){
				if(fwrite(err_buff, 1, s, fp) < s){
					fprintf(stderr, "Error in making err files\n");
				}
			}
			
			fclose(fp);
			
			close(pipe_err[READEND]);

			// return the return code.
			wait(&status);
			if(status != 0){
				printf("%d - th input created %d error", i, status);
				flag = 1;
			}
#ifdef DEBUG
			printf("created pid[%d]\n", pid);
#endif	
	}
	return flag;
}

int long_running_fuzzing(int trials)
{
	// creates tmp directory
	char * dir_name = create_tmp_dirs();
	int bug_count = 0;

	for(int i = 0 ; i < trials; i++){
		// Make [trials] input files
		pfile_info p_file_info = mkfuzzed_file(dir_name, i);
		
		// processes make input files
		bug_count += create_subproc_work(dir_name, p_file_info, i);
		free(p_file_info);
	}
	free(dir_name);
	return bug_count;
}

int main()
{
	srand(time(0x0));
#ifdef FUZZER
	// Fuzzer
	char * rand_char_arr;
	// 10 fuzzer strings

	for(int i = 0; i < 10; i++){
		rand_char_arr = fuzzer(10, 60, 30);
		printf("%s\n", rand_char_arr);
		free(rand_char_arr); 
	}

	// lowercases only
	rand_char_arr = fuzzer(1000, 97, 26);
   	printf("%s\n",rand_char_arr);
	free(rand_char_arr);
#endif

#ifdef lONGFUZZ
	printf("Bugs found : %d\n", long_running_fuzzing(100));	
#endif
}



