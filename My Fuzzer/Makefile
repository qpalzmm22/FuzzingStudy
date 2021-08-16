all : fuzzer psudo_troff test_p_troff

fuzzer : src/fuzzer.c src/fuzz_input_maker.c test/cat_tester.c 
	gcc -o bin/fuzzer src/fuzzer.c src/fuzz_input_maker.c test/cat_tester.c test/cat_oracle.c

test_p_troff: psudo_troff src/fuzzer.c src/fuzz_input_maker.c test/psudo_troff_tester.c
	gcc -o bin/test_p_troff src/fuzzer.c src/fuzz_input_maker.c test/psudo_troff_tester.c

psudo_troff : test/psudo_troff.c
	gcc -o bin/psudo_troff test/psudo_troff.c

clean : 
	rm -r bin/* tmp* 