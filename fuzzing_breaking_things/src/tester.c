#include <stdio.h>
#include <stdlib.h>
#include <time.h>


#include "../include/my_fileIO.h"
#include "../include/fuzzer.h"


//#define CRTSUBPROC

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
	
	// mkdtemp
#ifdef MKDTMP
	
	mkfuzzed_tmp();
	
#endif // !PRACTICE

#ifdef CRTSUBPROC
	create_subprocess();
#endif
}
