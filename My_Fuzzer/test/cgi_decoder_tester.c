#include "../include/fuzzer.h"

int
main(){
    config_t config;
    
    init_fuzzer(&config);
    fuzz_main(&config);

    return 0;
}