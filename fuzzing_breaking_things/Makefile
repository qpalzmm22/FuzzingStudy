all: fuzzer

fuzzer : src/fuzzer.c src/tester.c src/my_fileIO.c
	gcc -o fuzzer src/tester.c src/fuzzer.c src/my_fileIO.c

clean :
	rm -r temp_* fuzzer