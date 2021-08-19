
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

#define MIN_LEN 0
#define MAX_LEN 3000
#define CH_START 0
#define CH_RANGE 255
#define PROG_PATH "psudo_troff/psudo_troff.out"
// #define PROG_ARGS ""

// If DEFAULT_PATH is not defined, DATA_PATH should be defined
// #define DEFAULT_PATH
#define DATA_PATH "./data"

// #define EXEC_MODE M_STDIN
// #define TMP_MAX_SIZE 1024


#define TRIALS 1000
// #define TIMEOUT 3
// #define HANG_TIMEOUT 100
// #define CHECK_N_BYTES 100

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

#ifndef PROG_ARGS 
#define PROG_ARGS ""
#endif /* ! PROG_ARGS */

#ifdef DEFAULT_PATH

#ifndef DATA_PATH 
#define DATA_PATH "./"
#endif /* ! DATA_PATH */

#endif /* ! DEFAULT_PATH */

#ifndef EXEC_MODE 
#define EXEC_MODE M_STDIN
#endif /* ! EXEC_MODE */

#ifndef TMP_MAX_SIZE 
#define TMP_MAX_SIZE 1024
#endif /* ! TMP_MAX_SIZE */

#ifndef TRIALS 
#define TRIALS 100
#endif /* ! TRIALS */

// Fuzzing timeout. Runs fuzzing for this amount of seconds. After, interrupt stops the program.
#ifndef TIMEOUT 
#define TIMEOUT 100
#endif /* ! TIMEOUT */

// Ends process after running HANG_TIMEOUT seconds. 
#ifndef HANG_TIMEOUT 
#define HANG_TIMEOUT 3
#endif /* ! HANG_TIMEOUT */

#ifndef CHECK_N_BYTES
#define CHECK_N_BYTES 100
#endif /* ! CHECK_N_BYTES */

#ifndef ORACLE 
#define ORACLE default_oracle
#endif /* ! ORACLE */

#endif /* _HAVE_TYPES_H_ */