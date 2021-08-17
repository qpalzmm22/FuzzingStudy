all : cgi_decode cgi_decode_ex gcov_parser

cgi_decode : src/cgi_decode.c 
	gcc -o bin/cgi_decode src/cgi_decode.c

cgi_decode_ex : src/cgi_decode_ex.c
	gcc --coverage -o bin/cgi_decode_ex src/cgi_decode_ex.c
	
gcov : bin/cgi_decode_ex
	bin/cgi_decode_ex "Send+mail+to+me%40fuzzingbook.org"
	gcov cgi_decode_ex.c

gcov_parser : src/gcov_parser.c gcov cgi_decode_ex.c.gcov
	gcc -o bin/gcov_parser src/gcov_parser.c

clean : 
	rm -r bin/* *.gcda *.gcno *.gcov