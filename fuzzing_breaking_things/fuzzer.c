#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>


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
	char basename[] = "input.txt";
	char template[] = "temp_XXXXXX";
	char deliminator[] = "/";
	char *p_dir_name = (char*) malloc(100);
	
	strcpy(p_dir_name, mkdtemp(template));
	
	strcat(p_dir_name, deliminator);
	strcat(p_dir_name, basename);
	printf("%s\n", p_dir_name);
	
	FILE *fp = fopen(p_dir_name, "wr");
	
	if(fp == 0x0){
		perror("Error opening file");
		exit(1);
	}

	char buffer[101];
	// Needs to be freed
	char * rand_arr = fuzzer(100, 32, 32);
	int n;


	//while(n = fwrite(rand_arr,))
	//fwrite(rand_arr, fp);
	fputs(rand_arr, fp);
	fgets(buffer ,100, fp);

	printf("%s\n",buffer);
	
	free(rand_arr);
		
	fclose(fp);
	
}
