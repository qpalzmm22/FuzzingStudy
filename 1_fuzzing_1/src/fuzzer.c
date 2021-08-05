
#include "../include/my_fileIO.h"
#include "../include/fuzzer.h"


#define READEND 0
#define WRITEEND 1


// #define DEBUG

char * fuzzer(int max_length, int char_start, int char_range)
{
	char * rand_arr;
	unsigned int len;	

	len = rand() % (max_length) + 1; 

	rand_arr = (char*) malloc(sizeof(char) * (len + 1));

	for(int i = 0; i < len; i++){
		rand_arr[i] = char_start + rand() % (char_range);
	}
	rand_arr[len] = '\0';

	return rand_arr;
}

// Specified in the fuzzingbook.org
char *default_fuzzer(){
	return fuzzer(100, 32, 32);
}


