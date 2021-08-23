#include "../include/coverage_calculator.h"

// #define DEBUG
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

void
print_branch_coverage(b_result_t * p_result_t, int num_line_w_branches)
{
    printf("Covered branches :\n");
    for(int i = 0; i < num_line_w_branches; i++){
        printf("%d : \n", p_result_t[i].line_num);
        for(int j = 0; j < p_result_t[i].num_branch; j++ ){
            printf("[%d] => %d\n", j, p_result_t[i].runs[j]);
        }
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
read_gcov_coverage_with_bc_option(char * c_file, b_result_t * p_result_t)
{
    char target_file[PATH_MAX];
    
    sprintf(target_file, "%s.gcov", c_file);

    FILE * fp;
    if((fp = fopen(target_file, "r")) == NULL){
        perror("Error in open .gcov file");
        exit(1);
    }
    int branch_flag = 0;
    int branch_itr = 0;
    char line[4096];
    int line_number = 0;
    int tot_branches = 0;
    p_result_t[branch_itr].num_branch = 0;
 

    while(fgets(line, 4096, fp) != 0x0){
        if(strncmp(line, "branch", 6) == 0){

            branch_flag = 1;

            p_result_t[branch_itr].line_num = line_number;

            char* tokens[4];
            tokens[0] = strtok(line, " ");
            
            for(int i = 0; i < 3; i++){            
                tokens[i + 1] = strtok(0x0, " ");
            }

            if(strncmp(tokens[2], "never", 5) == 0 ){
                p_result_t[branch_itr].runs[p_result_t[branch_itr].num_branch] = 0;
            } else {
                p_result_t[branch_itr].runs[p_result_t[branch_itr].num_branch] = (atoi(tokens[3]) > 0 ? 1 : 0);
            }
            p_result_t[branch_itr].num_branch++;
            tot_branches++;
        } else if(strncmp(line, "branch", 6) != 0 && strncmp(line, "call", 4) != 0 && strncmp(line, "function", 8) != 0){
            // line coverage
            char * covered = trim( strtok( line, ":" ));
            line_number = atoi( trim( strtok( 0x0, ":" )));
            
            if(branch_flag == 1){
                branch_itr++;
            }
            branch_flag = 0;
            continue;         
        }else {
            if(branch_flag == 1){
                branch_itr++;
            }
            branch_flag = 0;
        }
    }
    
    fclose(fp);
    return branch_itr;    
}

// Malloced. Need to be freed.
char *
extract_program(char *filepath)
{
    char * filename = 0x0;
    int len = strlen(filepath);
    for(int i = len - 1 ; i >= 0 ; i--){
        if(filepath[i] == '/'){
            filename = (char*) malloc(sizeof(char) * (len - i + 2));
            strncpy(filename, filepath + i + 1 , len - i + 2);
            return filename;
        }
    }
    if(filename == 0x0){ // filepath = relative path
        filename = (char *) malloc(sizeof(char) * (len + 1));
        strcpy(filename, filepath);
    }

    return filename;
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
execute_calc(char* filepath, char ** args, int argc, int * coverage)
{
    gcov_creater(filepath, argc, args);

    char * filename = extract_program(filepath);
    read_gcov_coverage(filename, coverage);
    free(filename);

#ifdef DEBUG
    print_coverage(coverage);
#endif // DEBUG
}

void
execute_branch_cov(char* filepath, char ** args, int argc, b_result_t *p_result_t)
{
    gcov_branch_creater(filepath, argc, args);

    char * filename = extract_program(filepath);
    int num_line_w_branches = read_gcov_coverage_with_bc_option(filename, p_result_t);
    free(filename);
    
#ifdef DEBUG
    print_branch_coverage(p_result_t, num_line_w_branches);
#endif // DEBUG
}


#ifdef DEBUG
int 
main()
{
    //execute_calc();

    char filepath[] = "cgi_decode_ex.c";
    char *args[] = {"Send+mail+to+me+fuzzingbook.org"}; 
    int argc = 2;
    b_result_t b_coverages[MAX_COVERAGE_LINE] = {};
    execute_branch_cov(filepath, args, argc, b_coverages);

    return 0;
}

#endif /* DEBUG */