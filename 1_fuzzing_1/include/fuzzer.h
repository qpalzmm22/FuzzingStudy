#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <limits.h>
#include <sys/wait.h>
#include <fcntl.h>



// dynamically allocated string.
char * fuzzer(int max_length, int char_start, int char_range);

// fuzzer(100, 32, 32)
char *default_fuzzer();
