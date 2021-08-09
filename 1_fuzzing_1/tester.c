#include <stdio.h>
#include <stdlib.h>
#include <time.h>


#include "./include/my_fileIO.h"
#include "./include/fuzzer.h"

//#define FUZZER
//#define CRTSUBPROC
#define lONGFUZZ

// creates subprocess
int create_subproc_work(char* dir_name, pfile_info p_file_info, int i)
{
	int flag = 0;
	int status;
	int pid = fork();
	switch( pid ){
		case -1 : 
			perror("Fork Error");
			break;

		case 0 : // child process
			make_out_files(dir_name, p_file_info, i);
			break;

		default :
			// return the return code.
			wait(&status);
			if(status != 0){
				printf("%d - th input created %d error", i, status);
				flag = 1;
			}
			
			write_ret_code(status, i);
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


