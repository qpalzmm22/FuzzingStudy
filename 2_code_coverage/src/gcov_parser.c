#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <linux/limits.h> 

void read_gcov_coverage(char * c_file){
    
    strcat(c_file, ".gcov");

    printf("%s",c_file);

    FILE * fp;
    if((fp = fopen(c_file, "r")) == NULL){
        perror("Error in open .gcov file");
        exit(1);
    }

    char line[4096];
    while(fgets(line, 4096, fp) != 0x0){
        char * covered = trim(strtok(line, ":"));
        int line_number = atoi( trim( strtok(0x0, ":")));
        printf("coverd :%s, ln : %d\n", covered, line_number);
    }
        
    fclose(fp);
}

int main(){
    char filename[PATH_MAX] = "cgi_decode_ex.c";
    printf("%s",filename);
    read_gcov_coverage(filename);
    return 0;
}