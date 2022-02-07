/*
 * processlab - part2.c
 * 
 * École polytechnique de Montréal, Hiver 2022
 * Maxence Lefebvre, XXX 
 * Huy-Don Phan, 2074318
 * Section de laboratoire : 02 (B2)
 * 
 */

#include "libprocesslab/libprocesslab.h"

void question2( )
{
    int nChilds = 0;
    int temp = 0;
    registerProc(0, 0, getpid(), getppid());

    if (fork() == 0) {
        registerProc(1, 1, getpid(), getppid());

        if (fork() == 0) {
            registerProc(2, 1, getpid(), getppid());
            exit(1);
        }

        if (fork() == 0) {
            registerProc(2, 2, getpid(), getppid());
            exit(1);
        }

        if (fork() == 0) {
            registerProc(2, 3, getpid(), getppid());
            exit(1);
        }

        if (fork() == 0) {
            registerProc(2, 4, getpid(), getppid());
            exit(1);
        }

        while (wait(&temp) > 0) {
            nChilds += WEXITSTATUS(temp);
        }

        nChilds++;
        exit(nChilds);
    }

    if (fork() == 0) {
        registerProc(1, 2, getpid(), getppid());

        if (fork() == 0) {
            registerProc(2, 5, getpid(), getppid());
            exit(1);
        }

        if (fork() == 0) {
            registerProc(2, 6, getpid(), getppid());
            exit(1);
        }

        if (fork() == 0) {
            registerProc(2, 7, getpid(), getppid());
            exit(1);
        }

        while (wait(&temp) > 0) {
            nChilds += WEXITSTATUS(temp);
        }

        nChilds++;
        exit(nChilds);
    }

    while (wait(&temp) > 0) {
        nChilds += WEXITSTATUS(temp);
    }

    printf("Nombre de fils : %d\n", nChilds);
    printProcRegistrations();

    execlp("ls", "ls", "-l", NULL);
}