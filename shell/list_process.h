#ifndef SHELLC_LIST_PROCESS_H
#define SHELLC_LIST_PROCESS_H

#include <stdio.h>
#include <stdbool.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>
#define MAX 1024

typedef struct {
    int pid;
    int userid;
    int signal;
    char status[11];
    char datetime[21];
    char commandLine[MAX/2];
    
} tItemlP;

typedef struct nodeP *tPosLP;

struct nodeP {

    tItemlP data;
    tPosLP next;
};

typedef tPosLP tListP;

void CreateEmptyListP(tListP *L);

bool isEmptyListP(tListP L);

bool InsertElementP(tListP *L, int pid, char *tr[]);

void RemoveElementP(tListP *L, tPosLP p);

void removeAtPidP(int pid, tListP *LP);

void printListP(tListP L);

void printInfoByPid(int pid, tListP LP);

bool processExist(int pid, tListP L);

void clearListP(tListP *L);

void clearListTerminatedSignaledP(tListP *L, bool isSignal);

void removeHeadP(tListP *L);


#endif //SHELLC_LIST_PROCESS_H

