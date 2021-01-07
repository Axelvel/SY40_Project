#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include "utils.h"

#define N 100 //Nombre maximum de bocaux pouvant etre produits

pthread_mutex_t mutex; //Creation du mutex
pthread_cond_t cbocal, cvalve, cclock, cwait, nouveau; //Creation de variables de condition

pthread_t tbocal[N];
pthread_t tvalve;
pthread_t thorloge;


long bocalType; //Type du bocal actuellement en train d'etre rempli
int count; //Compteur du nombre de bocaux produits

void * bocal(void * data) {
    
    pthread_mutex_lock(&mutex);
    
    pthread_cond_wait(&nouveau, &mutex);
    
    bocalType = (long) data; //Affectation du type du bocal (pour reutilisation dans le thread valve)
    count++; //Incrementation du nombre de bocaux produits
    
    printf("\nPlacer un bocal (%d)\n", count);
    
	pthread_cond_signal(&cbocal);
  
    pthread_cond_wait(&cvalve, &mutex);

    printf("Enlever bocal\n");
    
    pthread_cond_signal(&nouveau);
    
    pthread_mutex_unlock(&mutex);

}


void * valve() {
    
    pthread_mutex_lock(&mutex);
   
	while(1) {
        
        pthread_cond_wait(&cbocal, &mutex);
        
        printf("Ouverture valve\n");
        
        for (int i = 0; i < bocalType; i++) {
            
            pthread_cond_signal(&cclock);
            pthread_cond_wait(&cwait, &mutex);

        }
	
        printf("Fermeture valve\n");
        
        pthread_cond_signal(&cvalve);
		
	}
	
    
    pthread_mutex_unlock(&mutex);
 
}


void * clockt(void * data) {
    
    pthread_mutex_unlock(&mutex);
    
    long i = (long) data;
    
	while(1) {
		
        pthread_cond_wait(&cclock, &mutex);
        
        printf("Horloge lancée\n");
        sleep(i);
        printf("Horloge finie\n");
        
        pthread_cond_signal(&cwait);
    
	}

    pthread_mutex_unlock(&mutex);

}


int main(int argc, char const *argv[]) {
  
	int count = 0;
  
	if (pthread_mutex_init(&mutex, NULL) != 0) { //initialisation du mutex
		printf("Mutex failed\n");
	}

    
	int numberOfBocals;
    long type;
    long time;
    
    
    do { //Nombre de bocaux a produire
        
        printf("\nHow many bocals do you want to produce?: ");
        fflush(stdin);
        
    } while (scanf("%d", &numberOfBocals) != 1 || numberOfBocals <= 0);
    
    
    for (int i = 0; i < numberOfBocals; i++) {
        
        printf("\nBocal %d\n", i);
        
        do { //Affectation du type pour chaque bocal a produire
            
            printf("What type?: ");
            fflush(stdin);
            
        } while (scanf("%ld", &type) != 1 || type <= 0);
        
        pthread_create(&tbocal[i], NULL, bocal, (void *) type); //Lancemement du thread bocal adequat
 
    }
    
    do { //Definition de la duree de l'horloge
        
        printf("\nHow long should the timer be?: ");
        fflush(stdin);
        
    } while (scanf("%ld", &time) != 1 || time <= 0);
    
   //Lancement des threads valve et horloge
    pthread_create(&thorloge, NULL, clockt, (void *) time);
    pthread_create(&tvalve, NULL, valve, NULL);
 
    sleep(1);
	
    startProduction(1);
		 
		 
    for (int i = 0; i < numberOfBocals; i++) { //Attente dee la terminaison de tous les threads bocaux
        
			 pthread_join(tbocal[i], NULL);
        
    }
  
  
  pthread_mutex_destroy(&mutex); //Destruction du mutex

  printf("\nTerminé\n");

  return 0;
    
}
