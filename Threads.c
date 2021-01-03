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
pthread_cond_t ouvrir_valve, fermer_valve;

pthread_t tbocal;
pthread_t tvalve;
pthread_t thorloge;


void * bocal() {

  printf("Placer un bocal\n");

  printf("Enlever bocal\n");
  pthread_exit(NULL);

}

void * valve() {

  printf("Ouverture valve\n");

  printf("Fermeture valve\n");

  pthread_exit(NULL);

}

void * horloge(int i) {
  printf("Horloge lancée\n");
  sleep(i);
  printf("Horloge finie\n");
  pthread_exit(NULL);
}


void * clockt() {
  printf("Horloge lancée\n");
  sleep(5);
  printf("Horloge finie\n");
  pthread_exit(NULL);
}


int main(int argc, char const *argv[]) {

  pthread_mutex_init(&mutex, NULL); //Initialisation du mutex




  for (int j = 0; j < N; j++) {
    printf("\n");
    printf("Bocal : %i\n", j);

    pthread_create(&tbocal, NULL, bocal, NULL);
    pthread_create(&tvalve, NULL, valve, NULL);
    pthread_create(&thorloge, NULL, clockt, NULL);

    pthread_join(tbocal, NULL);
    pthread_join(tvalve, NULL);
    pthread_join(thorloge, NULL);

    sleep(3);
  }

  printf("\nTerminé\n");


  // creation de la thread valve
  //pthread_create(tid+NbTh,0,(void *(*)())fonc_coiffeur,(void*)NbTh);

  //creation des threads clients
	 //for(num=0;num<NbTh;num ++){
		// pthread_create(tid+num,0,(void *(*)())fonc_client,(void*)num);
		// usleep(100000);
		// }




	//attend la fin de toutes les threads clients
	// for(num=0;num<NbTh;num ++)
	// 	pthread_join(tid[num],NULL);


  return 0;
}
