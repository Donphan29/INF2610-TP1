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

// fonction exécutée par les producteurs
void* producteur(void* pid) {
   int* id = (int*) pid;
   while (1) {
      printf("here\n");



      if (flag_de_fin) {
         printf("Producteur %d a produit ...\n", *id);
         break;
      }
   }
   
   return NULL;
}
// fonction exécutée par les consommateurs
void* consommateur(void *cid) {
   printf("incons\n");
   
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
   int nbConsommateurs = nbConsommateurs;
   char tampon[atoi(argv[3])];

   sem_t* libre = calloc(1, sizeof(sem_t));
   sem_init(libre, nbPartage, nbProducteurs);

   sem_t* occupe = calloc(1, sizeof(sem_t));
   sem_init(occupe, nbPartage, nbConsommateurs);

   sem_t* mutex = calloc(1, sizeof(sem_t));
   sem_init(mutex, nbPartage, 1);

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
      tampon[i] = 0;
   }

   return 0;
}
