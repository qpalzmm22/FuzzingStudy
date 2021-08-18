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
    *(end + 1) = 0x0;

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
int 
read_gcov_coverage(char * c_file, int * coverage)
{
    char target_file[PATH_MAX];
    
    sprintf(target_file, "%s.gcov", c_file);


    FILE * fp;
    if((fp = fopen(c_file, "r")) == NULL){
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

void 
execute_calc()
{
    char filename[] = "cgi_decode_ex.c";

    int coverage[MAX_COVERAGE_LINE];

    gcov_creater(filename, "Send+mail+to+me%40fuzzingbook.org");
    read_gcov_coverage(filename, coverage);

    print_coverage(coverage);
}

// Returns new size. 'to' array has combined array. Size must be guaranteed before run.
// time complexity: O(n)... space complexity: O(n)
// int
// combine_set(int * to, int * from){
//     int tmp[MAX_COVERAGE_LINE];
//     int i = 0, j = 0, k = 0;
//     while(to[i] != -1){
//         if(to[i] > from[j]){
//             tmp[k++];
//         }
//     }
// }

int 
main()
{
    execute_calc();
    return 0;
}