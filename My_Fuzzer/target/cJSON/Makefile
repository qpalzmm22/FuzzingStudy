all : my ji mj sy ws 
	gcc -o my/my --coverage my/cJSON.c my/my.c
	gcc -o ji/ji --coverage ji/cJSON.c ji/ji.c
	gcc -o mj/mj --coverage mj/cJSON.c mj/mj.c
	gcc -o sy/sy --coverage sy/cJSON.c sy/sy.c
	gcc -o ws/ws --coverage ws/cJSON.c ws/ws.c

my :
	gcc -o my/my --coverage my/cJSON.c my/my.c

ji :
	gcc -o ji/ji --coverage ji/cJSON.c ji/ji.c

mj :
	gcc -o mj/mj --coverage mj/cJSON.c mj/mj.c

sy :
	gcc -o sy/sy --coverage sy/cJSON.c sy/sy.c

ws :
	gcc -o ws/ws --coverage ws/cJSON.c ws/ws.c


clean : 
	rm -rf my/my ji/ji ws/ws sy/sy mj/mj  */*.gc* 
 
