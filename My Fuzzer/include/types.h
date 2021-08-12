enum mode{
    /* 0 */ M_STDIN,
    /* 1 */ M_ARG,
    /* 2 */ M_FILE
};

typedef struct _in_config{
    int min_len ;
    int max_len ;
    int ch_start ; 
    int ch_range ;
}in_config_t;


typedef struct _config{
    // fuzzer input config
    in_config_t in_configs;
 
    // binary path
    char *prog_path ;

    // output path
    char *in_path ;
    char *out_path ;

    // command line config
	enum mode exec_mode ;  // 0 = STDIN, 1 = ARG, 2 = M_FILe

    // Exec
    int trial ; // default 10 ?
    int timeout ;

    int hang_timeout ; // timeout by seconds
    void* (*oracle)() ;
} config_t, *pConfig_t ;