//
// Created by marcos on 11/18/22.
//

#ifndef SHELLC_HEAD_LIST_H
#define SHELLC_HEAD_LIST_H

#include <stdbool.h>

#define MAX 1024

typedef struct {
    char *data;
    int index;
} tIteml;

typedef struct node *tPosL;

struct node {

    tIteml data;
    tPosL next;
};

typedef tPosL tList;


void CreateEmptyList(tList *L);

bool isEmptyList(tList L);

bool InsertElement(tList *L, char *data);

void RemoveElement(tList *L, tPosL p);

void printList(tList L, int num);

void clearList(tList *L);

tIteml selectAtPosition(int index, tList L);

void removeHead(tList *L);

int retLastIndex(tList L);

#endif //SHELLC_HEAD_LIST_H
