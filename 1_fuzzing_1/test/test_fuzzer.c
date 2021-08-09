#include "../include/fuzzer.h"
#include "../include/runner.h"

#include <time.h>

#define PROG_NAME "cat"

void rand_fuzzer_run(char *prog_name)
{
    char * fuzzed_str = default_rand_fuzzer();
    run_process(prog_name, fuzzed_str);
    free(fuzzed_str);
}

void rand_fuzzer_runs(char *prog_name, int itr)
{
    for(int i = 0; i < itr; i++){
        rand_fuzzer_run(prog_name);
    }
}

void print_rand_fuzz(int itr){
    for(int i = 0; i < itr; i++){
        char * fuzzed_str = random_fuzzer(20, 20, 32, 32);
        printf("%s\n",fuzzed_str);
        free(fuzzed_str);
    }
}

int main()
{
    srand(time(0x0));

    // print_rand_fuzz(10);

    rand_fuzzer_run(PROG_NAME);

    rand_fuzzer_runs(PROG_NAME, 10);

    return 0;
}   