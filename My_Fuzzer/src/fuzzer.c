#include "../include/fuzzer.h"

#define READEND 0
#define WRITEEND 1

// #define DEBUG

static pConfig_t g_config;
static result_t g_result;
static pid_t g_pid;
static int g_itr;
static seed_info_t g_seed_info;


/*
    Description : 
    Using 'mkdtemp', creates randomly named folder 
    and create folders named "inputs/", "outputs/", "errors/".

    Return value : 
    Retunrs randomly created name of the folder
*/
char * 
create_tmp_dirs()
{
    // TODO :   
    // Change to path
	char template[64] = "tmp_XXXXXX";

	char * dir_name = (char *)a_calloc( 64 * sizeof(char));
	
	strcpy(dir_name, mkdtemp(template));	

	char in_dir_path[32];
	char out_dir_path[32];
	char err_dir_path[32];

	// make dirs
	sprintf(in_dir_path, "%s%s", dir_name, "/inputs");
	sprintf(out_dir_path, "%s%s", dir_name, "/outputs");
	sprintf(err_dir_path, "%s%s", dir_name, "/errors");

    
	if(mkdir(in_dir_path, S_IRWXU) != 0 ){
		perror("ERROR in creating in directory");
        exit(1);
	} 
	if(mkdir(out_dir_path, S_IRWXU) != 0 ){
		perror("ERROR in creating out directory");
        exit(1);
	} 
	if(mkdir(err_dir_path, S_IRWXU) != 0 ){
		perror("ERROR in creating err directory");
        exit(1);
	}

    return dir_name;
}

/*
    Description : 
    Simple wrapper function that writes to `stream` for `n` bytes
    and store from `ptr` buffer.
    
    Return value : 
    Actual number of bytes wrote. It must match n.
*/
int
my_fwrite(void * ptr, int n, FILE * stream)
{
	int sent = 0;

	while(sent < n){
		sent += fwrite(ptr + sent, 1, n - sent, stream);
	}

	return sent;
}

/*
    Description : 
    Simple wrapper function that reads from `stream` for `n` bytes 
    and store it in `ptr`

    Return value :
    Actually received number of bytes. It must match n.
*/
int
my_fread(void * ptr, int n, FILE * stream)
{
	int received = 0;

	while(received < n){
		received += fread(ptr + received , 1, n - received, stream);
	}

	return received;
}

/*
    Description : 
    Simple wrapper function that does calloc, and then assert that it was valid.

    Return value : 
    pointer to allocated block.
*/
void *
a_calloc(size_t nsize)
{
    void * tmp = calloc(1, nsize);
    assert(tmp);
    return tmp;
}

/*
    Description : 
    make input files with string of `str` with length of `len` on `in_data_path` path.
    
    Return value :
    None 
*/
void
make_input_files(char* str, int len, char *in_data_path)
{
    FILE *fp =  fopen(in_data_path, "wb");

    if(fp == 0){
        perror("Error in opening input file");
        exit(1);
    }
    if(my_fwrite(str, len, fp) != len){
        fprintf(stderr, "Error in my_writing input file\n");
    }
    fclose(fp);
}

