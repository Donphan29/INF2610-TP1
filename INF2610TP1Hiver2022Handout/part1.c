/*
 * processlab - part1.c
 * 
 * École polytechnique de Montréal, Hiver 2022
 * Maxence Lefebvre, XXXX 
 * Huy-Don Phan, 2074318
 * Section de laboratoire : 02 (B2)
 * 
 */


#include "./libprocesslab/libprocesslab.h"

void question1(int choix) {
    if (choix == 1) {
        printf("75dbcb01f571f1c32e196c3a7d27f62e (printed using printf)");
        write(STDOUT_FILENO, "75dbcb01f571f1c32e196c3a7d27f62e (printed using write)\n", strlen("75dbcb01f571f1c32e196c3a7d27f62e (printed using write)\n"));
        printf("\n");
    }

    else if (choix == 2) {
        setvbuf(stdout, NULL, _IONBF, 1024);
        printf("75dbcb01f571f1c32e196c3a7d27f62e (printed using printf)");
        write(STDOUT_FILENO, "75dbcb01f571f1c32e196c3a7d27f62e (printed using write)\n", strlen("75dbcb01f571f1c32e196c3a7d27f62e (printed using write)\n"));
        printf("\n");
    }
}