#include "../include/fuzzer.h"
#include "../include/fuzz_input_maker.h"
#include "../include/config.h"

#define READEND 0
#define WRITEEND 1

#define DEBUG


static pConfig_t g_config;

char 
*create_tmp_dirs()
{
	char template[64] = "tmp_XXXXXX";
    // TODO : 
    //char dir_path[PATH_MAX];
    //sprintf(dir_path, "%s/%s", g_config->out_path, template);
	//char *p_dir_name = mkdtemp(template);
	char * dir_name = malloc( 32 * sizeof(char));
	
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

ssize_t
my_fwrite(void * ptr, ssize_t n, FILE * stream)
{
	ssize_t sent = 0;

	while(sent < n){
		sent += fwrite(ptr + sent, n - sent, 1, stream);
	}

	return sent;
}

// fread() safe version
ssize_t
my_fread(void * ptr, ssize_t n, FILE * stream)
{
	ssize_t received = 0;

	while(received < n){
		received += fread(ptr + received , n - received, 1, stream);
	}

	return received;
}

void
make_input_files(char* str, int len, char *file_path)
{

    FILE *fp =  fopen(file_path, "wb");

    if(fp == 0){
        perror("ERROR in opening output file");
        exit(1);
    }
    if(my_fwrite(str, len, fp) != len){
        fprintf(stderr, "Error in my_writing input file\n");
    }
}

int
exec_process(char * str, int len, int itr)
{
    char *prog_name = g_config->prog_path;
    char *dir_path = g_config->out_path;

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

    switch(fork()) {
        case -1 :
            perror("Error in fork");
            exit(1);
            break;
        case 0 :  // child receives by stdin 
            if(g_config->exec_mode == M_STDIN){

                char in_file_path[PATH_MAX];
                sprintf(in_file_path, "%s%s%d", dir_path, "/inputs/input", itr);
                make_input_files(str, len, in_file_path);
                
                close(pipe_out[READEND]) ;
                close(pipe_err[READEND]) ;

                
                dup2(pipe_in[READEND], STDIN_FILENO) ;      
                close(pipe_in[READEND]) ;
                close(pipe_in[WRITEEND]) ;

                dup2(pipe_out[WRITEEND], STDOUT_FILENO) ;
                dup2(pipe_err[WRITEEND], STDERR_FILENO) ;

                execlp(prog_name, prog_name, NULL);
            }
            
            perror("Error in execlp");
            exit(1);
            break;
        default :
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

			char out_buff[1024];
			ssize_t s;

			while((s = read(pipe_out[READEND], out_buff, 1024)) > 0){
				if(fwrite(out_buff, 1, s, fp) < s){
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
			char err_buff[1024];

			while((s = read(pipe_err[READEND], err_buff, 1024)) > 0){
				if(fwrite(err_buff, 1, s, fp) < s){
					fprintf(stderr, "Error in making err files\n");
				}
			}
			
			fclose(fp);
			
			close(pipe_err[READEND]);

    }

}
// Sets and check the inputs
void
init_fuzzer()
{
    g_config = (pConfig_t)malloc(sizeof(config_t)) ;

    // Check user inputs

    //check_inputs();
    // in_config check
    if( MIN_LEN < 0 || MAX_LEN < 0 ){
        fprintf(stderr, "FUZZER Length must be 0 or greater than 0\n");
        exit(1);    
    }
    if( CH_START < 0 || CH_RANGE < 0 ){
        fprintf(stderr, "CH_START, CH_RANGE must be 0 or greater than 0\n");
        exit(1);    
    }
    // fuzzer
    g_config->in_configs.min_len = MIN_LEN ;
    g_config->in_configs.max_len = MAX_LEN ;
    g_config->in_configs.ch_start = CH_START ;
    g_config->in_configs.ch_range = CH_RANGE ;
 

    char real_path[PATH_MAX];
    // executable path
    if( realpath( PROG_PATH, real_path ) == NULL) {
        perror("Error on real path") ; 
        exit(1);
    } else if( access( real_path, F_OK ) == 0 ) {
        g_config->prog_path = real_path ;
    } else {
        perror("Can't access the file") ;
        exit(1);
    }
    // output path
    
    // Think...
    // g_config->in_path = IN_PATH ;
    // g_config->out_path = OUT_PATH ;

    // Check if every input 
    create_tmp_dirs();

    // command line g_config
    if(EXEC_MODE != M_STDIN && EXEC_MODE != M_ARG && EXEC_MODE != M_FILE){
        fprintf(stderr, "Exec mode must be between 0 ~ 2");
        exit(1);
    }
	g_config->exec_mode = EXEC_MODE;  // 0 = M_STDIN, 1 = ARG, 2 = M_FILe

    // Exec
    if(TRIALS < 0){
        fprintf(stderr, "TRIALS must be 0 or greater than 0");
        exit(1);
    }
    g_config->trial = TRIALS ; 


    if(TIMEOUT < 0){
        fprintf(stderr, "TIMEOUT must be 0 or greater than 0");
        exit(1);
    }
    g_config->timeout = TIMEOUT ; // timeout by seconds

    // consider hang if this amount time passes
    if(HANG_TIMEOUT < 0){
        fprintf(stderr, "HANG_TIMEOUT must be 0 or greater than 0");
        exit(1);
    }
    g_config->hang_timeout = HANG_TIMEOUT;

    // How to check if the function address' integrity
    g_config->oracle = ORACLE;


}

// Runs fuzz loop
void
fuzz_loop()
{
    if(g_config == 0x0){
        fprintf(stderr, "You must init first\n");
        exit(1);
    }  
    // Init
    // - g_config setting
    // - make tmp/input dir, output dir, error dir, bug dir
    srand(time(0x0));

    // Run 
    // - Timer start with interrupt 
    // - Make subprocesses and exec.
    // - Gets the results(STDOUT, STDIN)
    for(int i = 0; i < g_config->trial; i++){

        char *rand_str = (char *)malloc(sizeof(char) * (g_config->in_configs.max_len + 1));

        // crate_rand_str could change when using mutabion-based fuzzing
        int len = create_rand_str(g_config->in_configs, rand_str);
#ifdef DEBUG
        printf("rand array : %s\n", rand_str);
#endif
        exec_process(rand_str, len, i);
        free(rand_str);
    }

    // Oracle
    // - check STDOUT, STDERR, STDIN and return code to decide "what is bug". 
    // - Make file in bug dir.
    // g_config->oracle(); 
}

#ifdef DEBUG

int main()
{

    fuzz_loop();
}

#endif


