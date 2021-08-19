# README

This is serious Fuzzer.

I created abstract fuzzer and fuzzer driver that runs it. Fuzzer driver, which is only confined to `cat` command in this directory, gives config. info. to fuzzer and runs the fuzzer. The fuzzer takes the arguments, runs the program and returns the result.

## HOW TO USE

See `config.h`. Here, you can change the settings of fuzzer. Before start, YOU MUST make oracle function and include the header file of it in `config.h` and define `ORACLE` by the function of your choice.
 

- Things To consider
  - How to receive configs -> with `config.h` file.

  - Config
    - Fuzzer config
      - Time interrupt timing
    - String config
      - length
      - range
      - Some useful config?
    - Oracle config
      - What is `"unexpected outcome"`
        - ret = 0 but wrong. `false positives`
          - In case of `cat`, what goes in comes out. 
        - ret != 0 but right? `false negative`
          - In case of `cat`, see error code. 
        - `Run time errors` are bugs.


          - WIFEXITED
        - how to make Oracle generic?
        - Generic orcale returns `0` when 
  
      - How to save the results => files, but only "unexpected outcomes" => can set it in config..? or must

    - Program config
      - How to input fuzzed strings in program(1. STDIN, 2. Argument, 3. fuzzed string in file).
      - With or without env. vars? with different envs? fuzz envs?
  
- TODO lists
  - M_ARG, M_FILE MODE
  - Oracles
  - review code
  - Make file only if they are reasonable.

Assumptions
- In, out files are in same path.


Q.
1. Why not fuzz env. vars.