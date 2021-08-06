all: long_fuzzer buff_ovf llvm_ex no_llvm_ex info_leak prog_spec_check

long_fuzzer : src/fuzzer.c tester.c src/my_fileIO.c
	gcc -o bin/fuzzer tester.c src/fuzzer.c src/my_fileIO.c

buff_ovf :	examples/buffer_vul.c src/fuzzer.c
	gcc -o bin/buff_ovf examples/buffer_vul.c src/fuzzer.c -fno-stack-protector

llvm_ex : examples/program.c
	clang -o bin/llvm_program examples/program.c -fsanitize=address -g  

no_llvm_ex : examples/program.c
	clang -o bin/llvm_program examples/program.c -g  

info_leak : examples/info_leak.c
#	clang -o bin/info_leak examples/info_leak.c src/fuzzer.c -fsanitize=address -g 
	gcc -o bin/info_leak examples/info_leak.c src/fuzzer.c

prog_spec_check : examples/prog_spec_checker.c
	gcc -o bin/prog_spec_check examples/prog_spec_checker.c 

clean :
	rm -r bin/* tmp_* result/errors/* result/outputs/* 