all: bc_fuzzer cat_fuzzer buff_ovf llvm_ex no_llvm_ex info_leak prog_spec_check runner class_fuzzer

bc_fuzzer : src/fuzzer.c test/tester.c src/my_fileIO.c
	gcc -o bin/bc_fuzzer test/tester.c src/fuzzer.c src/my_fileIO.c

cat_fuzzer : src/fuzzer.c test/tester_fuzzer.c src/runner.c
	gcc -o bin/cat_fuzzer test/tester_fuzzer.c src/fuzzer.c src/runner.c

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

runner : src/runner.c test/test_runner.c 
	gcc -o bin/runner test/test_runner.c src/runner.c

class_fuzzer : src/fuzzer_class.c src/runner.c test/test_fuzzer_class.c
	gcc -o bin/class_fuzzer src/fuzzer_class.c src/runner.c test/test_fuzzer_class.c

clean :
	rm -r bin/* tmp_* result/errors/* result/outputs/* 