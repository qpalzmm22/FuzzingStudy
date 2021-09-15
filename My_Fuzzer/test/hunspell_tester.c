int
hunspell_oracle(int ret_code, char* input_buff, int input_len , char* stdout_buff, char* stderr_buff)
{
    if(ret_code != 0){
        printf("Return code : %d\n", ret_code);
        printf("input : %s\nstdout : %s\nstderr : %s\n", input_buff, stdout_buff, stderr_buff);
        return 0;
    }
    return 1;
}

int
main()
{
    init_fuzzer(hunspell_oracle);
    fuzz_loop();

    print_stats();

    return 0;
}