/*
    Description : 
    
    Return value : 
*/
int
exec_process(char * str, int len, int itr, char *out_buff, char *err_buff)
{
    char *prog_name = g_config->prog_path ;
    char ** argv = g_config->prog_argv ;
    char *dir_path = g_config->data_path ;
    int buf_size = g_config->tmp_buf_size ;

    int pipe_in[2];
    int pipe_out[2];
    int pipe_err[2];

    if(pipe(pipe_in) == -1){
        perror("Error in pipe_in");
    }
    if(pipe(pipe_out) == -1){
        perror("Error in pipe_out");
    }
    if(pipe(pipe_err) == -1){
        perror("Error in pipe_err");
    }

    switch(g_pid = fork()) {
        case -1 : ;
            perror("Error in fork");
            exit(1);
            break;
        case 0 : ; // child receives by stdin 
            close(pipe_out[READEND]) ;
            close(pipe_err[READEND]) ;

            char in_file_path[PATH_MAX];

            sprintf(in_file_path, "%s%s%d", dir_path, "/inputs/input", itr);
            make_input_files(str, len, in_file_path);

            if(g_config->exec_mode == M_STDIN){

                int sent = 0;
                while(sent < len){
                    sent = write(pipe_in[WRITEEND], str + sent, len - sent);
                    if(sent == -1){
                        perror("Error in write");
                        exit(1);    // CHECK
                    }
                    sent += sent;
                }                
            } // For file mode, make sure to locate file at the end of argvs
            else if(g_config->exec_mode == M_FILE){

                g_config->prog_argv[g_config->prog_argc++] = in_file_path;
                g_config->prog_argv =  (char**)realloc(argv, (g_config->prog_argc + 1) * sizeof(char*)) ;
                assert(argv) ;
                argv[g_config->prog_argc] = 0x0 ;
            } // For arg mode, make sure to locate fuzzed string at the end of argvs
            else if(g_config->exec_mode == M_ARG){
                
                g_config->prog_argv[g_config->prog_argc++] = str;
                g_config->prog_argv =  (char**)realloc(argv, (g_config->prog_argc + 1) * sizeof(char*)) ;
                assert(argv) ;
                argv[g_config->prog_argc] = 0x0 ;

            } else {
                fprintf(stderr, "Error on exec_mode");
                exit(1);
            }

            dup2(pipe_in[READEND], STDIN_FILENO) ;

            close(pipe_in[READEND]) ;
            close(pipe_in[WRITEEND]) ;

            dup2(pipe_out[WRITEEND], STDOUT_FILENO) ;
            dup2(pipe_err[WRITEEND], STDERR_FILENO) ;

            execv(prog_name, argv);
        
            perror("Error in execv");
            exit(1);
            break;
        default : ;
            close(pipe_in[WRITEEND]) ;
            close(pipe_in[READEND]) ;
            
            close(pipe_out[WRITEEND]) ;
            close(pipe_err[WRITEEND]) ;

            int ret_code;
            wait(&ret_code);

            // Write to a file
            char out_file_name[32]; // len = 19
			char err_file_name[32];
			sprintf(out_file_name, "%s%s%d", dir_path, "/outputs/output", itr);
			sprintf(err_file_name, "%s%s%d", dir_path, "/errors/error", itr);

			FILE *fp =  fopen(out_file_name, "wb");
			if(fp == 0){
				perror("ERROR in opening output file");
				exit(1);
			}

			char out_tmp_buff[buf_size];
            int s;
            int read_bytes = 0;

			while((s = read(pipe_out[READEND], out_tmp_buff, buf_size)) > 0){
                if( read_bytes < buf_size ){
                   memcpy(out_buff + read_bytes, out_tmp_buff + read_bytes, s); 
                }
                read_bytes += s;

				if(fwrite(out_tmp_buff, 1, s, fp) < s){
					fprintf(stderr, "Error in making out files\n");
				}
			}
			
			close(pipe_out[READEND]);
			fclose(fp);

			fp =  fopen(err_file_name, "wb");
			if(fp == 0){
				perror("ERROR in opening error file");
				exit(1);
			}

			char err_tmp_buff[buf_size];
            
            read_bytes = 0;

			while((s = read(pipe_err[READEND], err_tmp_buff, buf_size)) > 0){
                if( read_bytes < buf_size ){
                   memcpy(out_buff + read_bytes, out_tmp_buff + read_bytes, s); 
                }
                read_bytes += s;

				if(fwrite(err_tmp_buff, 1, s, fp) < s){
					fprintf(stderr, "Error in making err files\n");
				}
			}
			
			fclose(fp);
			close(pipe_err[READEND]);
            return ret_code;
    }

}

/*
    Description :
    Adds seed to seed list with given `weight`
    
    Return value : 
    None
*/
void
add_seed(int weight)
{
    g_seed_info.weight[g_seed_info.num_seed++] = weight;
}

/*
    Description : 
    Gets index of the seed by random.

    Expectation : 

    Return value : 
    Random index of seeds.
*/
int
get_seed_ind()
{
    return rand() % (g_seed_info.num_seed);
}

/*
    Description : 
    Search files with name starting with "seed_" and add them to seed list. 
    Here, the `g_seed_info.weight` contains weight of the seed list, and the `g_seed_info.num_seed` acts as index iterator 

    Expectation : 
    Users must make seed files with name 'seed_n' where n starts from 0.
    And there must not be a jump in number, or else fuzer will overwrite the existing seed.

    Return value : 
    None
*/
void
seed_init()
{
    char seed_file[PATH_MAX + NAME_MAX]; 

    while(1){
        sprintf(seed_file,"%s/%s%d", g_config->seed_path, "seed_", g_seed_info.num_seed);
        
        if( access( seed_file, R_OK ) != 0 ) {
            if(g_seed_info.num_seed == 0){
                perror("You must put at least one seed in seed_corpus dir");
                exit(1);
            }
            break;
        }
        add_seed(1);     
    }
}

