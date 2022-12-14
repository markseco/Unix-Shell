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
    char *userid;
    char *datetime;
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

void RemoveElementP(tListP *L, tPosLP p, bool endSignal, bool atExit);

void printListP(tListP L);

void printInfoByPid(int pid, tListP LP);

void clearListP(tListP *L, bool endSignal, bool atExit);

void removeHeadP(tListP *L);


#endif //SHELLC_LIST_PROCESS_H

