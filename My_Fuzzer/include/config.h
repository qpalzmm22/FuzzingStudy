
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

// If DEFAULT_PATH is not defined, DATA_PATH should be defined
// #define DEFAULT_PATH
#define PROG_PATH "/home/qpalzmm22/FuzzingStudy/My_Fuzzer/target/cJSON/cjson_read_fuzzer"
#define SRC_DIR_PATH "/home/qpalzmm22/FuzzingStudy/My_Fuzzer/target/cJSON"
// #define SRC_PATH {"/home/qpalzmm22/FuzzingStudy/My_Fuzzer/target/bc-1.07.1/bc/main.c"}

// ============= PATH CONFIG $ ============= //

// ============= TERMINATION CONDITION ^ ============= //

#define MAX_TRIALS 1000
// in seconds
#define TIMEOUT 100

// ============= TERMINATION CONDITION ^ ============= //

// #define HANG_TIMEOUT 100

// ============= BUFFER CONFIG ^ ============= //

// #define TMP_MAX_SIZE 1024
// #define CHECK_N_BYTES 100

// ============= BUFFER CONFIG $ ============= //


// ============= COVERAGE CONFIG ^ ============= //

#define COV_MODE M_BRANCH

// ============= COVERAGE CONFIG $ ============= //

// ============= VIEW CONFIG ^ ============= //

#define PRINT_BRANCH 0 // 1 for true 0, for false

// ============= VIEW CONFIG $ ============= //

#define DEL_SEED 1 // Delete newly create seed?

/* -------------------------------------------------------------------------------------
         |                       | 
         |     DEFAULT VALUES    |
         V                       V
   ---------------------------------------------------------------------------------*/


// M_SRC : Fuzz src code
// M_BIN : Fuzz binary program
#ifndef FUZZ_MODE 
#define FUZZ_MODE M_COMPILED_BIN
#endif /* ! FUZZ_MODE */

#ifndef RSG_TYPE 
#define RSG_TYPE T_MUT
#endif /* ! RSG_TYPE */

#ifndef MIN_LEN 
#define MIN_LEN 10
#endif /* ! MIN_LEN */

#ifndef MAX_LEN 
#define MAX_LEN 100
#endif /* ! MAX_LEN */

#ifndef CH_START 
#define CH_START 32
#endif /* ! CH_START */

#ifndef CH_RANGE 
#define CH_RANGE 32
#endif /* ! CH_RANGE */

#ifndef MAX_MUTATION 
#define MAX_MUTATION 1
#endif /* ! MAX_MUTATION */

#ifndef PROG_PATH 
#define PROG_PATH "./target/a.out"
#endif /* ! PROG_PATH */

#ifndef SRC_DIR_PATH 
#define SRC_DIR_PATH "./target"
#endif /* ! SRC_DIR_PATH */

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

#ifndef COV_MODE 
#define COV_MODE M_BRANCH
#endif /* ! COV_MODE */

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

#ifndef PRINT_BRANCH 
#define PRINT_BRANCH 0
#endif /* ! PRINT_BRANCH */

#endif /* _HAVE_TYPES_H_ */