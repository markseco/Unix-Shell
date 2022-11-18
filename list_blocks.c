#include "list_blocks.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/mman.h>
#include <unistd.h>

#define LNULL NULL

bool createNodeM(tPosLM *p) {
    *p = malloc(sizeof(struct nodeM));
    return *p != NULL;
}

void CreateEmptyListM(tListM *L) {
    if (createNodeM(L)){
        (*L)->next = LNULL;
        (*L)->data.address = NULL;

    }
}

bool isEmptyListM(tListM L) {
    return (L->next == NULL);
}

void getMonthName(int month, char mon[]){
    switch (month)
    {
        case 1:
            strcpy(mon, "Jan");
            break;
        case 2:
            strcpy(mon, "Feb");
            break;
        case 3:
            strcpy(mon, "Mar");
            break;
        case 4:
            strcpy(mon, "Apr");
            break;
        case 5:
            strcpy(mon, "May");
            break;
        case 6:
            strcpy(mon, "Jun");
            break;
        case 7:
            strcpy(mon, "Jul");
            break;
        case 8:
            strcpy(mon, "Aug");
            break;
        case 9:
            strcpy(mon, "Sep");
            break;
        case 10:
            strcpy(mon, "Oct");
            break;
        case 11:
            strcpy(mon, "Nov");
            break;
        case 12:
            strcpy(mon, "Dec");
            break;
        default:
            break;
    }
}

void dateTime(char dateTime[]){
    //type of arithmetic time
    time_t pressent;
    char mon[3];

    //the function obtains the actual time
    time(&pressent);

    //converts the 'pressent' variable to a struct
    struct tm *local = localtime(&pressent);
    int day, month, hours, minutes;
    day = local->tm_mday;
    month = local->tm_mon + 1;
    hours = local->tm_hour;
    minutes = local->tm_min;


    getMonthName(month, mon);

    sprintf(dateTime, "%s  %d  %d:%d", mon, day, hours, minutes);
}

bool InsertElementM(tListM *L, void *address, size_t size, char allocation_type[], int key, char file_name[], int file_descriptor, int shmid) {
    // Inserts an element in the List
    tPosLM n;

    /*we check there is enough memory to insert the new element
     if there is not enough memory we return false, otherwise we create a new nodeM*/
    if (!createNodeM(&n))
        return false;
    /*New nodeM values insertion*/

    n->data.address = address;
    n->data.size = size;
    dateTime(n->data.time);
    strcpy(n->data.allocation_type, allocation_type);
    n->data.key = key;
    strcpy(n->data.file_name, file_name);
    file_descriptor = file_descriptor;
    n->data.shmid = shmid;
    n->data.index = 0;
    n->next = LNULL;

    //if the list is empty we create the head nodeM, which points to the first element of the list

    if ((*L)->next == LNULL)
        (*L)->next = n;
    else {
        //we insert the element at the end
        tPosLM p;
        for (p = *L; p->next != LNULL; p = p->next) {
            n->data.index++;
        }
        p->next = n;
    }

    return true;
}

void removeMalloc(tListM *LM, tPosLM p){

    tPosLM i;
    if (p->next == NULL) {
        for (i = *LM; i->next != p; i = i->next);
        i->next = NULL;
        free(p->data.address);
    } else {
        i = p->next;
        free(p->data.address);
        p->data.address = i->data.address;
        p->data = i->data;
        p->next = i->next;
        p = i;
    }

    free(p);

}

void removeKey(key_t key){

    key_t clave = key;
    int id;

    if (clave == IPC_PRIVATE)
    {
        return;
    }
    if ((id = shmget(clave, 0, 0777)) == -1)
    {
        return;
    }
    shmctl(id, IPC_RMID, NULL);

}

