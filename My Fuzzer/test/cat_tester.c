#include "../include/fuzzer.h"

int
main(){

    init_fuzzer();
    fuzz_loop();

    return 0;
}