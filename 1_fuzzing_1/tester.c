#include <stdio.h>
#include <stdlib.h>
#include <time.h>


#include "./include/my_fileIO.h"
#include "./include/fuzzer.h"

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
	int pipeout[2];
	int pipeerr[2];

	pipe(pipeout);
	pipe(pipeerr);

	int pid = fork();
	switch( pid ){
		case -1 : 
			perror("Fork Error");
			break;

		case 0 : // child process

			close(pipeout[READEND]);
			close(pipeerr[READEND]);

			int devnull = open("/dev/null", O_RDONLY);
	
			dup2(devnull, 0);

			dup2(pipeout[WRITEEND], STDOUT_FILENO);
			dup2(pipeerr[WRITEEND], STDERR_FILENO);

			execlp(PROG_NAME, PROG_NAME, p_file_info->file_path, NULL);
			
			break;

		default :

			close(pipeout[WRITEEND]);
			close(pipeerr[WRITEEND]);
			
			// return the return code.
			wait(&status);

			if(status != 0){
				printf("%d - th input created %d error", i, status);
				flag = 1;
			}

			// Receive inputs from user
			// [!] Buf size fix
			char out_buff[16284];
			char err_buff[16284];
			ssize_t out_n, err_n;

			out_n = my_read(pipeout[READEND], out_buff);
			err_n = my_read(pipeerr[READEND], err_buff);

			close(pipeout[READEND]);
			close(pipeerr[READEND]);


			make_out_files(dir_name, i, out_buff, out_n, err_buff, err_n);


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


