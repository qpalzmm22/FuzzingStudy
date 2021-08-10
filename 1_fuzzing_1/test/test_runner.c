#include "../include/runner.h" 

void test_runner(){
    pPrint_Runner pRunner;
    pRunner = print_runner_init();
    p_result res = pRunner->f_run(pRunner, "qpalzmm22");

    printf("%s\n", pRunner->fail);

    free(res);
    free(pRunner);

}

void test_runner2(){
    
    pProgram_Runner pr_runner = program_runner_init("cat");

    p_result res = pr_runner->f_run(pr_runner, "hello");
    
    printf("%s\n", res->str);
    printf("%s\n", res->status);

    free(res);
    free(pr_runner);
}

int main(){
    
    //test_runner();

    test_runner2();
    return 0;
}

    

