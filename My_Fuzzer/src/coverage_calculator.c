#include "../include/coverage_calculator.h"

// #define DEBUG

void
free_N(void ** pp_allocated, int num)
{
    for(int i = 0; i < num; i++){
        free(pp_allocated[i]);
    }
    free(pp_allocated);
}
// Trims out all the spaces that are starting and ending
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
print_branch_coverage(cov_info_t ** pp_cov_info, int num_files)
{
    printf("Covered branches :\n");
    for(int i = 0 ; i < num_files ; i++){
        
        printf("Total : %d, Covered: %d\n", pp_cov_info[i]->tot_branches, pp_cov_info[i]->tot_branches_covered);
    }
    printf("\n");
}


// Returns -1 when there is no file named `c_file.gcov` which is very possible.
int 
read_gcov_coverage_with_bc_option(char * c_file, cov_info_t * pcov_info)
{
    char *bmap = pcov_info->bmap;
    char target_file[PATH_MAX];
    
    sprintf(target_file, "%s.gcov", c_file);

    FILE * fp;
    if((fp = fopen(target_file, "r")) == NULL){
        return -1;
    }
    int branch_itr = 0;
    int tot_branches = 0;
    int tot_branches_covered = 0;
 	char line[MAX_LINE_IN_FILE];

    while(fgets(line, MAX_LINE_IN_FILE, fp) != 0x0){
        if(strncmp(line, "branch", 6) == 0){

            char* tokens[4];
            tokens[0] = strtok(line, " ");
            
            for(int i = 0; i < 3; i++){            
                tokens[i + 1] = strtok(0x0, " ");
            }

            if(strncmp(tokens[2], "never", 5) == 0 ){
               bmap[branch_itr] = 0; 
            } else { 
                int num = atoi(tokens[3]);
				bmap[branch_itr] = (num > 0 ? 1 : 0);
            }
            tot_branches++;
            tot_branches_covered += bmap[branch_itr++]; 
        } 
    }
    pcov_info->tot_branches = tot_branches ;
    pcov_info->tot_branches_covered = tot_branches_covered;

    fclose(fp);
    return tot_branches;    
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

/*
    Description : 
    Input is presumely filepath of c file that requires gcda to be removed. 
    This function will spot last "." from `filepath` and remove everything after it.
    Then it will add "gcda\0" at the end of the path. 

    Expectations :
    This function assumes that gcda files and src files are in the same directory.

    Return value :
    If it was successful, it will return 0.
    If it wasn't(couldn't find file, couldn't find "." from `filepath`, etc" ) -1.
*/
int
remove_gcda(char *filepath)
{   
    char gcda_path[PATH_MAX];

    strcpy(gcda_path, filepath);
    int len = strlen(gcda_path);

    for(int i = len - 1 ; i >= 0 ; i--){
        if(PATH_MAX > i + 5 && gcda_path[i] == '.'){

            memcpy( gcda_path + i + 1, "gcda", 4);
            gcda_path[i + 5] = 0x0;

            if(remove(gcda_path) != 0){
                return 0;
            } else {
                // couldn't find or remove the file.
                return 0;
            }
        }
    }
    return -1;
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
        cov_info[i] = (cov_info_t *) malloc(sizeof(cov_info_t));
    }

    int num_files = get_file_names(src_dir_path, src_array);

    gcov_multiple(src_array, num_files, src_dir_path, cov_info);

    print_branch_coverage(cov_info, num_files);

    free_N((void**)src_array, MAX_NUM_SRC);
    free_N((void**)cov_info, MAX_NUM_SRC);

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
