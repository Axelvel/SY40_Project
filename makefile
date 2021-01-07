
all: utils.o executable clean

executable: semaphores threads
	gcc -o main main.c

semaphores: semaphores.o
	gcc -o semaphores semaphores.o utils.o

semaphores.o: semaphores.c
	gcc -c semaphores.c

threads: threads.o
	gcc -o threads threads.o utils.o -pthread

threads.o: threads.c
	gcc -c threads.c -pthread
	
utils.o: utils.c
	gcc -c utils.c
	
clean:
	rm *.o
