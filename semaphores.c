
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/sem.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include "utils.h"


#define SKEY   (key_t) IPC_PRIVATE //CLE CREE AU HASARD

int sem_id ; //identificateur des sémaphores



void bocal(int n, int goal){
  if (!fork()) {
      
      int count = 0;
      
      while(count < goal){
          
          P(5);

          printf("\nNouveau bocal %d (Type %d)\n", count, n);
            
          
          printf("Placer un bocal\n");
          V(1);
          
        
          semctl(sem_id, 0, SETVAL, n);

          P(4);
          printf("Enlever bocal\n");
            
          V(5);
          
          sleep(2);
          count++;
      }
        
      

    exit(0);
  }
}



void valve() {
  if (!fork()) {
    
      while(1) {
          P(1);
          printf("Ouverture valve\n");
            
            int value = semctl(sem_id, 0, GETVAL);
            //printf("Type = %i\n", m);
            
            for (int i = 0; i < value; i ++) {
                V(2);
                P(3);
            }
          
          printf("Fermeture valve\n");
            
          V(4);
      }

    exit(0);
  }
}


void horloge(int i){
  if (!fork()) {
      while(1) {
          P(2);
          printf("Horloge lancée\n");
          sleep(i);
          printf("Horloge finie\n");
          V(3);
      }
    

    exit(0);
  }
}



int main(int argc, char const *argv[]) {

  int i,semid; //Semid : identificateur des sémaphores
    initsem(SKEY); //On initialise le tableau de sémaphores
    
    
    int numberOfMachines;
    int type;
    int num;
    int time;
    
    do {
        printf("\nHow many production machines do you want?: ");
        fflush(stdin);
    } while (scanf("%d", &numberOfMachines) != 1 || numberOfMachines <= 0);
    
    
    for (int i = 0; i < numberOfMachines; i++) {
        
        printf("\nMachine %d\n", i);
        
        do {
            printf("What type: ");
            fflush(stdin);
        } while (scanf("%d", &type) != 1 || type <= 0);
        
        do {
            printf("How many: ");
            fflush(stdin);
        } while (scanf("%d", &num) != 1 || num <= 0);
        
        bocal(type, num);
    }
    
    do {
        printf("\nHow long should the timer be?: ");
        fflush(stdin);
    } while (scanf("%d", &time) != 1 || time <= 0);
    

      
    valve();
    horloge(time);
    
    startProduction(0);

      for (i=1; i<=numberOfMachines; i++) wait(0); //Ends the machines
      
      sleep(1);

  //TODO: liberSem()

  return 0;
}

