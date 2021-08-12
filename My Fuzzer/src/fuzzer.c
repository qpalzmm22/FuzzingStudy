#include "../include/fuzzer.h"
#define DEBUG

pConfig_t config;

void init()
{

    // Check user inputs
    if(MIN_LEN < 0 || MAX_LEN < 0){
        fprintf(stderr, "FUZZER Length must be 0 or greater than 0\n");
        exit(1);    
    }



    config = (pConfig_t)malloc(sizeof(config_t)) ;

    // fuzzer
    config->min_len = MIN_LEN ;
    config->max_len = MAX_LEN ;
    config->ch_start = CH_START ;
    config->ch_range = CH_RANGE ;
 
    // binary path
    config->prog_path = PROG_PATH ;

    // output path
    config->in_path = IN_PATH ;
    config->out_path = OUT_PATH ;

    // command line config
	config->exec_mode = EXEC_MODE;  // 0 = STDIN, 1 = ARG, 2 = M_FILe

    // Exec
    config->trial = TRIALS ; 

    config->timeout = TIMEOUT ; // timeout by seconds

    // consider hang if this amount time passes
    config->hang_timeout = HANG_TIMEOUT;

    config->oracle = ORACLE;
}


int 
create_rand_str(char * dest)
{
    int min_len, max_len, char_start, char_range;

    config->min_len = min_len ;
    config->max_len = max_len ;
    config->ch_start = char_start ;
    config->ch_range = char_range ;
    
	unsigned int len ;	

	len = rand() % (max_len - min_len + 1) + min_len ; 

	for(int i = 0; i < len; i++){
		dest[i] = char_start + rand() % (char_range + 1) ;
	}
	dest[len] = '\0' ;

	return len ;
}


// Runs fuzz loop
void fuzz_loop(){
    
    // Init
    // - Config setting
    // - make tmp/input dir, output dir, error dir, bug dir
    init();

    // Run 
    // - Timer start with interrupt 
    // - Make subprocesses and exec.
    // - Gets the results(STDOUT, STDIN)
    for(int i = 0; i < config->trial; i++){
        char rand_str[config->max_len];
        create_rand_str(rand_str);
#ifdef DEBUG
        printf("rand array : %s\n",rand_str);
#endif
    }

    // Oracle
    // - check STDOUT, STDERR, STDIN and return code to decide "what is bug". 
    // - Make file in bug dir.
    //config->oracle(); 
}

#ifdef DEBUG

int main()
{
    init();
    
    printf("min : %d\n", config->min_len);
    printf("max : %d\n", config->max_len);

    fuzz_loop();
}

#endif


