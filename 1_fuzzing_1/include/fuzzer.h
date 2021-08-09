#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <limits.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <assert.h>



// dynamically allocated string.
char * fuzzer(int max_length, int char_start, int char_range);

// fuzzer(100, 32, 32)
char *default_fuzzer();

char *random_fuzzer(int min_len, int max_len, int char_start, int char_range);

// random_fuzzer(10, 100, 32, 32)
char *default_rand_fuzzer();

int fuzzer_n(int max_length, int char_start, int char_range, char **buf);

int default_fuzzer_n(char **buf);
