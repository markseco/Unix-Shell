#include "list_blocks.h"
#include "libraries-headers.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LNULL NULL

bool createNodeM(tPosLM *p) {
    *p = malloc(sizeof(struct nodeM));
    return *p != NULL;
}

void CreateEmptyListM(tListM *L) {
    if (createNodeM(L))
        (*L)->next = LNULL;
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

bool InsertElementM(tListM *L, void *address, size_t size, char allocation_type[], int key, char file_name[], char file_descriptor[]) {
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
    strcpy(n->data.file_descriptor, file_descriptor);  
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


void RemoveElementM(tListM *L, tPosLM p) {
    tPosLM i;
    //this function removes an element at a given position, it can't remove the head nodeM
    if (p == NULL)
        return;

    if (p == *L) {
        return;
    } else if (p->next == NULL) {
        for (i = *L; i->next != p; i = i->next);
        i->next = NULL;
    } else {
        i = p->next;
        p->data = i->data;     
        p->next = i->next;
        p = i;
    }
    
    free(p->data.address);
   
    free(p);
}

/*
void print_mallocs(tListM L){
    printf("******List of blocks asignated malloc to the process %d\n", getpid());
    for(tPosLM p = L; p != NULL; p = p->next){
        if(strcmp("malloc", p->data.allocation_type) == 0)
            printf("       %d        %d  %s %s\n", p->data.address, p->data.size, p->data.time, p->data.allocation_type);
    }
}

void print_shareds(tListM L){
    printf("******List of blocks asignated shared to the process %d\n", getpid());
    for(tPosLM p = L; p != NULL; p = p->next){
        if(strcmp("shared", p->data.allocation_type) == 0)
            printf("       %d        %d  %s %s (key %d)\n", p->data.address, p->data.size, p->data.time, p->data.allocation_type, p->data.key);
    }
}*/


void printListM(tListM L, char memType) {
    if(memType == 'M')
        printf("******List of blocks asignated malloc to the process ");
    else if(memType == 'S')
        printf("******List of blocks asignated shared to the process ");
    else 
        printf("******List of blocks asignated to the process ");
    
    printf("%d\n", getpid());

    for(tPosLM p = L->next; p != NULL; p = p->next){
        if((strcmp("shared", p->data.allocation_type) == 0) && memType != 'M')
            printf("       %p        %ld  %s %s (key %d)\n", p->data.address, p->data.size, p->data.time, p->data.allocation_type, p->data.key);

        if(strcmp("malloc", p->data.allocation_type) == 0 && memType != 'S')
            printf("       %p        %ld  %s %s\n", p->data.address, p->data.size, p->data.time, p->data.allocation_type);
    }

}

void clearListM(tListM *L) {
    //it removes all the elements except the head nodeM
    tPosLM p = (*L)->next;

    while (p != NULL) {
        RemoveElementM(L, p);
        p = (*L)->next;
    }

    (*L)->next = NULL;

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


void removeHeadM(tListM *L) {
    if ((*L)->next == NULL) {
        free(*L);
    } else
        printf("This can only remove the head nodeM not the whole list.");
}

