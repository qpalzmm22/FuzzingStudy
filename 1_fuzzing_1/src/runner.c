#include "../include/runner.h"


// ----------------- Runner Class --------------------

p_result runner_run(char * inp)
{
    // how to check if runner is initialized??

    p_result res_tup = (p_result)malloc(sizeof(result));
    res_tup->input = inp;
    res_tup->status = UNRESOLVED;
    return res_tup;
}

pRunner runner_init()
{
    pRunner runner = (pRunner)malloc(sizeof(Runner));
    assert(runner);
    strcpy(runner->fail, FAIL);
    strcpy(runner->pass, PASS);
    strcpy(runner->unresolved, UNRESOLVED);
    runner->func_p = (fp_run)runner_run;

    return runner;
}


// ----------------- Runner Class --------------------

pPrint_Runner print_runner_init()
{
    pPrint_Runner runner = (pPrint_Runner)runner_init();
    runner->func_p = (fp_run)print_Runner_run;

    return runner;
}

p_result print_Runner_run(pPrint_Runner runner, char * inp)
{
    printf("%s\n", inp);
    return runner_run(inp);
}


// ----------------- ProgramRunner Class --------------------

pProgram_Runner program_runner_init()
{
    pProgram_Runner runner = (pProgram_Runner)runner_init();
    runner->func_p = (fp_run) print_Runner_run;

    return runner;
}

p_result program_runner_run(pProgram_Runner runner, char * inp){
    p_result res_tup = (p_result)malloc(sizeof(result));
    int status = runner->func_p2(runner, inp);
    res_tup->input = inp;

    if(status == 0){
        res_tup->status = PASS;
        printf("exit status : 0\n");
    } else if (status < 0) {
        res_tup->status = FAIL;
        printf("exit status : less than 0 [%d]\n",status);
    } else {
        res_tup->status = UNRESOLVED;
        printf("exit status : UNSOLVED[%d]\n",status);
    }
    return res_tup;
}

// short name for ProcessRunning_run_process
int pr_run_process(char * prog_name, char * in_buff)
{
    
    //printf("prg: %sargv[1]:%s\n", prog_name, argv[1]);
    int pipe_in[2];
    int pipe_out[2];
    int pipe_err[2];
    
    pipe(pipe_in);
    pipe(pipe_out);
    pipe(pipe_err);

    ssize_t s = 0;

    switch(fork()){
        case -1 :
            perror("ERROR in fork");
            break;
        case 0 :   
            close(pipe_out[READEND]) ;
            close(pipe_err[READEND]) ;

            write(pipe_in[WRITEEND], in_buff, strlen(in_buff));

            dup2(pipe_in[READEND], STDIN_FILENO) ;      
            close(pipe_in[READEND]) ;
            close(pipe_in[WRITEEND]);

            dup2(pipe_out[WRITEEND], STDOUT_FILENO) ;
            dup2(pipe_err[WRITEEND], STDERR_FILENO) ;

            execlp(prog_name, prog_name, NULL);
            break;
        default :
            close(pipe_in[WRITEEND]) ;
            close(pipe_in[READEND]) ;
            
            close(pipe_out[WRITEEND]) ;
            close(pipe_err[WRITEEND]) ;

            int status;
            wait(&status);

            char out_buff[1024];
            
            while((s = read(pipe_out[READEND], out_buff, 1024)) > 0){
				printf("out_buff : %s\n", out_buff);
			}

            char err_buff[1024];
            
            while((s = read(pipe_err[READEND], err_buff, 1024)) > 0){
				printf("err_buff : %s\n", err_buff);
			}

            close(pipe_out[READEND]) ;
            close(pipe_err[READEND]) ;

            return status;
    }
}
