all: scheduler

clean:
	rm scheduler main.o parse.o scheduler.o

scheduler: main.o parse.o scheduler.o
	gcc -o scheduler main.o parse.o scheduler.o

main.o: main.c
	gcc -c main.c

parse.o: parse.c parse.h
	gcc -c parse.c

scheduler.o: scheduler.c scheduler.h
	gcc -c scheduler.c
