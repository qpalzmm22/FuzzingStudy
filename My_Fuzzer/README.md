# README

This is serious Fuzzer.

I created abstract fuzzer and fuzzer driver that runs it. Fuzzer driver is specific to program and must be configureed specific to the program. You must create `Oracle` specific to the program and configure the fuzzer setting and run the fuzzer on the driver. The fuzzer takes the arguments, runs the program and returns the result.

## HOW TO USE

See `config.h`. Here, you can change the settings of fuzzer. Before start, YOU MUST make oracle function and include the header file of it in `config.h` and define `ORACLE` by the function of your choice.
 

- Things To consider
  - How to receive configs -> with `config.h` file. Is this ok??
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
  
      - How to save the results => files, but only "unexpected outcomes" => can set it in config..? or must


    - Program config
      - How to input fuzzed strings in program(1. STDIN, 2. Argument, 3. fuzzed string in file).
      - Fuzz mode => fuzz **binary** file, **source** file, or **precompiled** file.

    - Question
      - With or without env. vars? with different envs? fuzz envs?
      - how to make Oracle generic
  
- TODO lists
  - review code
  - Make file only if they are reasonable.

Assumptions
- In, Out files are in same path.
- If compiled mode, we assume that all the gcno, gcda files are in same directory
- If return code is not 0, we assume that it's a bug
- We assume that user won't put weird input on path. (Doesn't do much of input verification) <= needs to be fix
  
- For testing bc, copy all the source files to fuzzing dir.


Q.
1. Why not fuzz env. vars.

- Useful Reference
  - (Useful Mutations)[https://lcamtuf.blogspot.com/2014/08/binary-fuzzing-strategies-what-works.html]
