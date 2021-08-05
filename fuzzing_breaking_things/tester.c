#include <stdio.h>
#include <stdlib.h>
#include <time.h>


#include "./include/my_fileIO.h"
#include "./include/fuzzer.h"

//#define FUZZER
//#define CRTSUBPROC
//#define CRTSUBPROC

void long_running_fuzzing(int trials)
{
	// creates tmp directory
	char * dir_name = create_tmp_dir();

	for(int i = 0 ; i < trials; i++){
		// Make [trials] input files
		pfile_info p_file_info = mkfuzzed_file(dir_name, i);
		
		// processes make input files
		create_subprocess(p_file_info, i);
	}

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
#ifdef CRTSUBPROC
	create_subprocess();
#endif
	
	long_running_fuzzing(100);	
}


