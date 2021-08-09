#include "../include/runner.h" 

void test_runner(){
    pPrint_Runner pRunner;
    pRunner = print_runner_init();
    p_result res = pRunner->func_p(pRunner, "qpalzmm22");

    printf("%s\n", pRunner->fail);

    // Gotta FREE
    
    //

}

void test_runner2(){

    pr_run_process("cat", "hello Im Isaac\n");
}

int main(){
    
    test_runner();

    test_runner2();
    return 0;
}

    

