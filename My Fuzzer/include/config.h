
#ifndef _HAVE_CONFIG_H_
#define _HAVE_CONFIG_H_

#include "types.h"

#include <linux/limits.h>
#include <unistd.h>

/* -------------------------------------------------------------------------------------
        |       User Config       | 
        V                         V
   ---------------------------------------------------------------------------------*/


//#define DEFAULT_FUZZER

#define MIN_LEN 20
#define MAX_LEN 30
#define CH_START 32
#define CH_RANGE 100
#define PROG_PATH "/bin/cat"
#define PROG_ARGS "-E -n"
// If DEFAULT_PATH is not defined, DATA_PATH should be defined
// #define DEFAULT_PATH
#define DATA_PATH "./data"

// #define EXEC_MODE M_STDIN

#define TRIALS 100
// #define TIMEOUT  
// #define HANG_TIMEOUT 100
// #define CHECK_N_BYTES 100
// #define ORACLE cat_oracle

/* -------------------------------------------------------------------------------------
       |    DEFAULT VALUES     | 
       V                       V
   ---------------------------------------------------------------------------------*/

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
#define PROG_PATH "./a.out"
#endif /* ! PROG_PATH */

#ifdef DEFAULT_PATH

#ifndef DATA_PATH 
#define DATA_PATH "./"
#endif /* ! DATA_PATH */

#endif /* ! DEFAULT_PATH */

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
#endif /* ! HANG_TIMEOUT */

#ifndef CHECK_N_BYTES
#define CHECK_N_BYTES 100
#endif /* ! CHECK_N_BYTES */

#ifndef ORACLE 
#define ORACLE default_oracle
#endif /* ! ORACLE */

#endif /* _HAVE_TYPES_H_ */