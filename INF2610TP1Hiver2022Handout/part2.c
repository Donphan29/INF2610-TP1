/*
 * processlab - part2.c
 * 
 * Ecole polytechnique de Montreal,Hiver  2022
 * vos noms, prénoms et matricules
*/

#include "libprocesslab/libprocesslab.h"


// TODO
// Si besoin, ajouter ici les directives d'inclusion
// -------------------------------------------------

// -------------------------------------------------

void question2( )
{
    // TODO
    int nChilds = 0;
    int temp, temp1;
    registerProc(0, 0, getpid(), getppid());

    if (fork() == 0) {
        registerProc(1, 1, getpid(), getppid());
        if (fork() == 0) {
            registerProc(2, 1, getpid(), getppid());
            exit(1);
        }
        wait(&temp);
        nChilds += WEXITSTATUS(temp);
        if (fork() == 0) {
            registerProc(2, 2, getpid(), getppid());
            exit(1);
        }
        wait(&temp);
        nChilds += WEXITSTATUS(temp);
        if (fork() == 0) {
            registerProc(2, 3, getpid(), getppid());
            exit(1);
        }
        wait(&temp);
        nChilds += WEXITSTATUS(temp);
        if (fork() == 0) {
            registerProc(2, 4, getpid(), getppid());
            exit(1);
        }
        wait(&temp);
        nChilds += WEXITSTATUS(temp);
        nChilds++;
        exit(nChilds);
    }
    wait(&temp);
    if (fork() == 0) {
        registerProc(1, 2, getpid(), getppid());
        if (fork() == 0) {
            registerProc(2, 5, getpid(), getppid());
            exit(1);
        }
        wait(&temp);
        nChilds += WEXITSTATUS(temp);
        if (fork() == 0) {
            registerProc(2, 6, getpid(), getppid());
            exit(1);
        }
        wait(&temp);
        nChilds += WEXITSTATUS(temp);
        if (fork() == 0) {
            registerProc(2, 7, getpid(), getppid());
            exit(1);
        }
        wait(&temp);
        nChilds += WEXITSTATUS(temp);
        nChilds++;
        exit(nChilds);
    }
    wait(&temp1);
    nChilds += WEXITSTATUS(temp);
    nChilds += WEXITSTATUS(temp1);
    printf("Nombre de fils : %d \n", nChilds);
    printProcRegistrations();
    execlp("ls", "ls", "-l", NULL);
}

