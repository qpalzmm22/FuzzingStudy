#include "../include/fuzzer.h"

int
main(){

    // char** argv = { PROG_PATH, PROG_ARGS ,0x0 };

    init_fuzzer();
    fuzz_loop();

    return 0;
}