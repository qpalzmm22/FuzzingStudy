all: fuzzer

fuzzer : src/fuzzer.c tester.c src/my_fileIO.c
	gcc -o fuzzer tester.c src/fuzzer.c src/my_fileIO.c

clean :
	rm -r tmp_* fuzzer result/errors/* result/outputs/*