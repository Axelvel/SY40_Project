#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main(int argc, char const *argv[]) {

    
    int choice;
    
    do {
        printf("Quel programme voulez vous lancer ?\n");
        printf("    - Semaphores : 1\n");
        printf("    - Threads : 2\n");
        fflush(stdin);
    } while (scanf("%d", &choice) != 1 || choice <= 0 || choice > 2);
    
    if (choice == 1) {
        //Semaphores
        execl("semaphores", "semaphores", NULL);
    } else if (choice == 2){
        //Threads
        execl("threads", "threads", NULL);
    }

  return 0;
}

