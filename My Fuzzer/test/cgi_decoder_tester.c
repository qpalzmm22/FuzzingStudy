#include "../include/fuzzer.h"

int
main(){

    init_fuzzer(0x0);
    fuzz_loop();

    return 0;
}