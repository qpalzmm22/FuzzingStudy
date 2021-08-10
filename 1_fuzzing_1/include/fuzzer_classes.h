#include "runner.h"

typedef char * (*fp_fuzz) ();
typedef p_result (*fp_f_run) (void * , void*);
typedef p_result * (*fp_runs) (void * , void*, int);

typedef struct _Fuzzer{
    fp_fuzz f_fuzz;
    fp_f_run f_run;
    fp_runs f_runs;
}Fuzzer, *pFuzzer;

typedef struct _Random_Fuzzer{
    fp_fuzz f_fuzz;
    fp_f_run f_run;
    fp_runs f_runs;
    int min_length;
    int max_length;
    int char_start;
    int char_range;
}Random_Fuzzer, *pRandom_Fuzzer;


pFuzzer fuzzer_init();
char * fuzzer_fuzz();
p_result fuzzer_run(pFuzzer fuzzer, pRunner runner);
p_result * fuzzer_runs(pFuzzer fuzzer, pPrint_Runner print_runner, int trials);

pRandom_Fuzzer random_fuzzer_init();
//p_result random_fuzzer_run(pFuzzer fuzzer, pRunner runner);
//p_result * fuzzer_runs(pFuzzer fuzzer, pPrint_Runner print_runner, int trials);

char* random_fuzzer_fuzz(pRandom_Fuzzer random_fuzzer);

