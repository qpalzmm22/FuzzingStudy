#include "../include/fuzzer.h"
#include "../include/fuzz_input_maker.h"
#include "../include/gcov_creater.h"
#include "../include/coverage_calculator.h"


#define READEND 0
#define WRITEEND 1

// #define DEBUG

static pConfig_t g_config;
static result_t g_result;
static pid_t g_pid;
static int g_itr;

// TODO : template
char * 
create_tmp_dirs()
{
    // TODO :   
    // Change to path
	char template[64] = "tmp_XXXXXX";
    

	char * dir_name = (char *)malloc( 64 * sizeof(char));
	
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

void
get_seed(){
    // open directory
    // iterate and receive the seed by my_fread
}


int
my_fwrite(void * ptr, int n, FILE * stream)
{
	int sent = 0;

	while(sent < n){
		sent += fwrite(ptr + sent, 1, n - sent, stream);
	}

	return sent;
}

// fread() safe version
int
my_fread(void * ptr, int n, FILE * stream)
{
	int received = 0;

	while(received < n){
		received += fread(ptr + received , 1, n - received, stream);
	}

	return received;
}

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
                // is memcpy ok?

                g_config->prog_argv[g_config->prog_argc++] = str;
                g_config->prog_argv =  (char**)realloc(argv, (g_config->prog_argc + 1) * sizeof(char*)) ;
                assert(argv) ;
                argv[g_config->prog_argc] = 0x0 ;

            } else {
                fprintf(stderr, "Error on exec_mode");
                exit(1);
            }

            // write str by len amount

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
            // TODO : consider... is this right?
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

// g_config->prog_argv
// strcpy? can't touch prog_args...
void
make_argv()
{
    // Does not consider the case where argvs come as fuzzed input.
    char ** argv = (char **) calloc( 1, sizeof(char*));
    assert(argv);
    
    argv[0] = g_config->prog_path;
    int argc = 1;
    
    char * args = (char*)malloc((strlen(PROG_ARGS) + 1) * sizeof(char));
    assert(args);
    
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

// Returns 1 if return code is 0.
// TODO : How to make a generic oracle?? 
int
default_oracle(int exit_code, char* input, int input_len, char* stdout_buff, char* stderr_buff)
{
    if(exit_code != 0)
        return 0;
    else
        return 1;
}



// Sets and check the inputs, change so that it recieve pointer to config_t 
void
init_fuzzer(pConfig_t config)
{
    
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

    // in_config check
    if( MIN_LEN < 0 || MAX_LEN < 0 ){
        fprintf(stderr, "FUZZER Length must be 0 or greater\n");
        exit(1);    
    }
    if( CH_START < 0 || CH_RANGE < 0 ){
        fprintf(stderr, "CH_START, CH_RANGE must be 0 or greater\n");
        exit(1);    
    }

    // rand string generator
    g_config->in_configs.min_len = MIN_LEN ;
    g_config->in_configs.max_len = MAX_LEN ;
    g_config->in_configs.ch_start = CH_START ;
    g_config->in_configs.ch_range = CH_RANGE ;

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
        fprintf(stderr, "tmp_buf_size must be greater than 0") ;
        exit(1) ;
    }
    g_config->tmp_buf_size = TMP_MAX_SIZE;

    // Exec
    if(TRIALS < 0){
        fprintf(stderr, "TRIALS must be 0 or greater") ;
        exit(1) ;
    }
    g_config->trial = TRIALS ; 


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

    if(FUZZ_MODE != M_SRC && FUZZ_MODE != M_BIN){
        fprintf(stderr, "FUZZ_MODE must be set. 1 for src mode, 2 for bin mode\n");
        exit(1);
    }
    g_config->fuzz_mode = FUZZ_MODE; 

    char real_path[PATH_MAX] ;

    // executable path

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
            strcpy(g_config->src_path, real_path) ;
        } else {
            perror("Can't access the source code") ;
            exit(1);
        }
        char * src_wo_path = extract_program(g_config->src_path);
        g_config->src_wo_path = src_wo_path;

        if(COV_MODE != M_LINE && COV_MODE != M_BRANCH){
            fprintf(stderr, "COV_MODE must be set. 0 for line mode, 1 for branch mode\n");
            exit(1);
        }
        g_config->coverage_mode = COV_MODE;

        exec_gcc_coverage(g_config->src_path);

        char *p_bin_path;
        if((p_bin_path = realpath("a.out", 0x0)) == 0x0){
            perror("Error in real_path");
            exit(1);
        }

        strcpy(g_config->prog_path, p_bin_path);
        free(p_bin_path);


    }

    // --------------------- Initialize g_results----------------

    // It's useless since it's declared global?
    g_result.bugs = 0;
    g_result.tot_test_cases = 0;
    g_result.exec_time = 0;
    g_result.char_n = 0;
    
    g_result.tot_line_covered = 0;
    memset(g_result.cov_set, 0, MAX_COVERAGE_LINE);
    g_result.b_result = (b_result_t *) calloc(MAX_COVERAGE_LINE, sizeof(b_result_t));
}

void
calc_covered_lines()
{
    // O(lines)
    for(int i = 0; i < MAX_COVERAGE_LINE; i++){
        if(g_result.cov_set[i] > 0){
            g_result.tot_line_covered++;
        }
    }
}

