all : cgi_decode cgi_decode_ex gcov_creater coverage_calculator

cgi_decode : src/cgi_decode.c 
	gcc -o bin/cgi_decode src/cgi_decode.c

cgi_decode_ex : cgi_decode_ex.c
	gcc --coverage -o bin/cgi_decode_ex cgi_decode_ex.c

gcov_creater : src/gcov_creater.c
	gcc -o bin/gcov_creater src/gcov_creater.c

coverage_calculator : src/coverage_calculator.c src/gcov_creater.c
	gcc -o bin/coverage_calculator src/coverage_calculator.c src/gcov_creater.c

clean : 
	rm -r bin/* *.gcda *.gcno *.gcov a.out