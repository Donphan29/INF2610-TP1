/*
 *  libprocesslab.c
 */


#include "libprocesslab.h"

struct seg_t * pseg;

struct seg_t* allocateSharedMemory(long size)
{
    int fd = shm_open("a", O_RDWR|O_CREAT, 0600 );  // créer un segment
    if (fd==-1) { _exit(1); }
    off_t taille = sizeof(struct seg_t) * size;
    ftruncate(fd, taille);
    // rattacher le segment à l’espace d’adressage du processus appelant
    struct seg_t *pa = (struct seg_t *) mmap(NULL, taille,PROT_READ|PROT_WRITE, MAP_SHARED, fd,0);
    close(fd);
    if(pa==NULL) { _exit(1);}
    for(fd=0; fd<size; fd++) {pa[fd].procLevel = 0; pa[fd].procNum=0; pa[fd].pid=0; pa[fd].ppid=0;}
    return pa;
}

void registerProc(int procLevel, int procNum, pid_t pid, pid_t ppid)
{       int i;
        if(procLevel<0 || procLevel>2) return;
        if(procLevel==1 && (procNum<1 || procNum>2)) return;
        if(procLevel==2 && (procNum<1 || procNum>7)) return;
        if(procLevel==0)
        { pseg= allocateSharedMemory(10);i=0;}
        if(procLevel==1) i=procNum;
        if(procLevel==2) i=procLevel+procNum;
         pseg[i].procLevel = procLevel; pseg[i].procNum=procNum; pseg[i].pid=pid; pseg[i].ppid=ppid;
}


void printProcRegistrations()
{  int i;
        for(i=0; i<10; i++)
    printf("procLevel=%d, procNum=%d,  pid=%d, ppid=%d \n", pseg[i].procLevel,  pseg[i].procNum, pseg[i].pid, pseg[i].ppid);
    munmap(pseg, 10*sizeof(struct seg_t));
}

