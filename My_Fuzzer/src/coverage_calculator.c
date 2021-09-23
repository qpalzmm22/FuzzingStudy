#include "../include/coverage_calculator.h"

// #define DEBUG

void
free_N(void ** pp_alloc, int num)
{
    for(int i = 0; i < num; i++){
        free(pp_alloc[i]);
    }
    free(pp_alloc);
}

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
print_branch_coverage(cov_info_t ** pp_cov_info, int num_files)
{
    printf("Covered branches :\n");
    for(int i = 0 ; i < num_files ; i++){
        printf("File : %s\n", pp_cov_info[i]->file_name);

        for(int j = 0 ; j < pp_cov_info[i]->tot_branches; j++ ){
            printf("%d : \n", pp_cov_info[i]->b_infos[j].line_num);
            
            for(int k = 0 ; k < pp_cov_info[i]->b_infos[j].num_branch; k++){
                printf("[%d] => %d\n", k, pp_cov_info[i]->b_infos[j].runs[k]);
            }
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

// Returns -1 when there is no file named `c_file.gcov` which is very possible.
int 
read_gcov_coverage_with_bc_option(char * c_file, cov_info_t * pcov_info)
{
    b_info_t * pb_info = pcov_info->b_infos;
    char target_file[PATH_MAX];
    
    sprintf(target_file, "%s.gcov", c_file);

    FILE * fp;
    if((fp = fopen(target_file, "r")) == NULL){
        return -1;
    }
    int branch_flag = 0;
    int branch_itr = 0;
    char line[MAX_COVERAGE_LINE];
    int line_number = 0;
    int tot_branches = 0;
    int tot_branches_covered = 0;
    pb_info[branch_itr].num_branch = 0;
 

    while(fgets(line, MAX_COVERAGE_LINE, fp) != 0x0){
        if(strncmp(line, "branch", 6) == 0){

            branch_flag = 1;

            pb_info[branch_itr].line_num = line_number;

            char* tokens[4];
            tokens[0] = strtok(line, " ");
            
            for(int i = 0; i < 3; i++){            
                tokens[i + 1] = strtok(0x0, " ");
            }

            if(strncmp(tokens[2], "never", 5) == 0 ){
                pb_info[branch_itr].runs[pb_info[branch_itr].num_branch] = 0;
            } else { 
                // If branch at `line_number`, `branch_itr`-th branch was run at least once, mark it as 1. 
                // Else 0
                pb_info[branch_itr].runs[pb_info[branch_itr].num_branch] = (atoi(tokens[3]) > 0 ? 1 : 0);
            }
            pb_info[branch_itr].num_branch++;
            tot_branches++;
            tot_branches_covered += pb_info[branch_itr].runs[pb_info[branch_itr].num_branch];
        } else  {
            if(strncmp(line, "branch", 6) != 0 && strncmp(line, "call", 4) != 0 && strncmp(line, "function", 8) != 0){
                // line coverage
                char * covered = trim( strtok( line, ":" ));
                line_number = atoi( trim( strtok( 0x0, ":" )));
            }

            if(branch_flag == 1){
                branch_itr++;
            }
            branch_flag = 0;
            pb_info[branch_itr].num_branch = 0;
        }
    }
    pcov_info->tot_branches = tot_branches ;
    pcov_info->tot_branches_covered = tot_branches_covered;

    fclose(fp);
    return tot_branches;    
}

// Malloced. Need to be freed.
// gets filename from path.
char *
extract_filename(char *filepath)
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
    if(filename == 0x0){ // filepath = relative path, doesn't contain '/' in name
        filename = (char *) malloc(sizeof(char) * (len + 1));
        strcpy(filename, filepath);
    }

    return filename;
}

// Gets src_dirpath and iterate. Find all the files end with .c and add them to src_array
// Allocates memory to the names of the files
int
get_file_names(char *src_dirpath, char ** src_array)
{
    DIR * dir_ptr = 0x0;
    struct dirent * file = 0x0;

    if((dir_ptr = opendir(src_dirpath)) == 0x0){
        perror("Error in extract_filename");
        exit(1);
    }
    int i = 0;
    while((file = readdir(dir_ptr)) != 0x0){
        char * dot_c_ptr;
        // if file name contains .c 
        if((dot_c_ptr = strstr(file->d_name, ".c")) != 0x0){
            // and if file name contains it at the end of file, add it to the cov list
            if(file->d_name + (strlen(file->d_name) - 2) == dot_c_ptr){
                // TODO : How to make it secure?
                
                strncpy(src_array[i], file->d_name, NAME_MAX);
                
                i++;
            }
        }
    }
    
    closedir(dir_ptr);
    return i;
}


// Iteratre from end of the path. If `.` is found gets rid of it and 
// Is it needed?
int
remove_gcda(char *filepath)
{   
    char gcda_path[PATH_MAX];
    
    int len = strlen(filepath);
    // put .gcda after .c 
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
            //perror("Error in removing gcda");
            printf("couldn't remove %s \n", gcda_path);
        }
        free(gcda_path);
        return 0;
    }
}

