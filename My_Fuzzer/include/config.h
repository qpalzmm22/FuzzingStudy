
#ifndef _HAVE_CONFIG_H_
#define _HAVE_CONFIG_H_

#include "types.h"

#include <linux/limits.h>
#include <unistd.h>

/* -------------------------------------------------------------------------------------
        |                         | 
        |       User Config       |
        V                         V
   ---------------------------------------------------------------------------------*/

//#define DEFAULT_FUZZER

#define EXEC_MODE M_FILE
#define FUZZ_MODE M_COMPILED_BIN
#define RSG_TYPE T_MUT


// ============= PATH CONFIG ^ ============= // 


#define MIN_LEN 10
#define MAX_LEN 500

#define CH_START 32
#define CH_RANGE 32

#define MAX_MUTATION 1

// #define PROG_ARGS ""


// ============= PATH CONFIG ^ ============= // 

// If DEFAULT_PATH is not defined, DATA_PATH should be defined
// #define DEFAULT_PATHa
#define PROG_PATH "/home/qpalzmm22/FuzzingStudy/My_Fuzzer/target/bc-1.07.1/bc/bc"
#define SRC_PATH "/home/qpalzmm22/FuzzingStudy/My_Fuzzer/target/bc-1.07.1/bc/main.c"
#define DATA_PATH "./data"
#define SEED_PATH "./seed_corpus"
#define GC_PATH "/home/qpalzmm22/FuzzingStudy/My_Fuzzer/target/bc-1.07.1/bc"


// ============= PATH CONFIG $ ============= //

#define MAX_TRIALS 1000
#define TIMEOUT 100

// #define HANG_TIMEOUT 100



// ============= BUFFER CONFIG ^ ============= //

// #define TMP_MAX_SIZE 1024
// #define CHECK_N_BYTES 100

// ============= BUFFER CONFIG $ ============= //



// ============= COVERAGE CONFIG ^ ============= //

#define COV_MODE M_BRANCH

// ============= COVERAGE CONFIG $ ============= //



/* -------------------------------------------------------------------------------------
         |                       | 
         |     DEFAULT VALUES    |
         V                       V
   ---------------------------------------------------------------------------------*/


// M_SRC : Fuzz src code
// M_BIN : Fuzz binary program
#ifndef FUZZ_MODE 
#define FUZZ_MODE M_SRC
#endif /* ! FUZZ_MODE */

#ifndef RSG_TYPE 
#define RSG_TYPE T_RSG
#endif /* ! RSG_TYPE */

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

#ifndef MAX_MUTATION 
#define MAX_MUTATION 10
#endif /* ! MAX_MUTATION */

#ifndef PROG_PATH 
#define PROG_PATH "./a.out"
#endif /* ! PROG_PATH */

#ifndef SRC_PATH 
#define SRC_PATH "./helloWorld.c"
#endif /* ! SRC_PATH */

#ifndef SEED_PATH 
#define SEED_PATH "./seed_corpus"
#endif /* ! SEED_PATH */

#ifndef PROG_ARGS 
#define PROG_ARGS ""
#endif /* ! PROG_ARGS */

#ifdef DEFAULT_PATH

#ifndef DATA_PATH 
#define DATA_PATH "./"
#endif /* ! DATA_PATH */

#endif /* ! DEFAULT_PATH */

// Mode of getting fuzzed string
// M_STDIN : Gets by stdin.
// M_ARG : Gets by args(options must be set so that the fuzzed args can go at the end of command).
// M_FILE : Gets by file.
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

#ifndef COV_MODE
#define COV_MODE M_LINE
#endif /* ! COV_MODE */

#ifndef ORACLE 
#define ORACLE default_oracle
#endif /* ! ORACLE */

#endif /* _HAVE_TYPES_H_ */