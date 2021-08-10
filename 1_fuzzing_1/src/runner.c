#include "../include/runner.h"


// ----------------- Runner Class --------------------

p_result runner_run(pRunner runner, char * inp)
{
    // how to check if runner is initialized??

    p_result res_tup = (p_result)malloc(sizeof(result));
    res_tup->str = inp;
    res_tup->status = UNRESOLVED;
    return res_tup;
}

pRunner runner_init()
{
    pRunner runner = (pRunner)malloc(sizeof(Runner));
    assert(runner);
    // TODO : better this.
    strcpy(runner->fail, FAIL);
    strcpy(runner->pass, PASS);
    strcpy(runner->unresolved, UNRESOLVED);
    runner->f_run = (fp_run)runner_run;

    return runner;
}

// ----------------- Runner Class --------------------

pPrint_Runner print_runner_init()
{
    pPrint_Runner runner = (pPrint_Runner)runner_init();
    runner->f_run = (fp_run)print_Runner_run;

    return runner;
}

p_result print_Runner_run(pPrint_Runner runner, char * inp)
{
    printf("%s\n", inp);
    return runner_run((pRunner)runner, inp);
}


// ----------------- ProgramRunner Class --------------------

pProgram_Runner program_runner_init(char *prog_name)
{
    pProgram_Runner runner = (pProgram_Runner)malloc(sizeof(Program_Runner)) ;
    assert(runner) ;
    strcpy(runner->fail, FAIL) ;
    strcpy(runner->pass, PASS) ;
    strcpy(runner->unresolved, UNRESOLVED) ;
    strcpy(runner->program, prog_name) ;

    runner->f_run = (fp_run) program_runner_run;
    runner->f_run_proc = (fp_run_process) pr_run_process;
    return runner;
}

// short name for ProcessRunning_run_process
int pr_run_process(pProgram_Runner runner,  char * in_buff)
{
    char * prog_name = runner->program;
    //printf("prg: %sargv[1]:%s\n", prog_name, argv[1]);
    int pipe_in[2];
    int pipe_out[2];
    int pipe_err[2];
    
    // PIPE check
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
            close(pipe_in[WRITEEND]) ;

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
            runner->status = status;

            // Assumes that the output is less than 1024...
            
            runner->outputs = (char*) malloc( 1024 * sizeof(char));

            // TODO realloc
            while((s = read(pipe_out[READEND], runner->outputs, 1023)) > 0){
			}
            runner->outputs[1023] = '\0';
            
            runner->errors = (char*) malloc( 1024* sizeof(char));
            // TODO realloc
            while((s = read(pipe_err[READEND],  runner->errors, 1023)) > 0){
			}

            runner->outputs[1023] = '\0';

            close(pipe_out[READEND]) ;
            close(pipe_err[READEND]) ;

            return status;
    }
}

p_result program_runner_run(pProgram_Runner runner, char * inp)
{
    p_result res_tup = (p_result)malloc(sizeof(result));
    int status = runner->f_run_proc(runner, inp);
    res_tup->str = inp;

    if(status == 0){
        res_tup->status = PASS;
    } else if (status < 0) {
        res_tup->status = FAIL;
    } else {
        res_tup->status = UNRESOLVED;
    }
    return res_tup;
}