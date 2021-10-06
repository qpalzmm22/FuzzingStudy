#include "../../include/fuzzer.h"

int
main(){
    config_t config;
    config.oracle = 0x0;

    init_fuzzer(&config);

    config.rsg_type = T_RSG ;
    config.exec_mode = M_FILE;
    strcpy(config.prog_path, "/home/qpalzmm22/FuzzingStudy/My_Fuzzer/target/cJSON/cjson_test_case");
    strcpy(config.src_dir_path, "/home/qpalzmm22/FuzzingStudy/My_Fuzzer/target/cJSON/");
    
    config.max_trial = 5000;
    config.timeout = 100000;

    
    fuzz_main(&config);

    return 0;
}