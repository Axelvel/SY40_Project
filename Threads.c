#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include "utils.h"





#define N 100 //Maximum number of bocals


pthread_mutex_t mutex;
pthread_cond_t cbocal, cvalve, cclock, cwait, nouveau;

pthread_t tbocal[N];
pthread_t tvalve;
pthread_t thorloge;


long bocalType;
int count;

void * bocal(void * data) {
    pthread_mutex_lock(&mutex);
    
    pthread_cond_wait(&nouveau, &mutex);
    
    
    bocalType = (long) data;
    count++;
    
    printf("\nPlacer un bocal (%d)\n", count);
    
	pthread_cond_signal(&cbocal);
  
    
    pthread_cond_wait(&cvalve, &mutex);

    printf("Enlever bocal\n");
    
    pthread_cond_signal(&nouveau);
    
    pthread_mutex_unlock(&mutex);
    //pthread_exit(NULL);

}


void * valve() {
    pthread_mutex_lock(&mutex);
   
	while(1) {
		
	pthread_cond_wait(&cbocal, &mutex);

    printf("Ouverture valve\n");
    
    //for (int i = 0; i < l->value; i++){
   for (int i = 0; i < bocalType; i++){
    pthread_cond_signal(&cclock);
    
    pthread_cond_wait(&cwait, &mutex);
    
    
	}
	
	//l = deleteHead(l);
    printf("Fermeture valve\n");
    pthread_cond_signal(&cvalve); 
		
	}
	
    
    pthread_mutex_unlock(&mutex);
    //pthread_exit(NULL);

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
    //pthread_exit(NULL);
}


int main(int argc, char const *argv[]) {

  //pthread_mutex_init(&mutex, NULL); //Initialisation du mutex
  
	int count = 0;
  
	if (pthread_mutex_init(&mutex, NULL) != 0) {
		printf("mutex failed");
	}
  //for (int j = 0; j < N; j++) {
    printf("\n");
   // printf("Bocal : %i\n", j);
   
	int numberOfBocals;
    long type;
    long time;
    
    do {
        printf("\nHow many bocals do you want to produce?: ");
        fflush(stdin);
    } while (scanf("%d", &numberOfBocals) != 1 || numberOfBocals <= 0);
    
    
    for (int i = 0; i < numberOfBocals; i++) {
        
        printf("\nBocal %d\n", i);
        
        do {
            printf("What type: ");
            fflush(stdin);
        } while (scanf("%ld", &type) != 1 || type <= 0);
        
        
        pthread_create(&tbocal[i], NULL, bocal, (void *) type);
 
    }
    
    do {
        printf("\nHow long should the timer be?: ");
        fflush(stdin);
    } while (scanf("%ld", &time) != 1 || time <= 0);
    
   
    pthread_create(&thorloge, NULL, clockt, (void *) time);
    pthread_create(&tvalve, NULL, valve, NULL);
 
		 
		 
		 sleep(1);
	
		 
		 startProduction(1);
		 
		 //pthread_cond_signal(&cnew);
		 
		 for (int i = 0; i < numberOfBocals; i++) {
			 pthread_join(tbocal[i], NULL);
		}
  
  
  pthread_mutex_destroy(&mutex);

  printf("\nTerminé\n");


  return 0;
  //pthread_exit(NULL);
}
