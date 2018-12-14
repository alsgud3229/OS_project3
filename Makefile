all:
	gcc -I./include -c ./lib/mythread.c
	ar rsc libmythread.a mythread.o
	gcc -o example ./src/main.c -L. -lmythread -I./include
