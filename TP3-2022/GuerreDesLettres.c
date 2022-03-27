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
//...
bool flag_de_fin = false;
char* tampon = NULL;

int nbrLettresProd = 0;
int nbrLettresCons = 0;

sem_t* libre;
sem_t* occupe; 
sem_t* mutex;

// fonction exécutée par les producteurs
void* producteur(void* pid) {
   int* id = (int*) pid;
   int nbr = 0;
   srand(time(NULL));

   while (1) {
      sem_wait(libre);
      sem_wait(mutex);
      char lettre = 'A' + rand() % 26;
      for (int i = 0; i < sizeof(tampon); i++) {
         if (tampon[i] == ' ') {
            tampon[i] = lettre;
            nbr++;
            nbrLettresProd++;
            break;
         }
      }
      sem_post(mutex);
      sem_post(libre);
      if (flag_de_fin) {
         printf("Producteur %d a produit %d lettres\n", *id, nbr);
         break;
      }
   }
   return NULL;
}

// fonction exécutée par les consommateurs
void* consommateur(void *cid) {
   int* id = (int*) cid;
   int nbr = 0;

   while(1) {
      sem_wait(occupe);
      sem_wait(mutex);
      for (int i = 0; i < sizeof(tampon); i++) {
         if (tampon[i] != ' ') {
            tampon[i] = ' ';
            nbr++;
            nbrLettresCons++;
            break;
         }
      }
      sem_post(mutex);
      sem_post(occupe);
      // for (int i = 0; i < sizeof(tampon); i++) {
      //    if (tampon[i] == 0) {
      //       printf("Consommateur %d a consommer %d lettres\n", *id, nbr);
      //       break;
      //    }
      // }
      if (flag_de_fin) {
         printf("Consommateur %d a consommé %d lettres\n", *id, nbr);
         break;
      }
   }
   
   return NULL;
}

void* signalHandler() {
   flag_de_fin = !flag_de_fin;
}

// fonction main
int main(int argc, char* argv[]) {
   /* Les paramètres du programme sont, dans l’ordre :
      le nombre de producteurs, le nombre de consommateurs
      et la taille du tampon.*/
   int nbPartage = 0;
   int nbProducteurs = atoi(argv[1]);
   int nbConsommateurs = atoi(argv[2]);
   tampon = calloc(atoi(argv[3]), sizeof(char));

   libre = calloc(1, sizeof(sem_t));
   sem_init(libre, nbPartage, nbProducteurs);

   occupe = calloc(1, sizeof(sem_t));
   sem_init(occupe, nbPartage, nbConsommateurs);

   mutex = calloc(1, sizeof(sem_t));
   sem_init(mutex, nbPartage, 1);

   for (int i = 0; i < sizeof(tampon); i++) {
      tampon[i] = ' ';
   }

   pthread_t threadProd_id[nbProducteurs];
   pthread_t threadProd_args[nbProducteurs];

   pthread_t threadCons_id[nbConsommateurs];
   pthread_t threadCons_args[nbConsommateurs];

   for (int i = 0; i < nbProducteurs; i++) {
      threadProd_args[i] = i;
      pthread_create(&threadProd_id[i], NULL, producteur, (void*) &threadProd_args[i]);
   }

   for (int i = 0; i < nbConsommateurs; i++) {
      threadCons_args[i] = i;
      pthread_create(&threadCons_id[i], NULL, consommateur, (void*) &threadCons_args[i]);
   }

   signal(SIGALRM, (void*) signalHandler);
   alarm(1);

   for (int i = 0; i < nbProducteurs; i++) {
      pthread_join(threadProd_id[i], NULL);
   }

   for (int i = 0; i < nbConsommateurs; i++) {
      // tampon[i] = 0;
      pthread_join(threadCons_id[i], NULL);
   }

   printf("==> Nombre total de lettres produites: %d\n", nbrLettresProd);
   printf("==> Nombre total de lettres consommées: %d\n", nbrLettresCons);

   return 0;
}