// Assumes that gcov files do not return different branch structure.
void
union_branch_cov(b_result_t * p_result)
{
    for(int i = 0; i < g_result.tot_line_covered; i++){
        g_result.b_result[i].line_num = p_result[i].line_num;
        g_result.b_result[i].num_branch = p_result[i].num_branch; 
        for(int j = 0; j < p_result[i].num_branch; j++){
            g_result.b_result[i].runs[j] += p_result[i].runs[j];
        }
    }
}

void 
print_result()
{

    printf("\n\n");
    printf("===================================================== FUZZING RESULT ================================================\n");
    if(g_config->fuzz_mode == M_BIN){
        printf("=                  Program Path : %70s            =\n", g_config->prog_path);
    } else if(g_config->fuzz_mode == M_SRC){
        printf("=                   Source Path : %70s            =\n", g_config->src_path);
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
    
    if(g_config->fuzz_mode == M_SRC){
        printf("= ---------------------------------------------------- COVERAGE --------------------------------------------------- =\n");
        if(g_config->coverage_mode == M_LINE){
            calc_covered_lines();
            
            printf("=           Total lines covered : %70d            =\n", g_result.tot_line_covered);
            for(int i = 0; i < MAX_COVERAGE_LINE; i++){
                if(g_result.cov_set[i] > 0)
                    printf("=            covered line [ %2d ] : %70d            =\n", i, g_result.cov_set[i]);
            }
        } else if(g_config->coverage_mode == M_BRANCH){
            for(int i = 0; i < g_result.tot_line_covered; i++){
                for(int j = 0; j < g_result.b_result[i].num_branch; j++ ){
                    printf("=           Branch Line [ %2d ] ::   :: Branch [ %2d ] => %40d    %3.f %%           =\n", g_result.b_result[i].line_num, j, g_result.b_result[i].runs[j], (double )g_result.b_result[i].runs[j] / g_config->trial * 100);
                }
                printf("=                                                                                                                   =\n");
            } 
        }
    } 
    printf("=====================================================================================================================\n");
    printf("\n\n");
}

void
signal_handler(int sig)
{
    if(sig == SIGALRM){
        g_result.bugs++;
        printf("Program hanged !\nCheck %d-th files\n", g_itr);
        kill(g_pid, SIGKILL);
    }
    if(sig == SIGINT){ 
        print_result();
        exit(1);
    }
}

// Runs fuzz loop
void
fuzz_loop()
{
    if(g_config == 0x0){
        fprintf(stderr, "You must init first\n");
        exit(1);
    }

    srand(time(0x0));
    
    signal(SIGALRM, signal_handler);
    
    for(int i = 0; i < g_config->trial; i++){
        g_itr = i;
        // TODO size can change
        char out_buff[g_config->tmp_buf_size];
        char err_buff[g_config->tmp_buf_size];

        // crate_rand_str could change when using mutabion-based fuzzing
        char *rand_str;
        int len;

        if(g_config->rnd_str_gen_type == T_RSG){

            rand_str = (char *) malloc(sizeof(char) * (g_config->in_configs.max_len + 1));

            len = create_rand_str(g_config->in_configs, rand_str);
            

        } else if(g_config->rnd_str_gen_type == T_MUT){

            rand_str = (char *) malloc(sizeof(char) * (g_config->in_configs.max_len + 1 + g_config->trial));
            
            len = create_mut_str(g_config->trial, "hello World", 11, rand_str);
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
        g_result.exec_time +=(double) (end - start) / CLOCKS_PER_SEC; 

        if( !g_config->oracle(exit_code, rand_str, len, out_buff, err_buff)) {
            g_result.bugs++;  
            printf("Bug found!\nCheck %d-th files\n", i);
        }
        g_result.tot_test_cases++;

        memset(out_buff, 0, g_config->tmp_buf_size);
        memset(err_buff, 0, g_config->tmp_buf_size);

        if(g_config->fuzz_mode == M_SRC){
            if(g_config->coverage_mode == M_LINE){
                // make gcov file
                exec_gcov(g_config->src_wo_path);

                // read gcov file
                int cov_bit_map[MAX_COVERAGE_LINE] = {0};
                int n = read_gcov_coverage(g_config->src_wo_path, cov_bit_map);

                for(int i = 0 ; i < MAX_COVERAGE_LINE; i++){
                    g_result.cov_set[i] += cov_bit_map[i];
                }

                // remove gcov file
                if(remove_gcda(g_config->src_wo_path) != 0){
                    fprintf(stderr, "Error in src_path. Src_path must have dot in it\n");
                    exit(1);
                }
            } else if(g_config->coverage_mode == M_BRANCH){
                exec_gcov_with_bc_option(g_config->src_wo_path);

                b_result_t * p_result = (b_result_t *)calloc(MAX_COVERAGE_LINE, sizeof(b_result_t));
                g_result.tot_line_covered = read_gcov_coverage_with_bc_option(g_config->src_wo_path, p_result);
                
                union_branch_cov(p_result);
                free(p_result);
            }   
        }
        free(rand_str);
    }

    // Print result pretty
    print_result();

    free(g_result.b_result);

    free(g_config->src_wo_path);
    free(g_config->prog_argv[1]);
    free(g_config->prog_argv);
}

