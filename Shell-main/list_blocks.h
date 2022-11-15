#ifndef SHELL_LIST_BLOCKS_H
#define SHELL_LIST_BLOCKS_H

#include <stdbool.h>
#include <stddef.h>
#include <sys/shm.h>
#define MAX 1024

typedef struct {
    void *address;
    size_t size;
    char time[19];
    char allocation_type[13];
    key_t key;
    char file_name[MAX];
    int file_descriptor;
    int index;
} tItemlM;

typedef struct nodeM *tPosLM;

struct nodeM {

    tItemlM data;
    tPosLM next;
};

typedef tPosLM tListM;


void CreateEmptyListM(tListM *L);

bool isEmptyListM(tListM L);

bool InsertElementM(tListM *L, void*address, size_t size, char allocation_type[], int key, char file_name[], int file_descriptor);

void RemoveElementM(tListM *L, tPosLM p);

void printListM(tListM L, char memType);

void clearListM(tListM *L);

tItemlM selectAtPositionM(int index, tListM L);

void removeHeadM(tListM *L);

int retLastIndexM(tListM L);

size_t size_of_shared(tListM L, key_t key);


#endif //SHELL_LIST_BLOCKS_H


