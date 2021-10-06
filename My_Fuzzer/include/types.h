#ifndef _HAVE_TYPES_H_
#define _HAVE_TYPES_H_

#define MAX_BRANCH 1024
#define MAX_LINE_IN_FILE 1024

#define MAX_SEED_FILES 1024
#define MAX_NUM_SRC 100

#define MAX_SEED_LEN 4096
#define INIT_SEED_ENERGY 1

#define MAX_ARGV 32

#define DEFAULT_MAKE_OUT 1 // Don't make
#define BLACKBOX 0 // 1 if you want to test blackbox, 0 if you want to test greybox

#include <linux/limits.h>

enum fz_mode{
    /* 0 */ M_SRC, // Fuzzing source file
    /* 1 */ M_BIN, // Fuzzing binary file (w/o coverage)
    /* 2 */ M_COMPILED_BIN, // Fuzzing binary file that is compiled with `--coverage` option
};

enum ex_mode{
    /* 0 */ M_STDIN, // Obtain fuzzed input from STDIN 
    /* 1 */ M_ARG, // Obtain fuzzed input from argument
    /* 2 */ M_FILE // Obtain fuzzed input from file
};

enum cov_mode{
    /* 0 */ M_LINE, // line coverage 
    /* 1 */ M_BRANCH, // branch coverage
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


// typedef struct _queue{
//     int size ;
//     int front ;
//     int rear ;
//     char queue[MAX_SEED_FILES][NAME_MAX] ;
// }queue_t;

typedef struct seed_t{
    char str[MAX_SEED_LEN];
    int len;
    double energy;
} seed_t;

typedef struct seed_info_t{
    int num_seed ;
    int init_num_seed ; 
    seed_t seeds[MAX_SEED_FILES] ; // weight inforamtion is stored
    double norm_energy[MAX_SEED_FILES] ;
}seed_info_t, *pSeed_info_t;


typedef struct _config{
    // fuzzer input config
    enum rsg_type rsg_type; 
    in_config_t in_configs;

    // // Seed schedular
    // queue_t seed_queue;
 
    // fuzz mode 0 :=> input : source path
    enum fz_mode fuzz_mode ; // 0 = Source code, 1 = Executable binary file
    char ** src_path ; // TODO CHANGE TO HEAP
    char * src_wo_path ;

    // fuzz mode 1 :=> input : binary path
    char prog_path[PATH_MAX] ;
    char** prog_argv ;
    char prog_argc;

    // fuzz mode 2 : => precompiled bin;
    // char prog_path[PATH_MAX] ;
    // char** prog_argv ;
    // char prog_argc ;

    // src, gcov, gcda, gcno path
    char src_dir_path[PATH_MAX] ;
    
    // d_ for system defining variables
    int d_num_src_files; 

    // input, output, error data path
    char data_path[PATH_MAX] ;

    // seed corpus path
    char seed_path[PATH_MAX] ;

    
    // path to write logs
    char exp_file_path[PATH_MAX];

    // command line config
	enum ex_mode exec_mode ;  // 0 = STDIN, 1 = ARG, 2 = M_FILe

    // Specifics
    int tmp_buf_size ;

    // Termination confition
    int max_trial ; 
    int timeout ;
    
    // Do Not Make outfile option
    int make_out ;

    // Coverage
    enum cov_mode coverage_mode;

    int hang_timeout ; // timeout by seconds

    int (*oracle)(int, char*, int, char*, char*) ;
} config_t, *pConfig_t ;

// typedef struct _branch_info{
//     int line_num; // line number of the branch containing line
//     int num_branch; // number of branches in one line
//     char runs[MAX_BRANCH]; // boolean of whether that branch was run
// } b_info_t, *pb_info;

typedef struct _coverage_info{
    char * file_name;
    int tot_branches;
    int tot_branches_covered;
    char bmap[MAX_BRANCH]; // info per branch 
} cov_info_t, *pcov_info_t;

typedef struct _result{
    int bugs ;
    int tot_test_cases ;
    double loop_time;
    double exec_time;
    int char_n ;
    int tot_branches;
    int tot_branches_covered;
    cov_info_t ** pp_union_cov;
}result_t ,*pResult_t;


#endif /* !_HAVE_TYPES_H_ */