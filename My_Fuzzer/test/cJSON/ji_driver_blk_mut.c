#include "../../include/fuzzer.h"

int
main(){
    config_t config;
    config.oracle = 0x0;

    init_fuzzer(&config);

 	config.exec_mode = M_STDIN;
	config.box_mode = M_BLACK;

 	strcpy(config.prog_path, "/home/qpalzmm22/FuzzingStudy/My_Fuzzer/target/cJSON/ji/ji");
    strcpy(config.src_dir_path, "/home/qpalzmm22/FuzzingStudy/My_Fuzzer/target/cJSON/ji/");
    
    config.max_trial = 500000;
    config.timeout = 10000000;

    
    for(int i = 0; i < 5; i++){
        fuzz_main(&config);
    }

    return 0;
}
