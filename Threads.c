#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>

//#define NbTh 10      //Nombre de processus symbolisant les clients
//#define N  8	//Nombre de chaises dans le salon de coiffure

#define N 3 //Nombre de bocaux à produire


//pthread_t tid[NbTh+1]; //
pthread_mutex_t mutex;
pthread_cond_t cbocal, cvalve, cclock, attendre, nouveau;

pthread_t tbocal[10];
pthread_t tvalve;
pthread_t thorloge;


void * bocal() {
    pthread_mutex_lock(&mutex);
    
    pthread_cond_wait(&nouveau, &mutex);
    
    printf("Placer un bocal\n");
    
	pthread_cond_signal(&cbocal);
  
    
    pthread_cond_wait(&cvalve, &mutex);

    printf("Enlever bocal\n\n");
    
    pthread_cond_signal(&nouveau);
    
    pthread_mutex_unlock(&mutex);
    //pthread_exit(NULL);

}

void * bocal2() {
    pthread_mutex_lock(&mutex);
    
    //pthread_cond_wait(&nouveau, &mutex);
    
    printf("Placer un bocal\n");
    
	pthread_cond_signal(&cbocal);
  
    
    pthread_cond_wait(&cvalve, &mutex);

    printf("Enlever bocal\n\n");
    
    //pthread_cond_signal(&nouveau);
    
    pthread_mutex_unlock(&mutex);
    //pthread_exit(NULL);

}

void * valve() {
    pthread_mutex_lock(&mutex);
   
	while(1) {
		
	pthread_cond_wait(&cbocal, &mutex);

    printf("Ouverture valve\n");
    
    for (int i = 0; i < 2;i++){
    //printf("A ");
    pthread_cond_signal(&cclock);
    
    pthread_cond_wait(&attendre, &mutex);
    //printf("B ");
	}
    printf("Fermeture valve\n");
    pthread_cond_signal(&cvalve); 
		
	}
	
    
    pthread_mutex_unlock(&mutex);
    //pthread_exit(NULL);

}


void * clockt() {
    pthread_mutex_unlock(&mutex);
    
	while(1) {
		
	pthread_cond_wait(&cclock, &mutex);
    
    printf("Horloge lancée\n");
    sleep(2);
    printf("Horloge finie\n");
    pthread_cond_signal(&attendre);
    
	}

    pthread_mutex_unlock(&mutex);
    //pthread_exit(NULL);
}


int main(int argc, char const *argv[]) {

  //pthread_mutex_init(&mutex, NULL); //Initialisation du mutex
  
	if (pthread_mutex_init(&mutex, NULL) != 0) {
		printf("mutex failed");
	}else {
		printf("mutex initialized!");
	}
  //for (int j = 0; j < N; j++) {
    printf("\n");
   // printf("Bocal : %i\n", j);

    
   
    pthread_create(&thorloge, NULL, clockt, NULL);
    pthread_create(&tvalve, NULL, valve, NULL);
    //sleep(1);
   // for (int i = 0; i < 3; i++ ) {
		 pthread_create(&tbocal[0], NULL, bocal, NULL);
		 //sleep(1);
		 pthread_create(&tbocal[1], NULL, bocal, NULL);
		 
		 pthread_create(&tbocal[2], NULL, bocal, NULL);
		 //sleep(1);
		 pthread_create(&tbocal[3], NULL, bocal, NULL);
		 
		 
		 sleep(1);
		 pthread_cond_signal(&nouveau);
   // }
    //pthread_exit(NULL);

    /*for (int i = 0; i < 3; i++ )*/ pthread_join(tbocal[0], NULL);
    //pthread_join(tvalve, NULL);
    //pthread_join(thorloge, NULL);
	pthread_join(tbocal[1], NULL);
	
	pthread_join(tbocal[2], NULL);
	
	pthread_join(tbocal[3], NULL);

    //sleep(3);
 // }
  
  
  pthread_mutex_destroy(&mutex);

  printf("\nTerminé\n");


  return 0;
  //pthread_exit(NULL);
}