void removeShared(tListM *LM, tPosLM p, int fromListRemove){

    tPosLM i;
    if (p->next == NULL) {
        for (i = *LM; i->next != p; i = i->next);
        i->next = NULL;

        if(fromListRemove){

            if(shmdt(p->data.address) == -1)
            {
                printf("Imposible to deallocate the shared memory\n");
            }

            removeKey(p->data.key);

        }


    } else {
        i = p->next;

        if(fromListRemove){

            if(shmdt(p->data.address) == -1)
            {
                printf("Imposible to deallocate the shared memory\n");
            }

            removeKey(p->data.key);

        }

        p->data.address = i->data.address;
        p->data = i->data;
        p->next = i->next;
        p = i;
    }


    free(p);


}

void removeMmap(tListM *LM, tPosLM p, int fromListRemove){
    tPosLM i;
    if (p->next == NULL) {
        for (i = *LM; i->next != p; i = i->next);
        i->next = NULL;
        munmap(p->data.address, p->data.size);
    } else {
        i = p->next;
        munmap(p->data.address, p->data.size);
        p->data.address = i->data.address;
        p->data = i->data;
        p->next = i->next;
        p = i;
    }

    free(p);

}

void RemoveElementM(tListM *LM, tPosLM p, int fromListRemove) {

    //this function removes an element at a given position, it can't remove the head nodeM
    if (p == NULL)
        return;

    if (p == *LM) {
        return;
    }else if(!strcmp(p->data.allocation_type, "malloc")){
        removeMalloc(LM, p);
    }else if(!strcmp(p->data.allocation_type, "shared")){
        removeShared(LM, p, fromListRemove);
    }else if(!strcmp(p->data.allocation_type, "mmap")){
        removeMmap(LM, p, fromListRemove);
    }


}

void clearListM(tListM *L) {
    //it removes all the elements except the head nodeM
    tPosLM p = (*L)->next;

    while (p != NULL) {
        RemoveElementM(L, p, 1);
        p = (*L)->next;
    }

    (*L)->next = NULL;

}

void printListM(tListM L, char memType) {
    if(memType == 'M')
        printf("******List of blocks asignated malloc to the process ");
    else if(memType == 'S')
        printf("******List of blocks asignated shared to the process ");
    else if(memType == 'P')
        printf("******List of blocks asignated mmap to the process ");
    else
        printf("******List of blocks asignated to the process ");

    printf("%d\n", getpid());

    for(tPosLM p = L->next; p != NULL; p = p->next){
        if((strcmp("shared", p->data.allocation_type) == 0) && memType != 'M' && memType != 'P')
            printf("       %p %18ld  %s %s (key %d)\n", p->data.address, p->data.size, p->data.time, p->data.allocation_type, p->data.key);

        if(strcmp("malloc", p->data.allocation_type) == 0 && memType != 'S' && memType != 'P')
            printf("       %p %18ld  %s %s\n", p->data.address, p->data.size, p->data.time, p->data.allocation_type);

        if(strcmp("mmap", p->data.allocation_type) == 0 && memType != 'S' && memType != 'M')
            printf("       %p %18ld  %s %s (descriptor %d)\n", p->data.address, p->data.size, p->data.time, p->data.file_name, p->data.file_descriptor);
    }

}

tItemlM selectAtPositionM(int index, tListM L) {
    //it returns an element with a certain index
    if (!isEmptyListM(L)) {
        if (index == 0) {
            tPosLM p = L->next;
            return p->data;

        }
        for (tPosLM p = L->next; p->next != LNULL && p->data.index <= index; p = p->next) {

            if (index == p->data.index)
                return p->data;

        }

    }

    tItemlM notFound;
    notFound.index = -1;
    return notFound;
}

int retLastIndexM(tListM L) {
    tPosLM p = L;

    while (p->next != NULL) {
        p = p->next;
    }

    return p->data.index;

}

size_t size_of_shared(tListM L, key_t key){

    for(tPosLM p = L->next; p != NULL; p = p->next){
        if(p->data.key == key)
            return p->data.size;
    }

    return 0;
}

void removeHeadM(tListM *L) {
    if ((*L)->next == NULL) {
        free(*L);
    } else
        printf("This can only remove the head nodeM not the whole list.");
}