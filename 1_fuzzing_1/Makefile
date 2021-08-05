all: long_fuzzer buff_ovf

long_fuzzer : src/fuzzer.c tester.c src/my_fileIO.c
	gcc -o bin/fuzzer tester.c src/fuzzer.c src/my_fileIO.c

buff_ovf :	examples/buffer_vul.c src/fuzzer.c
	gcc -o bin/buff_ovf examples/buffer_vul.c src/fuzzer.c -fno-stack-protector
clean :
	rm -r bin/* tmp_* result/errors/* result/outputs/* 