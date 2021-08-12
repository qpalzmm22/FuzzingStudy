#include <linux/limits.h>
#include <unistd.h>
#include "setting.h"

enum mode{
    /* 0 */ M_STDIN,
    /* 1 */ M_ARG,
    /* 2 */ M_FILE
};


#ifndef MIN_LEN 
#define MIN_LEN 20
#endif /* ! MIN_LEN */

#ifndef MAX_LEN 
#define MAX_LEN 20
#endif /* ! MAX_LEN */

#ifndef CH_START 
#define CH_START 32
#endif /* ! CH_START */

#ifndef CH_RANGE 
#define CH_RANGE 32
#endif /* ! CH_RANGE */

#ifndef PROG_PATH 
#define PROG_PATH "./"
#endif /* ! PROG_PATH */

#ifndef IN_PATH 
#define IN_PATH "./"
#endif /* ! IN_PATH */

#ifndef OUT_PATH 
#define OUT_PATH "./"
#endif /* ! OUT_PATH */

#ifndef EXEC_MODE 
#define EXEC_MODE M_STDIN
#endif /* ! EXEC_MODE */

#ifndef TRIALS 
#define TRIALS 100
#endif /* ! TRIALS */

// Fuzzing timeout. Runs fuzzing for this amount of seconds. After, interrupt stops the program.
#ifndef TIMEOUT 
#define TIMEOUT 100
#endif /* ! TIMEOUT */

#ifndef HANG_TIMEOUT 
#define HANG_TIMEOUT 100
#endif /* ! TIMEOUT */

#ifndef ORACLE 
#define ORACLE 0x0
#endif /* ! ORACLE */

typedef struct config{
    // fuzzer input config
    int min_len ;
    int max_len ;
    int ch_start ; 
    int ch_range ;
 
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