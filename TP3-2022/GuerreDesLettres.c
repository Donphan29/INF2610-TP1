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
#include <string.h>


/* Variables globales */
bool flagDeFin = false;
char* tampon = NULL;


/* Sémaphore qui indique si le tampon est libre pour une production
 * ou qui met en attente le producteur lorsque ce dernier est plein
 */
sem_t* libre;


/* Sémaphore qui indique si le tampon est occupé pour une consommation
 * ou qui met en attente le consommateur lorsque ce dernier est vide
 */
sem_t* occupe;


/* Sémaphore qui asure l'accès exclusif aux variables partagées 
 * comme le tampon, le nombre total de lettres poduites ou consommées
 */
sem_t* mutex;


/* Fonction exécutée par les producteurs */
void* producteur(void* pid) {
   int* id = (int*) pid;
   int nProductions = 0;
   srand(time(NULL));

   while (1) {
      sem_wait(libre);
      sem_wait(mutex);
      char lettre = 'A' + rand() % 26;

      for (int i = 0; i < sizeof(tampon); i++) {
         if (tampon[i] == 1) {
            tampon[i] = lettre;
            nProductions++;
            break;
         }
      }

      sem_post(mutex);
      sem_post(libre);

      if (flagDeFin) {
         printf("Producteur %d a produit %d lettres\n", *id, nProductions);
         break;
      }
   }
   int* res = malloc(sizeof(int));
   *res = nProductions;
   return (void*) res;
}


/* Fonction exécutée par les consommateurs */
void* consommateur(void *cid) {
   int* id = (int*) cid;
   int nConsommations = 0;
   bool fin = false;

   while(1) {
      sem_wait(occupe);
      sem_wait(mutex);

      for (int i = 0; i < sizeof(tampon); i++) {
         if (tampon[i] >= 'A' && tampon[i] <='Z') {
            tampon[i] = 1;
            nConsommations++;
         }
         if (tampon[i] == 0 && !fin){
            tampon[i] = 1;
            fin = true;
         }
      }

      sem_post(mutex);
      sem_post(occupe);

      if (fin) {
         printf("Consommateur %d a consommé %d lettres\n", *id, nConsommations);
         break;
      }
   }
   int* res = malloc(sizeof(int));
   *res = nConsommations;
   return (void*) res;
}


/* Fonction qui inverse la valeur du flag de fin après un SIGALRM */
void* signalHandler() {
   flagDeFin = !flagDeFin;
}


/* Fonction main */
int main(int argc, char* argv[]) {
   /* Les paramètres du programme sont, dans l’ordre :
      le nombre de producteurs, le nombre de consommateurs
      et la taille du tampon.*/
   int nPartage = 0;
   int nProducteurs = atoi(argv[1]);
   int nConsommateurs = atoi(argv[2]);
   int nLettresProduites = 0;
   int nLettresConsommees = 0;

   /* Initialisation du tampon */
   tampon = calloc(atoi(argv[3]), sizeof(char));
   memset(tampon, 1, atoi(argv[3])*sizeof(int));

   /* Initialisation des sémaphores */
   libre = calloc(1, sizeof(sem_t));
   sem_init(libre, nPartage, nProducteurs);

   occupe = calloc(1, sizeof(sem_t));
   sem_init(occupe, nPartage, nConsommateurs);

   mutex = calloc(1, sizeof(sem_t));
   sem_init(mutex, nPartage, 1);


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
      int* produit;
      pthread_join(idThreadsProducteur[i], (void**) &produit);
      nLettresProduites += *produit;
   }

   /* Ajout de ) pour arreter les threads consommateurs*/
   for (int i = 0; i < nConsommateurs; i++) {
      if (tampon[i] == 1)
         tampon[i] = 0;
      else
         i--;
      
   }

   /* Mise en attente des threads consommateurs */
   for (int i = 0; i < nConsommateurs; i++) {
      int* cons;
      pthread_join(idThreadsConsommateur[i], (void**) &cons);
      nLettresConsommees += *cons;
   }


   /* Affichage des lettres totales produites et consommées */
   printf("--> Nombre total de lettres produites: %d\n", nLettresProduites);
   printf("--> Nombre total de lettres consommées: %d\n", nLettresConsommees);

   return 0;
}
