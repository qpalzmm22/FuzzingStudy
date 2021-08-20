#include "../include/fuzzer.h"

int
main(){
    config_t config;
    config.oracle = 0x0;

    init_fuzzer(&config);
    fuzz_loop();

    return 0;
}