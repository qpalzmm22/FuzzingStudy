#include "../include/fuzzer_classes.h"
#include <time.h>

// TODO : FREE...
void test_fuzzer_class()
{
    pFuzzer fuzzer = fuzzer_init();
    pPrint_Runner print_runner = print_runner_init();
    p_result * results = fuzzer->f_runs(fuzzer, print_runner, 10);
    free(fuzzer);
    free(print_runner);
    for(int i = 0; i < 10 ; i++){
        free(results[i]);
    }
}

// TODO : FREE...
void test_rand_fuzzer_class()
{
    
    pRandom_Fuzzer rand_fuzzer = random_fuzzer_init();
    rand_fuzzer->min_length = 20;
    rand_fuzzer->max_length = 20;
    
    for(int i = 0; i < 10; i++){
        char * fuzzed_str = rand_fuzzer->f_fuzz(rand_fuzzer);
        printf("%s\n", fuzzed_str);
        free(fuzzed_str);
    }

    pProgram_Runner cat = program_runner_init("cat");

    for(int i = 0; i < 10; i++){
        char * fuzzed = rand_fuzzer->f_fuzz(rand_fuzzer);
        p_result result = cat->f_run(cat, fuzzed);
        assert(strcmp(result->str, fuzzed) == 0);
        assert(strcmp(result->status, PASS) == 0);
        printf("args = %s, returncode = %d, stdout=%s, stderr=%s\n", cat->program, cat->status, cat->outputs, cat->errors);
        free(fuzzed);
        free(result);
    }

    free(rand_fuzzer);
}

int main()
{
    srand(time(0x0));

    //test_fuzzer_class();

    test_rand_fuzzer_class();

    //test_rand_fuzzer_class2();

}