/*
    Description : 
    Create file with name of "seed_n" where n is `seed_ind` 
    and content of `str` with length of `len`.

    Expectation : 
    
    Return value : 
    None
*/
void
make_seed_file(char * str, int len, int seed_ind)
{
    char seed_path[PATH_MAX + NAME_MAX];
    sprintf(seed_path,"%s/seed_%d",g_config->seed_path, seed_ind);

    FILE * fp;

    if((fp = fopen(seed_path, "w")) == NULL){
        perror("Error in opening seed file");
        exit(1);
    }

    if(my_fwrite(str, len, fp) != len){
        fprintf(stderr, "Error in my_fwriting seed file\n");
    }

    fclose(fp);
}

/*
    Description : 
    Search files with name of "seed_n" where n is `seed_ind` and return its content to `buf`. 

    Expectation : 
    The buf must be big enough to hold contents from seed file.

    Return value : 
    Length of the returned seed.
*/
int 
read_seed_file(char* buf, int seed_ind)
{
    char seed_path[PATH_MAX + NAME_MAX];
    
    sprintf(seed_path, "%s/seed_%d",g_config->seed_path, seed_ind);
    FILE * fp;

    if((fp = fopen(seed_path, "r")) == NULL){
        perror("Error in opening seed file");
        exit(1);
    }

    int received = 0;
    int tmp;

    while((tmp = fread(buf, 1, g_config->in_configs.max_len + 1 + g_config->in_configs.max_mutation, fp)) != 0){
        received += tmp;
    }
    buf[received] = 0x0;

    fclose(fp);

    return received;
}

/*
    Description : 
    Search files with name of "seed_n" where n is `seed_ind` and return its content to buf. 

    Expectation : 
    The buf must be big enough to hold contents from seed file.

    Return value : 
    Length of the returned seed.
*/
void
make_argv()
{
    // Does not consider the case where argvs come as fuzzed input.
    char ** argv = (char **) a_calloc( sizeof(char*));
    
    argv[0] = g_config->prog_path;
    int argc = 1;
    
    char * args = (char*) a_calloc((strlen(PROG_ARGS) + 1) * sizeof(char));
    
    // TODO : Better function...?
    strncpy(args, PROG_ARGS, strlen(PROG_ARGS) + 1);    

    char * arg = strtok(args, " ");
    while( arg != 0x0){                                                                                                                                                                                                                                                                                   
        argc++;
        argv = (char**)realloc(argv, (argc + 1) * sizeof(char*));
        assert(argv);
        argv[argc - 1] = arg;

        arg = strtok(NULL, " ");
    }

    // adds null at the end of argv
    argv[argc] = 0x0 ;

    g_config->prog_argv = argv ;
    g_config->prog_argc = argc ;
}   

/*
    Description : 
    Fuzzer will automactically use this oracle if no oracle was given by user.
    This oracle examines return code and consider it as bug if the return code was not 0
    
    Return value : 
    0 for no bug, 1 for bug.
*/
int
default_oracle(int exit_code, char* input, int input_len, char* stdout_buff, char* stderr_buff)
{
    if(exit_code != 0)
        return 0;
    else
        return 1;
}



