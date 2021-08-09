typedef struct _result{
    char * input;
    char * status;
}result, *p_result;

typedef p_result (*fp_run) (void * , char*);
typedef int (*fp_run_process) (void * , char*);

typedef struct _Runner{
    char pass[16];
    char fail[16];
    char unresolved[16];
    fp_run func_p;
}Runner, *pRunner;

typedef struct _Print_Runner{
    char pass[16];
    char fail[16];
    char unresolved[16];
    fp_run func_p;
}Print_Runner, *pPrint_Runner;

typedef struct _Program_Runner{
    char pass[16];
    char fail[16];
    char unresolved[16];
    fp_run func_p;
    fp_run_process func_p2;
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


Runner runner_int();
p_result runner_run(char * inp);

pPrint_Runner print_runner_init();
p_result print_Runner_run(pPrint_Runner runner,char * inp);


int pr_run_process(char * prog_name, char * arg);