int
gcov_multiple(char ** src_array, int num_files, char * src_dir_path, cov_info_t ** ppcov_info)
{
    int tot_branches = 0;
    char file_path[PATH_MAX];
    char abs_file_path[PATH_MAX];
    char abs_dir_path[PATH_MAX];


    for(int i = 0; i < num_files; i++) {
        sprintf(file_path, "%s/%s", src_dir_path, src_array[i]);
        
        
        if(realpath(file_path, abs_file_path) == 0x0){
            perror("Error in realpath in test_multi_source");
            exit(1);
        }

        if(realpath(src_dir_path, abs_dir_path) == 0x0){
            printf("fp : %s\n",abs_dir_path);
            perror("Error in realpath in test_multi_source");
            exit(1);
        }

        exec_gcov_with_bc_option(abs_file_path, abs_dir_path);
        
        // read coverage 
        int num_branches = read_gcov_coverage_with_bc_option(abs_file_path, ppcov_info[i]);
        
        if(num_branches == -1){
            continue;
        }
        tot_branches += num_branches;
    }
    return tot_branches;
}

// get coverage of already compiled and executed src files that are in one dir.
void
test_multi_source()
{
    char * src_dir_path = "target/bc-1.07.1/bc";

    char **src_array = (char **) malloc(sizeof(char*) * MAX_NUM_SRC);
    for(int i = 0 ; i < MAX_NUM_SRC ; i++){
        src_array[i] = (char*) malloc(sizeof(char) * NAME_MAX);
    }

    cov_info_t ** cov_info = (cov_info_t **)malloc(sizeof(cov_info_t*) * MAX_NUM_SRC);
    for(int i = 0 ; i < MAX_NUM_SRC ; i++){
        cov_info[i] = (cov_info_t *) malloc(sizeof(cov_info_t) * MAX_COVERAGE_LINE);
    }

    int num_files = get_file_names(src_dir_path, src_array);

    gcov_multiple(src_array, num_files, src_dir_path, cov_info);

   print_branch_coverage(cov_info, num_files);

    free_N((void **)src_array, MAX_NUM_SRC);
    free_N((void **)cov_info, MAX_NUM_SRC);

}


#ifdef DEBUG
int 
main()
{
    //execte_line_cov();

    // char filepath[] = "target/cgi_decode_ex.c";
    // char *args[] = {"Send+mail+to+me+fuzzingbook.org"}; 
    // char *gcpath = "./";
    // char * src_dirpath = "target";
    // int argc = 2;
    // b_info_t b_coverages[MAX_COVERAGE_LINE] = {};
    // execute_branch_cov(filepath, gpath, args, argc, b_coverages);
 
    // b_info_t ** b_coverages = (b_info_t **)malloc(sizeof(b_info_t*) * MAX_SRC_FILES);
    // for(int i = 0 ; i < MAX_SRC_FILES ; i++){
    //     b_coverages[i] = (b_info_t *) malloc(sizeof(b_info_t) * MAX_COVERAGE_LINE);
    // }
    // //b_info_t b_coverages[MAX_SRC_FILES][MAX_COVERAGE_LINE];
    // char * src_dir_path = "target/bc-1.07.1/bc";
    // char * bin_path = "target/bc-1.07.1/bc/bc";
    // char *args[] = {"seed_corpus"} ;
    // int argc = 2;
    // mult_src_cov(src_dir_path, bin_path, (b_info_t **)b_coverages, args, argc);

    test_multi_source();

    return 0;
}

#endif /* DEBUG */