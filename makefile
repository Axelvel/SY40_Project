
all: executable clean

executable: semaphores threads
	gcc -o main main.c

semaphores: Two.o
	gcc -o semaphores Two.o

Two.o: Two.c
	gcc -c Two.c

threads: Threads.o
	gcc -o threads Threads.o

Threads.o: Threads.c
	gcc -c Threads.c

clean :
	rm *.o