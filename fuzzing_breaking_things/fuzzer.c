#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>


char * fuzzer(int max_length, int char_start, int char_range)
{
	char * rand_arr;
	unsigned int strlen;	

	strlen = 1 + rand() % max_length; 

	rand_arr = (char*) malloc(sizeof(char) * strlen);

	for(int i = 0; i < strlen; i++){
		rand_arr[i] = char_start + rand() % char_range;
	}

	return rand_arr;
}

void mkfuzzed_tmp()
{
	char basename[] = "input.txt";
	char template[] = "temp_XXXXXX";
	char deliminator[] = "/";
	char *p_dir_name = (char*) malloc( 100 * sizeof(char));
	
	strcpy(p_dir_name, mkdtemp(template));
	
	strcat(p_dir_name, deliminator);
	strcat(p_dir_name, basename);
	printf("%s\n", p_dir_name);
	
	FILE *fp = fopen(p_dir_name, "w+");
	
	if(fp == 0x0){
		perror("Error opening file");
		exit(1);
	}

	char buffer[101];
	// Needs to be freed
	char * rand_arr = fuzzer(100, 32, 32);

	fputs(rand_arr, fp);
	
	free(rand_arr);
	fclose(fp);
	
	fp = fopen(p_dir_name, "r");
	
	if(fp == 0x0){
		perror("Error Opening File");
		exit(1);
	}
	
	fgets(buffer, 100, fp);

	printf("%s\n",buffer);
		
	fclose(fp);
}


