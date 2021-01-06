
all: executable clean

executable: semaphores threads
	gcc -o main main.c

semaphores: semaphores.o
	gcc -o semaphores semaphores.o utils.o

semaphores.o: semaphores.c
	gcc -c Two.c

threads: Threads.o
	gcc -o threads Threads.o utils.o

Threads.o: Threads.c
	gcc -c Threads.c
	
utils.o: utils.c
	gcc -c utils.c
	
clean :
	rm *.o
