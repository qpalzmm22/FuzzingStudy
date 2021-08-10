typedef struct _result{
    char * str;
    char * status;
}result, *p_result;

typedef p_result (*fp_run) (void * , char*);
typedef int (*fp_run_process) (void * , char*);

typedef struct _Runner{
    char pass[16];
    char fail[16];
    char unresolved[16];
    fp_run f_run;
}Runner, *pRunner;

typedef struct _Print_Runner{
    char pass[16];
    char fail[16];
    char unresolved[16];
    fp_run f_run;
}Print_Runner, *pPrint_Runner;

typedef struct _Program_Runner{
    char pass[16];
    char fail[16];
    char unresolved[16];
    fp_run f_run;
    char program[16];
    char * outputs;
    char * errors;
    int status;
    fp_run_process f_run_proc;
}Program_Runner, *pProgram_Runner;

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <assert.h>

#define READEND 0
#define WRITEEND 1

#define FAIL "FAIL"
#define PASS "PASS"
#define UNRESOLVED "UNRESOLVED"


pRunner runner_init();
p_result runner_run(pRunner runner, char * inp);

pPrint_Runner print_runner_init();
p_result print_Runner_run(pPrint_Runner runner,char * inp);

pProgram_Runner program_runner_init(char *prog_name);
p_result program_runner_run(pProgram_Runner runner, char * inp);

int pr_run_process(pProgram_Runner runner, char * arg);