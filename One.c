#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/sem.h>
#include <errno.h>
#include <sys/types.h>
//#include <sys/ipc.h>
//#include <sys/msg.h>

#define IFLAGS (SEMPERM | IPC_CREAT)
#define SKEY   (key_t) IPC_PRIVATE //CLE CREE AU HASARD

#define SEMPERM 0600		 /* Permission */

#define N 2 //Nombre de bocaux à produire


/*********************************************************************/
/*  Pour Operation P et V 					     */

int sem_id ; //identificateur des sémaphores
struct sembuf sem_oper_P ;  /* Operation P */
struct sembuf sem_oper_V ;  /* Operation V */

/*********************************************************************/

int initsem(key_t semkey){

	int status = 0;
   	union semun {
		int val;
		struct semid_ds *stat;
		short * array;
	} ctl_arg;
    if ((sem_id = semget(semkey, 5, IFLAGS)) > 0) {//Création de 5 sémaphores

	    	short array[5] = {0, 0, 0, 0, 0}; //initialisation des sémaphores à 0
	    	ctl_arg.array = array;
	    	status = semctl(sem_id, 0, SETALL, ctl_arg); //Création du tableau de sémaphore
    }
   if (sem_id == -1 || status == -1) {
	perror("Erreur initsem");
	return (-1);
    } else return (sem_id);
}

void P(int semnum) { //Wait
	sem_oper_P.sem_num = semnum; //numero du semaphore
	sem_oper_P.sem_op  = -1 ; //valeur pour modifier le sempaphore
	sem_oper_P.sem_flg = 0 ; //sem_undo ?
	semop(sem_id,&sem_oper_P,1); //effectue l'opération sur le sémaphore
}

void V(int semnum) { //Wake
	sem_oper_V.sem_num = semnum;
	sem_oper_V.sem_op  = 1 ;
	sem_oper_V.sem_flg  = 0 ;
	semop(sem_id,&sem_oper_V,1);
}




void bocal(){
  if (!fork()) {

    printf("Placer un bocal\n");
    V(1);

    P(4);
    printf("Enlever bocal\n");

    exit(0);
  }



}

void valve() {


  if (!fork()) {

    P(1);
    printf("Ouverture valve\n");
		V(2);

    P(3);
    printf("Fermeture valve\n");
    V(4);

    exit(0);
  }
}


void horloge(int i){
  if (!fork()) {

    P(2);
    printf("Horloge lancée\n");
    sleep(i);
    printf("Horloge finie\n");
    V(3);

    exit(0);
  }
}



int main(int argc, char const *argv[]) {

  int i,semid; //Semid : identificateur des sémaphores
	initsem(SKEY); //On initialise le tableau de sémaphores

  for (int j = 0; j < N; j++) {
    printf("\n");
    printf("Bocal : %i\n", j);

    bocal();
    valve();
    horloge(5);

    for (i=1; i<=3; i++) wait(0);
    sleep(3);
  }

  //TODO: liberSem()

  return 0;
}
