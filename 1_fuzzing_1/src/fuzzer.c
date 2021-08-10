
#include "../include/my_fileIO.h"
#include "../include/fuzzer.h"

// #define DEBUG

char * fuzzer(int max_length, int char_start, int char_range)
{
	char * rand_arr;
	unsigned int len;	

	len = rand() % (max_length) + 1; 

	rand_arr = (char*) malloc(sizeof(char) * (len + 1));
	assert(rand_arr);

	for(int i = 0; i < len; i++){
		rand_arr[i] = char_start + rand() % (char_range + 1);
	}
	rand_arr[len] = '\0';

	return rand_arr;
}

// Specified in the fuzzingbook.org
char *default_fuzzer()
{
	return fuzzer(100, 32, 32);
}

// fuzz method from RandomFuzzer class 
// buffer is allocated, so needs to be freed.
char * random_fuzzer(int min_len, int max_len, int char_start, int char_range)
{
	
	char * rand_arr;
	int len = rand() % (max_len - min_len + 1) + min_len ;

	rand_arr = (char *) malloc(sizeof(char) * (len + 1));
	assert(rand_arr);

	for(int i = 0; i < len; i++){
		rand_arr[i] = char_start + rand() % (char_range + 1);
	}
	rand_arr[len] = '\0';

	return rand_arr;
}

// buffer is allocated, so needs to be freed.
char *default_rand_fuzzer()
{
	return random_fuzzer(10, 100, 32, 32); 
}

// Same as fuzzer exept it receives char * (buffer) to store sting and returns the size of length
// buffer is allocated, so needs to be freed.
int fuzzer_n(int max_length, int char_start, int char_range, char ** buffer)
{
	unsigned int len;	

	len = rand() % (max_length) + 1; 

	*buffer = (char*) malloc(sizeof(char) * (len + 1));
	assert(buffer);

	for(int i = 0; i < len; i++){
		(*buffer)[i] = char_start + rand() % (char_range + 1);
	}
	(*buffer)[len] = '\0';

	return len;
}

// default fuzzer with return value of fuzzed string length
int default_fuzzer_n(char** buffer){
	return fuzzer_n(100, 32, 32, buffer);
}
