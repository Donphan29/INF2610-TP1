/*
 * TP3 - GuerreDesLettres.c
 * 
 * École Polytechnique de Montréal, Hiver 2022
 * Maxence Lefebvre, 2085581
 * Huy-Don Phan, 2074318
 * Section de laboratoire : 02 (B2)
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <semaphore.h>
#include <pthread.h>
#include <stdbool.h>
#include <unistd.h>


/* Variables globales */
bool flagDeFin = false;
char* tampon = NULL;
int nLettresProduites = 0;
int nLettresConsommees = 0;
int ip = 0;
int ic = 0;
int tailleTampon;

/* Sémaphore qui indique si le tampon est libre pour une production
 * ou qui met en attente le producteur lorsque ce dernier est plein
 */
sem_t libre;

/* Sémaphore qui indique si le tampon est occupé pour une consommation
 * ou qui met en attente le consommateur lorsque ce dernier est vide
 */
sem_t occupe;

/* Sémaphore qui asure l'accès exclusif aux variables partagées 
 * comme le tampon, ip et ic
 */
sem_t mutex;

/* Fonction exécutée par les producteurs */
void* producteur(void* pid) {
   int* id = (int*) pid;
   int nProductions = 0;
   srand(time(NULL));

   while (1) {
      sem_wait(&libre);
      sem_wait(&mutex);
      char lettre = 'A' + rand() % 26;

      tampon[ip] = lettre;
      ip = (ip + 1) % tailleTampon;
      nProductions++;
      nLettresProduites++;

      sem_post(&mutex);
      sem_post(&occupe);

      if (flagDeFin) {
         printf("Producteur %d a produit %d lettres\n", *id, nProductions);
         pthread_exit(NULL);
      }
   }
   pthread_exit(NULL);
}

/* Fonction exécutée par les consommateurs */
void* consommateur(void *cid) {
   int* id = (int*) cid;
   int nConsommations = 0;

   while(1) {
      sem_wait(&occupe);
      sem_wait(&mutex);

      if (tampon[ic] == 0) {
         ic = (ic + 1) % tailleTampon;
         printf("Consommateur %d a consommé %d lettres\n", *id, nConsommations);
         sem_post(&mutex);
         sem_post(&libre);
         pthread_exit(NULL);
      };

      tampon[ic] = ' ';
      ic = (ic + 1) % tailleTampon;
      nConsommations++;
      nLettresConsommees++;

      sem_post(&mutex);
      sem_post(&libre);
   }
   pthread_exit(NULL);
}

/* Fonction qui inverse la valeur du flag de fin après un SIGALRM */
void* signalHandler() {
   flagDeFin = !flagDeFin;
}

/* Fonction main */
int main(int argc, char* argv[]) {
   int nPartage = 0;
   int nProducteurs = atoi(argv[1]);
   int nConsommateurs = atoi(argv[2]);
   tailleTampon = atoi(argv[3]);
   tampon = calloc(tailleTampon, sizeof(char));

   /* Initialisation des sémaphores */
   sem_init(&libre, nPartage, tailleTampon);
   sem_init(&occupe, nPartage, 0);
   sem_init(&mutex, nPartage, 1);

   /* Initialisation des id et des arguments des threads producteurs et consommateurs */
   pthread_t idThreadsProducteur[nProducteurs];
   pthread_t argsThreadsProducteur[nProducteurs];

   pthread_t idThreadsConsommateur[nConsommateurs];
   pthread_t argsThreadsConsommateurs[nConsommateurs];

   /* Création des threads producteurs et consommateurs */
   for (int i = 0; i < nProducteurs; i++) {
      argsThreadsProducteur[i] = i;
      pthread_create(&idThreadsProducteur[i], NULL, producteur, (void*) &argsThreadsProducteur[i]);
   }

   for (int i = 0; i < nConsommateurs; i++) {
      argsThreadsConsommateurs[i] = i;
      pthread_create(&idThreadsConsommateur[i], NULL, consommateur, (void*) &argsThreadsConsommateurs[i]);
   }

   /* Lancement d'une alarme d'une seconde */
   signal(SIGALRM, (void*) signalHandler);
   alarm(1);

   /* Mise en attente des threads producteurs */
   for (int i = 0; i < nProducteurs; i++) {
      pthread_join(idThreadsProducteur[i], NULL);
   }

   /* Ajout des 0 dans le tampon */
   int tempIc = ic;
   for (int i = 0; i < nConsommateurs; tempIc = (tempIc + 1) % tailleTampon, i++) {
      sem_wait(&libre);
      sem_wait(&mutex);
      tampon[tempIc] = 0;
      sem_post(&mutex);
      sem_post(&occupe);
   }

   /* Mise en attente des threads consommateurs */
   for (int i = 0; i < nConsommateurs; i++) {
      pthread_join(idThreadsConsommateur[i], NULL);
   }

   /* Affichage des lettres totales produites et consommées */
   printf("Nombre total de lettres produites: %d\n", nLettresProduites);
   printf("Nombre total de lettres consommées: %d\n", nLettresConsommees);

   return 0;
}