/*
    Description : 
    This function gets configuration setting from user.
    The input `config` is set. If user has options missing, Fuzzer uses values in config.h
    which is set of default setting. (Not implemented)

    Return value : 
    None. Global variable, g_config is set.
*/
void
init_fuzzer(pConfig_t config)
{
    srand(time(NULL));

    char real_path[PATH_MAX] ;

    if(config == 0x0){
        fprintf(stderr, "You must pass valid config\n");
        exit(1);
    }
    g_config = config;


    if (RSG_TYPE != T_RSG && RSG_TYPE != T_MUT){
        fprintf(stderr, "RSG_TYPE must be between 0 ~ 1. 0 for randomly generated strings, 1 for Mutant based strings\n") ;
        exit(1) ;
    }
    g_config->rnd_str_gen_type = RSG_TYPE ;
    
    if(g_config->rnd_str_gen_type == T_MUT){
        if( realpath(SEED_PATH, real_path ) == NULL) {
            perror("Error on finding seed directory") ; 
            exit(1);
        } else if( access( real_path, R_OK ) == 0 ) {
            strcpy(g_config->seed_path, real_path) ;
        } else {
            perror("Can't access the seed directory") ;
            exit(1);
        }

        g_seed_info.num_seed = 0;
        seed_init();
    }


    // in_config check
    if( MIN_LEN < 0 || MAX_LEN < 0 ){
        fprintf(stderr, "FUZZER Length must be 0 or greater\n");
        exit(1);    
    }
    if( CH_START < 0 || CH_RANGE < 0 ){
        fprintf(stderr, "CH_START, CH_RANGE must be 0 or greater\n");
        exit(1);    
    }
    if(MAX_MUTATION < 0){
        fprintf(stderr, "MAX_MUTATION must be 0 or greater\n");
        exit(1);   
    }

    // random && mutation based string generator
    g_config->in_configs.min_len = MIN_LEN ;
    g_config->in_configs.max_len = MAX_LEN ;
    g_config->in_configs.ch_start = CH_START ;
    g_config->in_configs.ch_range = CH_RANGE ;

    g_config->in_configs.max_mutation = MAX_MUTATION;


    // Gets argvs
    // g_config->args needs to be freed;
    make_argv() ;

    char * tmp = create_tmp_dirs() ;
    strcpy(g_config->data_path, tmp) ;
    free(tmp) ;
    
    // command line g_config
    if(EXEC_MODE != M_STDIN && EXEC_MODE != M_ARG && EXEC_MODE != M_FILE){
        fprintf(stderr, "EXEC_MODE must be between 0 ~ 2") ;
        exit(1) ;
    }
	g_config->exec_mode = EXEC_MODE;  // 0 = M_STDIN, 1 = ARG, 2 = M_FILe

    // Pipe buffer size
    if(TMP_MAX_SIZE <= 0 ){
        fprintf(stderr, "TMP_MAX_SIZE must be greater than 0") ;
        exit(1) ;
    }
    g_config->tmp_buf_size = TMP_MAX_SIZE;

    // Exec
    if(MAX_TRIALS < 0){
        fprintf(stderr, "TRIALS must be 0 or greater") ;
        exit(1) ;
    }
    g_config->max_trial = MAX_TRIALS ; 


    if(TIMEOUT < 0){
        fprintf(stderr, "TIMEOUT must be 0 or greater than 0");
        exit(1);
    }
    g_config->timeout = TIMEOUT ; // timeout by seconds


#ifdef HANG_TIMEOUT

    // consider hang if this amount time passes
    if(HANG_TIMEOUT < 0){
        fprintf(stderr, "HANG_TIMEOUT must be 0 or greater than 0");
        exit(1);
    }
    g_config->hang_timeout = HANG_TIMEOUT;

#endif /* HANG_TIMEOUT */ 

    // Q. How to check the integrity of function address?
    if(g_config->oracle == 0x0)
        g_config->oracle = ORACLE;

    // --------------------- Other settings --------------------

    signal(SIGINT, signal_handler);

    // ---------------------- Coverage -------------------------

    if(FUZZ_MODE != M_SRC && FUZZ_MODE != M_BIN && FUZZ_MODE != M_COMPILED_BIN){
        fprintf(stderr, "FUZZ_MODE must be set. 0 for src mode, 1 for bin mode, 2 for precompiled mode(with --coverage)\n");
        exit(1);
    }
    g_config->fuzz_mode = FUZZ_MODE; 

    // --------------------- source -------------------------//

    g_config->src_path = (char **) a_calloc(sizeof(char*) * MAX_NUM_SRC);
    for(int i = 0 ; i < MAX_NUM_SRC ; i++){
        g_config->src_path[i] = (char*) a_calloc(sizeof(char) * NAME_MAX);
    }

    if(g_config->fuzz_mode == M_BIN){
        if( realpath(PROG_PATH, real_path ) == NULL) {
            perror("Error on finding program") ; 
            exit(1);
        } else if( access( real_path, X_OK ) == 0 ) {
            strcpy(g_config->prog_path, real_path) ;
        } else {
            perror("Can't access the program") ;
            exit(1);
        }   
    } else if(g_config->fuzz_mode == M_SRC){

        if( realpath(SRC_PATH, real_path ) == NULL) {
            perror("Error on finding source code") ; 
            exit(1);
        } else if( access( real_path, R_OK ) == 0 ) {
            strcpy(g_config->src_path[0], real_path) ;
        } else {
            perror("Can't access the source code") ;
            exit(1);
        } 

        if(COV_MODE != M_LINE && COV_MODE != M_BRANCH){
            fprintf(stderr, "COV_MODE must be set. 0 for line mode, 1 for branch mode\n");
            exit(1);
        }
        g_config->coverage_mode = COV_MODE;

        char * tmp = get_bin_path(g_config->src_path[0]);
        if(tmp == 0x0){
            fprintf(stderr, "Error in getting bin path of the source code");
            exit(1);
        }
        strcpy(g_config->prog_path, tmp) ;
        free(tmp);

        exec_gcc_coverage(g_config->src_path[0], g_config->prog_path);

    } else if(g_config->fuzz_mode == M_COMPILED_BIN){
        if( realpath(PROG_PATH, real_path ) == NULL) {
            perror("Error on finding program") ; 
            exit(1);
        } else if( access( real_path, X_OK ) == 0 ) {
            strcpy(g_config->prog_path, real_path) ;
        } else {
            perror("Can't access the program") ;
            exit(1);
        }

        if(SRC_PATH == 0x0){
            if( realpath(SRC_DIR_PATH, real_path ) == NULL) {
                perror("Error on finding program") ; 
                exit(1);
            } else if( access( real_path, R_OK ) == 0 ) {
                strcpy(g_config->src_dir_path, real_path) ;
            } else {
                perror("Can't access the program") ;
                exit(1);
            }
            g_config->d_num_src_files = get_file_names(g_config->src_dir_path, g_config->src_path);
        } else {
            g_config->d_num_src_files = 1;
            if( realpath(SRC_PATH, real_path ) == NULL) {
                perror("Error on finding source code") ; 
                exit(1);
            } else if( access( real_path, R_OK ) == 0 ) {
                strcpy(g_config->src_path[0], real_path) ;
            } else {
                perror("Can't access the source code") ;
                exit(1);
            }
        }
        // TODO : Change to array

        if(COV_MODE != M_LINE && COV_MODE != M_BRANCH){
            fprintf(stderr, "COV_MODE must be set. 0 for line mode, 1 for branch mode\n");
            exit(1);
        }
        g_config->coverage_mode = COV_MODE;
    } 

    // --------------------- 
    
    // if src_file_path exists, it's sigle file.
    //  Else it's multiple file


    // --------------------- Initialize g_results----------------


    // It's useless since it's declared global?
    g_result.bugs = 0;
    g_result.tot_test_cases = 0;
    g_result.exec_time = 0;
    g_result.char_n = 0;

    g_result.tot_branches = 0; 
    g_result.tot_branches_covered = 0;
    

    g_result.cov_set = (int **) a_calloc(g_config->d_num_src_files * sizeof(int*));

    for(int i = 0; i < MAX_NUM_SRC; i++){
        g_result.cov_set[i] = (int *) a_calloc(MAX_COVERAGE_LINE * sizeof(int));
    }

    //memset(g_result.cov_set, 0, MAX_COVERAGE_LINE);
    g_result.pp_union_cov = (cov_info_t **) a_calloc(MAX_NUM_SRC * sizeof(cov_info_t*));

    for(int i = 0; i < MAX_NUM_SRC; i++){
        g_result.pp_union_cov[i] = (cov_info_t *) a_calloc(MAX_COVERAGE_LINE * sizeof(cov_info_t));
    }

}
/*
    Description : 
    Union coverage information for g_result.pp_union_cov with input, `pp_cov_info`
    While at it, it also continuously copies the other infomation 
    like number of brances or number of bracnches covered

    Expectation : 

    Return value : 
    Return 1 if union has made change to original set
    Return 0 if not.
*/
int
union_branch_cov(cov_info_t ** pp_cov_info)
{
    g_result.tot_branches = 0;
    g_result.tot_branches_covered = 0;
    int inc_flag = 0;
    for(int i = 0 ; i < g_config->d_num_src_files ; i++){
        
        for(int j = 0 ; j < pp_cov_info[i]->tot_branches; j++ ){
            // TODO : Unecessary repetitions. Please resolve.

            g_result.pp_union_cov[i]->b_infos[j].line_num = pp_cov_info[i]->b_infos[j].line_num;
            g_result.pp_union_cov[i]->b_infos[j].num_branch = pp_cov_info[i]->b_infos[j].num_branch;
            

            for(int k = 0 ; k < pp_cov_info[i]->b_infos[j].num_branch; k++){
                if(g_result.pp_union_cov[i]->b_infos[j].runs[k] == 0 && pp_cov_info[i]->b_infos[j].runs[k] != 0){
                    g_result.pp_union_cov[i]->tot_branches_covered++;
                    g_result.pp_union_cov[i]->b_infos[j].runs[k] += pp_cov_info[i]->b_infos[j].runs[k];
                    inc_flag = 1;
                }
            }
            
        }
        g_result.pp_union_cov[i]->tot_branches = pp_cov_info[i]->tot_branches;
        g_result.tot_branches += pp_cov_info[i]->tot_branches;
        g_result.tot_branches_covered += g_result.pp_union_cov[i]->tot_branches_covered;
    }
    return inc_flag;
}


