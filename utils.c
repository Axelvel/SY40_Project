#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/sem.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <pthread.h>

 

#define IFLAGS (SEMPERM | IPC_CREAT)
#define SKEY   (key_t) IPC_PRIVATE //CLE CREE AU HASARD

#define SEMPERM 0600		 /* Permission */

/*********************************************************************/
/*  Pour Operation P et V 					     */

int sem_id ; //identificateur des sémaphores
struct sembuf sem_oper_P ;  /* Operation P */
struct sembuf sem_oper_V ;  /* Operation V */

/*********************************************************************/

pthread_cond_t nouveau;

int initsem(key_t semkey){

	int status = 0;
   	union semun {
		int val;
		struct semid_ds *stat;
		short * array;
	} ctl_arg;
    if ((sem_id = semget(semkey, 6, IFLAGS)) > 0) {//Création de 6 sémaphores

	    	short array[6] = {0, 0, 0, 0, 0, 0}; //initialisation des sémaphores à 0
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

void startProduction(int mode){
    
    if (mode == 0) {
        V(5);
    } else if (mode == 1) {
        pthread_cond_signal(&nouveau);
        printf("Threads");
    } else {
        printf("Error\n");
    }
}

