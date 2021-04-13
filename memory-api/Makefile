all: null forget_free data free_print funny_free_print

clean:
	rm -f null forget_free data free_print funny_free_print

null: null.c
	gcc -g -o null null.c -Wall

forget_free: forget_free.c
	gcc -g -o forget_free forget_free.c -Wall -Wno-unused-but-set-variable

data: data.c
	gcc -g -o data data.c -Wall

free_print: free_print.c
	gcc -g -o free_print free_print.c -Wall

funny_free_print: funny_free_print.c
	gcc -g -o funny_free_print funny_free_print.c -Wall
