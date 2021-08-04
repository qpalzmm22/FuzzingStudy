#include "fuzzer.c"

//#define FUZZER
#define MKDTMP

int main()
{
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

}
