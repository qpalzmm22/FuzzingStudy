#include "../include/fuzzer.h"

int count_1 = 0;
int count_2 = 0;
int count_3 = 0;

int
ps_troff_oracle(int ret_code, char* input_buff, int input_len , char* stdout_buff, char* stderr_buff)
{

    if(ret_code !=0){
        printf("return code : %d\n", ret_code);
        if(ret_code == 256){
            count_1++;
        } else if(ret_code == 512){
            count_2++;
        } else if(ret_code == 768 ){
            count_3++;
        }
        return 0;
    } else {
        return 1;
    }
}

void
print_stats(){
    printf("================ Psudo Troff Oracle Statistic =================\n");
    printf("\\D followed by non-printable characters : %d\n", count_1);
    printf(". followed by newline : %d\n", count_2);
    printf("8 bit character followed by newline: %d\n", count_3);
    printf("===============================================================\n");
}


int
main()
{
    config_t config;
    config.oracle = ps_troff_oracle;
    
    init_fuzzer(&config);
    fuzz_loop();

    print_stats();

    return 0;
}