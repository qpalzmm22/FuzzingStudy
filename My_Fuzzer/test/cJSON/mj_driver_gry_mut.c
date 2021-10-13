#include "../../include/fuzzer.h"

int
main(){
    config_t config;
    config.oracle = 0x0;

    init_fuzzer(&config);

    config.exec_mode = M_STDIN;
   	strcpy(config.prog_path, "/home/qpalzmm22/FuzzingStudy/My_Fuzzer/target/cJSON/mj/mj");
    strcpy(config.src_dir_path, "/home/qpalzmm22/FuzzingStudy/My_Fuzzer/target/cJSON/mj");
    
    config.max_trial = 5000;
    config.timeout = 100000;

    
    for(int i = 0; i < 5; i++){
        fuzz_main(&config);
    }

    return 0;
}
