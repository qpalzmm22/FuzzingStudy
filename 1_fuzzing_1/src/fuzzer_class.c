#include "../include/fuzzer_classes.h"

// ---------------- Fuzzer class -----------
pFuzzer fuzzer_init()
{
    pFuzzer fuzzer = (pFuzzer)malloc(sizeof(Fuzzer));
    fuzzer->f_fuzz = (fp_fuzz)fuzzer_fuzz;
    fuzzer->f_run = (fp_f_run)fuzzer_run;
    fuzzer->f_runs = (fp_runs)fuzzer_runs;
}

char * fuzzer_fuzz()
{
    char * empty_string = (char *) malloc(1 *sizeof(char));
    empty_string[0] = '\0';
    return empty_string;
}

// TODO : FREE...
p_result fuzzer_run(pFuzzer fuzzer, pRunner runner)
{
    if(runner == 0x0){
        runner = runner_init();
    }
    p_result result = runner->f_run(runner, fuzzer->f_fuzz());
    return result;
}

p_result * fuzzer_runs(pFuzzer fuzzer, pPrint_Runner print_runner, int trials)
{
    p_result * results = (p_result *) malloc(trials * sizeof(p_result));

    if(print_runner == 0x0){
        print_runner = print_runner_init();
    }
    for(int i = 0; i < trials; i++){
        results[i] = fuzzer->f_run(fuzzer, print_runner);
    }
    return results;
}

// ---------------- RandomFuzzer class --------------

pRandom_Fuzzer random_fuzzer_init()
{
    pRandom_Fuzzer random_fuzzer = (pRandom_Fuzzer)malloc(sizeof(Random_Fuzzer));
    random_fuzzer->f_fuzz = (fp_fuzz)random_fuzzer_fuzz;
    random_fuzzer->f_run = (fp_f_run)fuzzer_run;
    random_fuzzer->f_runs = (fp_runs)fuzzer_runs;
    random_fuzzer->min_length = 10;
    random_fuzzer->max_length = 10;
    random_fuzzer->char_start = 32;
    random_fuzzer->char_range = 32;
}

char* random_fuzzer_fuzz(pRandom_Fuzzer random_fuzzer)
{
    int min_len = random_fuzzer->min_length;
    int max_len = random_fuzzer->max_length;
    int char_start = random_fuzzer->char_start;
    int char_range = random_fuzzer->char_range;


    char * rand_arr;
	int len = rand() % (max_len - min_len + 1) + min_len ;

	rand_arr = (char *) malloc(sizeof(char) * (len + 1));
	assert(rand_arr);

	for(int i = 0; i < len; i++){
		rand_arr[i] = char_start + rand() % (char_range);
	}
	rand_arr[len] = '\0';

	return rand_arr;
}

