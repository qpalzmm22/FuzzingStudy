#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <linux/limits.h>

#include "../include/gcov_creater.h"

#define MAX_COVERAGE_LINE 4096

char * 
trim(char* str)
{   
    // ltrim
    while(*str != 0x0 && isspace(*str)){
        str++;
    }

    // rtrim
    int len = strlen(str);
    char * end = str + len - 1;
    while(str != end && isspace(*end)){
        end--;
    }

    *(end+1)= 0x0;

    return str;
}

void
print_coverage(int * coverage)
{
    printf("Covered lines :\n");
    int i = 0;    
    while(coverage[i] != -1){
        printf("(%d)", coverage[i++]);
    }
}

// Returns the size of coverage. Indicates the end of array by inserting -1 at the end.
// Change to linked list ? 
int 
read_gcov_coverage(char * c_file, int * coverage)
{
    char target_file[PATH_MAX];
    
    sprintf(target_file, "%s.gcov", c_file);

    FILE * fp;
    if((fp = fopen(target_file, "r")) == NULL){
        perror("Error in open .gcov file");
        exit(1);
    }
    
    int i = 0;
    char line[4096];
    while(fgets(line, 4096, fp) != 0x0){
        char * covered = trim( strtok( line, ":" ));
        int line_number = atoi( trim( strtok( 0x0, ":" )));

        if(*covered == '-' || *covered == '#'){
            continue;
        }
        coverage[i++] = line_number;
    }
    coverage[i] = -1;
    
    fclose(fp);
    return i;    
}

// Malloced. Need to be freed.
char *
extract_program(char *filepath)
{
   
    int len = strlen(filepath);
    for(int i = len - 1 ; i >= 0 ; i--){
        if(filepath[i] == '/'){
            char * filename = (char*) malloc(sizeof(char) * (len - i + 1));
            strncpy(filename, filepath + i + 1 , len - i + 1);
            return filename;
        }
    }
    return 0x0;
}


void 
execute_calc()
{
    char filepath[] = "./cgi_decode_ex.c";
    char *args[] = {"Send+mail+to+me%40fuzzingbook.org"}; 
    int argc = 2;

    int coverage[MAX_COVERAGE_LINE];


    gcov_creater(filepath, argc, args);

    printf("hello\n");

    char * filename = extract_program(filepath);
    if(filename == 0x0){
        fprintf(stderr, "You must give file path\n");
        exit(1);
    }
    printf("fn : %s\n", filename);

    read_gcov_coverage(filename, coverage);
    
    free(filename);
    print_coverage(coverage);
}

int 
main()
{
    execute_calc();
    return 0;
}