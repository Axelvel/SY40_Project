
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/sem.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include "utils.h"


#define SKEY   (key_t) IPC_PRIVATE //Cle creee au hasard
int sem_id ; //Identificateur des semaphores


void bocal(int type, int num){ //Fonction creant un processus bocal
    
    
    if (!fork()) {
        
      
        int count = 0; //Initialisation du compteur de bocaux de la machine
      
        while(count < num) {
          
            P(5);

            printf("\nNouveau bocal %d (Type %d)\n", count, type);
            
          
            printf("Placer un bocal\n");
          
            V(1);
          
        
            semctl(sem_id, 0, SETVAL, type); //Affecte la valeur du type au semaphore 0 (pour pouvoir la reutiliser dans le processus valve)

            P(4);
          
            printf("Enlever bocal\n");
            
            V(5);
    
            count++; //Incrémente le compteur de bocaux
        }

        exit(0);
      
    }
    
}



void valve() {
    
    if (!fork()) {
    
        while(1) {
            
            P(1);
            
            printf("Ouverture valve\n");
            
            int value = semctl(sem_id, 0, GETVAL); //Recupere la valeur du type de bocal actuel
            
            for (int i = 0; i < value; i ++) { //Lancement puis attente de la fin du processus horloge
                
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
    
    do { //Combien de machines de production
        
        printf("\nHow many production machines do you want?: ");
        fflush(stdin);
        
    } while (scanf("%d", &numberOfMachines) != 1 || numberOfMachines <= 0);
    
    
    for (int i = 0; i < numberOfMachines; i++) {
        
        printf("\nMachine %d\n", i);
        
        do { //Quel type de bocal associer a la machine
            
            printf("What type?: ");
            fflush(stdin);
            
        } while (scanf("%d", &type) != 1 || type <= 0);
        
        do { //Quelle quantite la machine devra produire
            
            printf("How many?: ");
            fflush(stdin);
            
        } while (scanf("%d", &num) != 1 || num <= 0);
        
        bocal(type, num); //Lancement du processus bocal adequat
    }
    
    do { //Definition de la duree de l'horloge
        
        printf("\nHow long should the timer be?: ");
        fflush(stdin);
        
    } while (scanf("%d", &time) != 1 || time <= 0);
    

    //Lancement de la valve et de l'horloge
    valve();
    horloge(time);
    
    startProduction(0); //Lancement de la chaine de production

    for (i=1; i<=numberOfMachines; i++) wait(0); //Attend la fin des machines
      
    sleep(1);
    
    printf("\nTerminé\n\n");

  return 0;
}