/*
    Description : 
    Print result of fuzzing

    Expectation : 

    Return value : 
    None

*/
void
make_log_result()
{

}


/*
    Description : 
    Print result of fuzzing

    Expectation : 

    Return value : 
    None

*/
void 
print_result()
{

    printf("\n\n");
    printf("===================================================== FUZZING RESULT ================================================\n");
    if(g_config->fuzz_mode == M_BIN || g_config->fuzz_mode == M_COMPILED_BIN){
        printf("=                  Program Path : %70s            =\n", g_config->prog_path);
    } else if(g_config->d_num_src_files == 1){
        printf("=                   Source Path : %70s            =\n", g_config->src_path[0]);
    } else {
        printf("=                    Source Dir : %70s            =\n", g_config->src_dir_path);
    }
    for(int i = 1; i < g_config->prog_argc; i++){
        printf("=                  Prog arg[%d] : %70s            =\n", i, g_config->prog_argv[i]);
    }
    printf("=                   Output Path : %70s            =\n", g_config->data_path);
    printf("=                    Test Cases : %70d            =\n", g_result.tot_test_cases);
    printf("=  Total Exec. Time(in seconds) : %70.3f            =\n", g_result.exec_time);
    printf("=  Avg. Exec. Time(in mseconds) : %70.3f            =\n", g_result.exec_time / g_result.tot_test_cases * 1000);
    printf("= ---------------------------------------------------- BUG STATS -------------------------------------------------- =\n");
    printf("=                    Bugs Found : %70d            =\n", g_result.bugs);
    printf("=            Bugs per testcases : %70.3f            =\n", ((double)g_result.bugs) / g_result.tot_test_cases);
    printf("=      Bugs per number of chars : %70.5f            =\n", ((double)g_result.bugs) / g_result.char_n);
    
    if(g_config->fuzz_mode == M_SRC || g_config->fuzz_mode == M_COMPILED_BIN){
        printf("= ---------------------------------------------------- COVERAGE --------------------------------------------------- =\n");
        printf("=               Branch Coverage : %70.3f %%          =\n", ((double)g_result.tot_branches_covered) * 100 / g_result.tot_branches);
        printf("=                                                                                                                   =\n");
        for(int i = 0 ; i < g_config->d_num_src_files; i++){
            printf("=                      File [%d] : %70s            =\n", i, g_config->src_path[i]);
            printf("=                         +---- : %70.5f %%          =\n", ((double) g_result.pp_union_cov[i]->tot_branches_covered) * 100 / g_result.pp_union_cov[i]->tot_branches);
            printf("=                                                                                                                   =\n");
            if(PRINT_BRANCH){
                for(int j = 0; j < g_result.pp_union_cov[i]->tot_branches; j++){
                    b_info_t branch_info = g_result.pp_union_cov[i]->b_infos[j];
                    for(int k = 0; k < branch_info.num_branch; k++){
                        printf("=   Branch Line [ %10d ] ::   :: Branch [ %2d ] => %40d    %3.f %%           =\n", branch_info.line_num, k, branch_info.runs[k], (double)branch_info.runs[k] / g_result.tot_test_cases);
                    }
                    printf("=                                                                                                                   =\n");
                } 
            }
        } 
    } 
    printf("=====================================================================================================================\n");
    printf("\n\n");
}

