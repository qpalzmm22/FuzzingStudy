#include <stdio.h>
#include <stdlib.h>
#include <math.h>
/*
 *
 */

// runs bc in subprocess

char * fuzzer(int max_length, int char_start, int char_range){
	char * rand_arr;
	unsigned int strlen;	

	strlen = 1 + rand() % max_length; 

	rand_arr = (char*) malloc(sizeof(char) * strlen);

	for(int i = 0; i < strlen; i++){
		rand_arr[i] = char_start + rand() % char_range;
	}

	return rand_arr;
}

