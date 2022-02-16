/*
 * libprocesslab.h
*/

#ifndef _LIBPROCESSLAB_H
#define _LIBPROCESSLAB_H
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/errno.h>
#include <stdint.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/types.h>

struct seg_t { int procLevel; int procNum; pid_t pid; pid_t ppid; };

struct seg_t* allocateSharedMemory(long size);
void registerProc(int procLevel, int procNum, pid_t pid, pid_t ppid);
void printProcRegistrations();

#endif