void
exit_protocol()
{
    print_result();

    for(int i = 0 ; i < g_config->d_num_src_files; i++){
        free(g_result.pp_union_cov[i]);
    }
    free(g_result.pp_union_cov);

    free(g_config->prog_argv[1]);
    free(g_config->prog_argv);
}

void
signal_handler(int sig)
{
    if(sig == SIGALRM){
        g_result.bugs++;
        printf(" *** Program hanged ! *** \n *** Check %d-th files *** \n", g_itr);
        kill(g_pid, SIGKILL);
    }
    if(sig == SIGINT){ 
        print_result();
        // Do we need to free? 
        exit(1);
    }
}

/*
    Description :
    1. Runs fuzz loop.
    TODO

    Return value : none
*/
void
fuzz_loop()
{
    if(g_config == 0x0){
        fprintf(stderr, "You must init first\n");
        exit(1);
    }
    
    signal(SIGALRM, signal_handler);
    
    int i = 0;
    while(1){
        // Termination Condition
        // This is not exactly how much time we would like fuzzer to run.
        // This is timeout on total run_time of testing program.
        if(g_result.exec_time >= g_config->timeout || i == g_config->max_trial ){
            
            exit_protocol();
            exit(0);
        }
        g_itr = i++;
        // TODO size can change
        char out_buff[g_config->tmp_buf_size];
        char err_buff[g_config->tmp_buf_size];

        // crate_rand_str could change when using mutabion-based fuzzing
        char *rand_str;
        int len;
        int seed_ind;

        if(g_config->rnd_str_gen_type == T_RSG){

            rand_str = (char *) a_calloc(sizeof(char) * (g_config->in_configs.max_len + 1));

            len = create_rand_str(g_config->in_configs, rand_str);

        } else if(g_config->rnd_str_gen_type == T_MUT){

            // TODO : How should I allocate sufficient amount of memory for random string input
            rand_str = (char *) a_calloc(sizeof(char) * (g_config->in_configs.max_len + 1 + g_config->in_configs.max_mutation * 4));
            
            seed_ind = get_seed_ind();
            len = read_seed_file(rand_str, seed_ind);
#ifdef DEBUG
            printf("seed array : %s\n", rand_str);
#endif
            len = create_mut_str(g_config->in_configs.max_mutation, rand_str, len, rand_str);
        }
        
        g_result.char_n += len;
#ifdef DEBUG
        printf("rand array : %s\n", rand_str);
#endif

        alarm(g_config->hang_timeout);
        clock_t start = clock();
        
        int exit_code = exec_process(rand_str, len, i, out_buff, err_buff);

#ifdef DEBUG
        printf("ret code : %d\n", exit_code);
        printf("out_buff : %s\n", out_buff);
        printf("err_buff : %s\n", err_buff);
#endif

        clock_t end = clock();
        g_result.exec_time += (double) (end - start) / CLOCKS_PER_SEC; 

        if( !g_config->oracle(exit_code, rand_str, len, out_buff, err_buff)) {
            g_result.bugs++;  
            printf(" *** Bug found! *** \n *** Check %d-th files *** \n", i);
        }
        g_result.tot_test_cases++;

        memset(out_buff, 0, g_config->tmp_buf_size);
        memset(err_buff, 0, g_config->tmp_buf_size);

       
        cov_info_t ** cov_info = (cov_info_t **)a_calloc(sizeof(cov_info_t*) * MAX_NUM_SRC);
        for(int i = 0 ; i < MAX_NUM_SRC ; i++){
            cov_info[i] = (cov_info_t *) a_calloc(sizeof(cov_info_t) * MAX_COVERAGE_LINE);
        }

        gcov_multiple(g_config->src_path, g_config->d_num_src_files, g_config->src_dir_path, cov_info); 
        
        // Something is added to branch coverage
        if(union_branch_cov(cov_info) > 0){
            char new_seed_file[NAME_MAX];
            
            // make seed_file
            printf(" *** add %s to seed *** \n", rand_str);
            make_seed_file(rand_str, len, g_seed_info.num_seed);
            add_seed(1);
        }

        free_N((void**)cov_info, MAX_NUM_SRC);
    
        char abs_file_path[PATH_MAX+3];
        // remove gcov file
        for(int i = 0; i < g_config->d_num_src_files; i++){
            sprintf(abs_file_path, "%s/%s", g_config->src_dir_path, g_config->src_path[i]);
            // printf();
            if(remove_gcda(abs_file_path) != 0){
                fprintf(stderr, "Error in src_path.\n");
                exit(1);
            }
        }

        free(rand_str);
        // print_result();
    }

    // Print result pretty
    exit_protocol();
}