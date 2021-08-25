#ifndef _HAVE_TYPES_H_
#define _HAVE_TYPES_H_

#define MAX_COVERAGE_LINE 4096

#define MAX_SEED_FILES 1024

#include <linux/limits.h>
#include "coverage_calculator.h"

enum fz_mode{
    /* 0 */ M_SRC,
    /* 1 */ M_BIN,
};

enum ex_mode{
    /* 0 */ M_STDIN,
    /* 1 */ M_ARG,
    /* 2 */ M_FILE
};

enum cov_mode{
    /* 0 */ M_LINE,
    /* 1 */ M_BRANCH,
};

enum rsg_type{
    /* 0 */ T_RSG, // Random String Generator
    /* 1 */ T_MUT, // Mutation Based Generator
};

typedef struct _in_config{
    int min_len ;
    int max_len ;
    int ch_start ; 
    int ch_range ;
    int max_mutation ;
}in_config_t;

typedef struct _queue{
    int size ;
    int front ;
    int rear ;
    char queue[256][MAX_SEED_FILES] ;
}queue_t;


typedef struct _config{

    // fuzzer input config
    enum rsg_type rnd_str_gen_type; 
    in_config_t in_configs;

    // Seed schedular
    queue_t seed_queue;
 
    // fuzz mode M :=> input : source path
    enum fz_mode fuzz_mode ; // 0 = Source code, 1 = Executable binary file
    char src_path[PATH_MAX] ;
    char * src_wo_path ;

    // fuzz mode 2 :=> input : binary path
    char prog_path[PATH_MAX] ;
    char** prog_argv ;
    char prog_argc;

    // input, output, error data path
    char data_path[PATH_MAX] ;

    // seed corpus path
    char seed_path[PATH_MAX] ;

    // command line config
	enum ex_mode exec_mode ;  // 0 = STDIN, 1 = ARG, 2 = M_FILe

    // Specifics
    int tmp_buf_size ;

    // Exec
    int max_trial ; 
    int timeout ;
    
    // Coverage
    enum cov_mode coverage_mode;

    int hang_timeout ; // timeout by seconds
    int (*oracle)(int, char*, int, char*, char*) ;
} config_t, *pConfig_t ;

typedef struct _result{
    int bugs ;
    int tot_test_cases ;
    double exec_time;
    int char_n ;
    int tot_line_covered;
    int cov_set[MAX_COVERAGE_LINE];
    b_result_t *b_result;
}result_t ,*pResult_t;


#endif /* !_HAVE_TYPES_H_ */