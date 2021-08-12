#include "../include/fuzzer.h"

int
main(){
    init_fuzzer();
    fuzzloop();

    return 0;
}