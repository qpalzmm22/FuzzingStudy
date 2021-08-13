all : fuzzer

fuzzer : src/fuzzer.c src/fuzz_input_maker.c test/cat_tester.c
	gcc -o bin/fuzzer src/fuzzer.c src/fuzz_input_maker.c test/cat_tester.c

clean : 
	rm -r bin/* tmp* 