#include "../include/coverage_calculator.h"

#define DEBUG
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
    while(i < MAX_COVERAGE_LINE){
        if(coverage[i] >= 1){
            printf("(%d)", i);
        }
        i++;
    }
    printf("\n");
}

int
get_branch_coverage(int* coverage, int * branch_coverage)
{  
    int tot_cov = 0;
    for(int i = 0; i < MAX_COVERAGE_LINE - 1  ; i++){
        if(coverage[i] && coverage[i + 1]){
            branch_coverage[i] = 1;
            tot_cov++;
        } else if(i >0 && coverage[i-1] && coverage[i]){
            branch_coverage[i] = 1;
            tot_cov++;
        }
    }
    return tot_cov;
}


// Returns the size of coverage. Indicates the end of array by inserting -1 at the end.
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
    int tot_cov = 0;
    char line[4096];
    while(fgets(line, 4096, fp) != 0x0){
        char * covered = trim( strtok( line, ":" ));
        int line_number = atoi( trim( strtok( 0x0, ":" )));

        if(*covered == '-' || *covered == '#'){
            continue;
        }
        coverage[line_number] = 1;
        tot_cov++;
    }
    
    fclose(fp);
    return tot_cov;    
}

int 
read_gcov_coverage_with_bc_option(char * c_file, int * coverage)
{
    char target_file[PATH_MAX];
    
    sprintf(target_file, "%s.gcov", c_file);

    FILE * fp;
    if((fp = fopen(target_file, "r")) == NULL){
        perror("Error in open .gcov file");
        exit(1);
    }
    int tot_cov = 0;
    char line[4096];
    while(fgets(line, 4096, fp) != 0x0){

        if(strncmp(line, "branch", 6)){
            
        }
        // char * covered = trim( strtok( line, ":" ));
        // int line_number = atoi( trim( strtok( 0x0, ":" )));

        // if(*covered == '-' || *covered == '#'){
        //     continue;
        // }
        coverage[line_number] = 1;
        tot_cov++;
    }
    
    fclose(fp);
    return tot_cov;    
}

// Malloced. Need to be freed.
char *
extract_program(char *filepath)
{
    int len = strlen(filepath);
    for(int i = len - 1 ; i >= 0 ; i--){
        if(filepath[i] == '/'){
            char * filename = (char*) malloc(sizeof(char) * (len - i + 2));
            strncpy(filename, filepath + i + 1 , len - i + 2);
            return filename;
        }
    }
    return filepath;
}

int
remove_gcda(char *filepath)
{   
    char *gcda_path = 0x0;
    int len = strlen(filepath);
    for(int i = len - 1 ; i >= 0 ; i--){
        if(filepath[i] == '.'){
            gcda_path = (char*) calloc(i + 5, sizeof(char));

            assert(gcda_path);
            strncpy(gcda_path, filepath, i + 1);
            sprintf(gcda_path, "%s%s", gcda_path, "gcda");
            break;
        }
    }
    if(gcda_path == 0x0){
        return -1;
    } else{
        if(remove(gcda_path) != 0){
            free(gcda_path);
            perror("Error in removing gcda");
            exit(1);
        }
        free(gcda_path);
        return 0;
    }
}


void 
execute_calc()
{
    char filepath[] = "./cgi_decode_ex.c";
    char *args[] = {"Send+mail+to+me%40fuzzingbook.org"}; 
    int argc = 2;

    int coverage[MAX_COVERAGE_LINE] = {0};

    gcov_creater(filepath, argc, args);

    char * filename = extract_program(filepath);

    read_gcov_coverage(filename, coverage);
    
    free(filename);
    print_coverage(coverage);
}
void
execute_branch_cov()
{
    char filepath[] = "./cgi_decode_ex.c";
    char *args[] = {"Send+mail+to+me%40fuzzingbook.org"}; 
    int argc = 2;

    int coverage[MAX_COVERAGE_LINE] = {0};
    int branch_coverage[MAX_COVERAGE_LINE] = {0};

    gcov_creater(filepath, argc, args);

    char * filename = extract_program(filepath);

    read_gcov_coverage(filename, coverage);
    free(filename);

    get_branch_coverage(coverage, branch_coverage);

    print_coverage(coverage);
    print_coverage(branch_coverage);
}


#ifdef DEBUG
int 
main()
{
    //execute_calc();

    execute_branch_cov();

    return 0;
}

#endif /* DEBUG */