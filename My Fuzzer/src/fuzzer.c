#include "../include/fuzzer.h"
#include "../include/fuzz_input_maker.h"


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
	char template[64] = "tmp_XXXXXX";
    // TODO :   
    // Change to path

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
        perror("ERROR in opening input file");
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

    char *prog_name = g_config->prog_path;
    char ** argv = g_config->prog_argv;
    char *dir_path = g_config->data_path;

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

            char in_file_path[PATH_MAX];

            sprintf(in_file_path, "%s%s%d", dir_path, "/inputs/input", itr);
            make_input_files(str, len, in_file_path);

            if(g_config->exec_mode == M_STDIN){

                close(pipe_out[READEND]) ;
                close(pipe_err[READEND]) ;

                int sent = 0;
                while(sent < len){
                    sent = write(pipe_in[WRITEEND], str + sent, len - sent);
                    if(sent == -1){
                        perror("Error in write");
                        exit(1);    // CHECK
                    }
                    sent += sent;
                }
                // write str by len amount

                dup2(pipe_in[READEND], STDIN_FILENO) ;

                close(pipe_in[READEND]) ;
                close(pipe_in[WRITEEND]) ;

                dup2(pipe_out[WRITEEND], STDOUT_FILENO) ;
                dup2(pipe_err[WRITEEND], STDERR_FILENO) ;

                execv(prog_name, argv);
            }

            perror("Error in execlp");
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

			char out_tmp_buff[1024];
            // TODO : consider... is this right?
            int s;
            int read_bytes = 0;

			while((s = read(pipe_out[READEND], out_tmp_buff, 1024)) > 0){
                if( read_bytes < 1024 ){
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

			char err_tmp_buff[1024];
            
            read_bytes = 0;

			while((s = read(pipe_err[READEND], err_tmp_buff, 1024)) > 0){
                if( read_bytes < 1024 ){
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
    char * args = (char*)malloc((strlen(PROG_ARGS) + 1) * sizeof(char));
    assert(args);
    // TODO : Better function...
    strcpy(args, PROG_ARGS);    

    char ** argv = (char **) calloc( 1, sizeof(char*));
    assert(argv);
    
    argv[0] = g_config->prog_path;
    int argc = 1;

    char * arg = strtok(args, " ");
    while( arg != 0x0){                                                                                                                                                                                                                                                                                       
        argc++;
        argv = (char**)realloc(argv, argc * sizeof(char*));
        assert(argv);
        argv[argc - 1] = arg;

        arg = strtok(NULL, " ");
    }

    // adds null at the end of argv
    argv = (char**)realloc(argv, (argc + 1) * sizeof(char*)) ;
    assert(argv) ;
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

// Sets and check the inputs
void
init_fuzzer(int (*oracle)(int, char*, int, char*, char* ))
{
    g_config = (pConfig_t)malloc(sizeof(config_t)) ;

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

    char real_path[PATH_MAX] ;
    // executable pat
    if( realpath(PROG_PATH, real_path ) == NULL) {
        perror("Error on real path") ; 
        exit(1);
    } else if( access( real_path, X_OK ) == 0 ) {
        strcpy(g_config->prog_path, real_path) ;
    } else {
        perror("Can't access the file") ;
        exit(1);
    }

    // Gets argvs
    // g_config->args needs to be freed;
    make_argv() ;
    // Needs to be freed

    char * tmp = create_tmp_dirs() ;
    strcpy(g_config->data_path, tmp) ;
    free(tmp) ;
    
    // command line g_config
    if(EXEC_MODE != M_STDIN && EXEC_MODE != M_ARG && EXEC_MODE != M_FILE){
        fprintf(stderr, "Exec mode must be between 0 ~ 2") ;
        exit(1) ;
    }
	g_config->exec_mode = EXEC_MODE;  // 0 = M_STDIN, 1 = ARG, 2 = M_FILe

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
    // Q. Receive it through parameter?

    if(oracle != 0x0)
        g_config->oracle = oracle;
    else
        g_config->oracle = ORACLE;
    // --------------------- Initialize g_results----------------

    // It's useless since it's declared global?
    g_result.bugs = 0;
    g_result.tot_test_cases = 0;

}

void print_result()
{
    printf("================== FUZZING RESULT =========================================================================\n");
    printf("=        Program Path : %70s            =\n", g_config->prog_path);
    for(int i = 1; i < g_config->prog_argc; i++){
        printf("=         Prog arg[%d] : %70s            =\n", i, g_config->prog_argv[i]);
    }
    printf("=         Output Path : %70s            =\n", g_config->data_path);
    printf("=          Test Cases : %70d            =\n", g_result.tot_test_cases);
    printf("=          Bugs Found : %70d            =\n", g_result.bugs);
    printf("============================================================================================================\n");
}

void
alarm_handler(int sig)
{
    if(sig == SIGALRM){
        g_result.bugs++;
        printf("Program hanged !\nCheck %d-th files\n", g_itr);
        kill(g_pid, SIGKILL);
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
    
    signal(SIGALRM, alarm_handler);

    for(int i = 0; i < g_config->trial; i++){
        g_itr = i;
        // TODO size can change
        char out_buff[1024];
        char err_buff[1024];

        char *rand_str = (char *) malloc(sizeof(char) * (g_config->in_configs.max_len + 1));

        // crate_rand_str could change when using mutabion-based fuzzing
        int len = create_rand_str(g_config->in_configs, rand_str);
#ifdef DEBUG
        printf("rand array : %s\n", rand_str);
#endif
        alarm(g_config->timeout);

        int exit_code = exec_process(rand_str, len, i, out_buff, err_buff);
#ifdef DEBUG
        printf("ret code : %d\n", exit_code);
        printf("out_buff : %s\n", out_buff);
        printf("err_buff : %s\n", err_buff);
#endif
        if( !g_config->oracle(exit_code, rand_str, len, out_buff, err_buff)) {
            g_result.bugs++;  
            printf("Bug found!\nCheck %d-th files\n", i);
        }
        g_result.tot_test_cases++;

        memset(out_buff, 0, 1024);
        memset(err_buff, 0, 1024);

        free(rand_str);
    }

    // Print result pretty
    print_result();

    free(g_config->prog_argv[1]);
    free(g_config->prog_argv);
    free(g_config);
}

