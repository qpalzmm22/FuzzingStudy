#include "../include/fuzzer.h"
#include "../include/cat_oracle.h"

int
main(){

    init_fuzzer(cat_oracle);
    fuzz_loop();

    return 0;
}