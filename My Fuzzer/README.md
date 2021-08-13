# README

This is serious Fuzzer.

I created abstract fuzzer and fuzzer driver that runs it. Fuzzer driver, which is only confined to `cat` command in this directory, gives config. info. to fuzzer and runs the fuzzer. The fuzzer takes the arguments, runs the program and returns the result.

## HOW TO USE

See `config.h`. Here, you can change the settings of fuzzer. Before start, YOU MUST make oracle function and include the header file of it in `config.h` and define `ORACLE` by the function of your choice.
 

- Things To consider
  - How to receive configs. JSON file?
  - Config
    - Fuzzer config
      - Time interrupt timing
    - String config
      - length
      - range
      - Some useful config?
    - Oracle config
      - What is "unexpected outcome"
      - How to save the results => files, but only "unexpected outcomes" => can set it in config

    - Program config
      - How to input fuzzed strings in program(1. STDIN, 2. Argument, 3. fuzzed string in file).
      - With or without env. vars? with different envs? fuzz envs?



Q.
1. Why not fuzz env. vars.