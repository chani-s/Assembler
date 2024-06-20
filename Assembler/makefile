run: main.o macro_process.o cmp_funs.o first_scan.o list.o second_scan.o general_funs.o
	gcc -g -ansi -pedantic -Wall main.o macro_process.o cmp_funs.o first_scan.o second_scan.o list.o general_funs.o -o run
main.o: main.c defs.h macro_process.h general_funs.h first_scan.h
	gcc -c -ansi -pedantic -Wall main.c -o main.o
macro_process.o: macro_process.c defs.h macro_process.h cmp_funs.h general_funs.h
	gcc -c -ansi -pedantic -Wall macro_process.c -o macro_process.o
cmp_funs.o: cmp_funs.c defs.h cmp_funs.h
	gcc -c -ansi -pedantic -Wall cmp_funs.c -o cmp_funs.o
first_scan.o: first_scan.c defs.h cmp_funs.h general_funs.h list.h first_scan.h second_scan.h
	gcc -c -ansi -pedantic -Wall first_scan.c -o first_scan.o
list.o: list.c defs.h list.h cmp_funs.h general_funs.h
	gcc -c -ansi -pedantic -Wall list.c -o list.o
second_scan.o: second_scan.c defs.h second_scan.h cmp_funs.h general_funs.h list.h
	gcc -c -ansi -pedantic -Wall second_scan.c -o second_scan.o
general_funs.o: general_funs.c defs.h
	gcc -c -ansi -pedantic -Wall general_funs.c -o general_funs.o
