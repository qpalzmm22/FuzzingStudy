all : fuzzer

fuzzer : src/fuzzer.c
	gcc -o bin/fuzzer src/fuzzer.c src/fuzz_input_maker.c

clean : 
	rm -r bin/* tmp* 