#define  _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>


/*
 *
 */

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

void call_sub_process(){
	char template[] = "inputXXXXXX";
	int fd;
	fd = mkstemp(template);
	 printf("file_d : %d\n", fd);
	// readlink("/proc/self/fd/3", filename, 1024);asd
	//printf("file_name : %s\n", filename);
}
