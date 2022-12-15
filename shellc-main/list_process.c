#include <time.h>
#include <unistd.h>
#include <pwd.h>
#include <sys/resource.h>
#include "list_process.h"



bool createNodeP(tPosLP *p) {
    *p = malloc(sizeof(struct nodeP));
    return *p != NULL;
}

void CreateEmptyListP(tListP *L) {
    if (createNodeP(L)){
        (*L)->next = NULL;
    
    }
}

bool isEmptyListP(tListP L) {
    return (L->next == NULL);
}

void dateTimeP(char dateTime[]){
    //type of arithmetic time
    time_t pressent;

    //the function obtains the actual time
    time(&pressent);

    //converts the 'pressent' variable to a struct
    struct tm *local = localtime(&pressent);
    int day, month, hours, minutes, year, seconds;
    day = local->tm_mday;
    month = local->tm_mon + 1;
    hours = local->tm_hour;
    minutes = local->tm_min;
    year = local->tm_year + 1900;
    seconds = local->tm_sec;

    sprintf(dateTime, "%d/%d/%d  %d:%d:%d", year, month, day, hours, minutes, seconds);
}

int commandLineLength(char *tr[]){
    int i = 0;
    int len = 0;

    while(tr[i] != NULL){

        len += strlen(tr[i]) + 1;

        i++;
    }

    return (len + 1);
}

bool InsertElementP(tListP *L, int pid, char *tr[]){
    tPosLP n;
    int i = 0;

    if(!createNodeP(&n))
        return false;

    n->next = NULL;
    n->data.pid = pid;
    n->data.signal = 0;
    strcpy(n->data.status, "ACTIVE  ");
    strcpy(n->data.commandLine, " ");
    dateTimeP(n->data.datetime);

    n->data.userid = getuid();

    
    
    while(tr[i] != NULL){
        strcat(n->data.commandLine, tr[i]);
        strcat(n->data.commandLine, " ");
        i++;
    }



    tPosLP p;
    for(p = (*L) ; p->next != NULL; p = p->next);
    p->next = n;

    return true;
}


void RemoveElementP(tListP *L, tPosLP p){
    tPosLP i;
    //this function removes an element at a given position, it can't remove the head node
    if (p == NULL)
        return;

    if (p == *L) {
        return;
    } else if (p->next == NULL) {
        for (i = *L; i->next != p; i = i->next);
        
        i->next = NULL;
    } else {
    
        i = p->next;

        p->data.pid = i->data.pid;
        p->data.userid = i->data.userid;
        p->data.signal = i->data.signal;
        strcpy(p->data.status, i->data.status);
        strcpy(p->data.datetime, i->data.datetime);
        strcpy(p->data.commandLine, i->data.commandLine);
        p->next = i->next;
        p = i;
    }
    
    
    
    free(p);
}

void removeAtPidP(int pid, tListP *LP){

    for(tPosLP p = (*LP)->next; p != NULL; p = p->next){
        if(p->data.pid == pid){
            RemoveElementP(LP, p);
            return;
        }
    }
}

struct SEN{
    char *nombre;
    int senal;
};

static struct SEN sigstrnum[]={   
	{"HUP", SIGHUP},
	{"INT", SIGINT},
	{"QUIT", SIGQUIT},
	{"ILL", SIGILL}, 
	{"TRAP", SIGTRAP},
	{"ABRT", SIGABRT},
	{"IOT", SIGIOT},
	{"BUS", SIGBUS},
	{"FPE", SIGFPE},
	{"KILL", SIGKILL},
	{"USR1", SIGUSR1},
	{"SEGV", SIGSEGV},
	{"USR2", SIGUSR2}, 
	{"PIPE", SIGPIPE},
	{"ALRM", SIGALRM},
	{"TERM", SIGTERM},
	{"CHLD", SIGCHLD},
	{"CONT", SIGCONT},
	{"STOP", SIGSTOP},
	{"TSTP", SIGTSTP}, 
	{"TTIN", SIGTTIN},
	{"TTOU", SIGTTOU},
	{"URG", SIGURG},
	{"XCPU", SIGXCPU},
	{"XFSZ", SIGXFSZ},
	{"VTALRM", SIGVTALRM},
	{"PROF", SIGPROF},
	{"WINCH", SIGWINCH}, 
	{"IO", SIGIO},
	{"SYS", SIGSYS},
/*senales que no hay en todas partes*/
#ifdef SIGPOLL
	{"POLL", SIGPOLL},
#endif
#ifdef SIGPWR
	{"PWR", SIGPWR},
#endif
#ifdef SIGEMT
	{"EMT", SIGEMT},
#endif
#ifdef SIGINFO
	{"INFO", SIGINFO},
#endif
#ifdef SIGSTKFLT
	{"STKFLT", SIGSTKFLT},
#endif
#ifdef SIGCLD
	{"CLD", SIGCLD},
#endif
#ifdef SIGLOST
	{"LOST", SIGLOST},
#endif
#ifdef SIGCANCEL
	{"CANCEL", SIGCANCEL},
#endif
#ifdef SIGTHAW
	{"THAW", SIGTHAW},
#endif
#ifdef SIGFREEZE
	{"FREEZE", SIGFREEZE},
#endif
#ifdef SIGLWP
	{"LWP", SIGLWP},
#endif
#ifdef SIGWAITING
	{"WAITING", SIGWAITING},
#endif
 	{NULL, -1}
};    /*fin array sigstrnum */

int ValorSenal(char * sen)  /*devuelve el numero de senial a partir del nombre*/ 
{ 
  int i;
  for (i=0; sigstrnum[i].nombre!=NULL; i++)
  	if (!strcmp(sen, sigstrnum[i].nombre))
		return sigstrnum[i].senal;
  return -1;
}

char *NombreSenal(int sen)  /*devuelve el nombre senal a partir de la senal*/ 
{			/* para sitios donde no hay sig2str*/
 int i;
  for (i=0; sigstrnum[i].nombre!=NULL; i++)
  	if (sen==sigstrnum[i].senal)
		return sigstrnum[i].nombre;

 if(sen == 0){
    return("000");
 }
 return ("SIGUNKNOWN");
}


void actualiceStateP(tPosLP p){
    int stat;
    
    if (waitpid(p->data.pid, &stat, WNOHANG | WUNTRACED | WCONTINUED)==p->data.pid) {
        if(WIFSTOPPED(stat)){
            strcpy(p->data.status,"STOPPED");
            p->data.signal = WSTOPSIG(stat);
        }else if(WIFSIGNALED(stat)){
            strcpy(p->data.status, "SIGNALED");
            p->data.signal = WTERMSIG(stat);
        }else if(WIFEXITED(stat)){
            strcpy(p->data.status, "FINISHED");
            p->data.signal = WEXITSTATUS(stat);
        }
    }
}



void printStatusProcess(tPosLP p){
    int prio;
    prio = getpriority(PRIO_PROCESS, p->data.pid);
    struct passwd *uname = NULL;
    char * username = (uname = getpwuid(p->data.userid)) == NULL ? "UNKNOWN" : uname->pw_name;
    
    actualiceStateP(p);

    printf("%d\t\t%s\tp=%d\t%s\t%s\t(%s)\t%s\n", p->data.pid, username, prio, p->data.datetime, p->data.status, NombreSenal(p->data.signal),p->data.commandLine);
}

void printListP(tListP L){
    for(tPosLP p = L->next; p!=NULL; p = p->next){
        printStatusProcess(p);
    }

}

void printInfoByPid(int pid, tListP LP){
    for(tPosLP p = LP->next; p != NULL; p = p->next){
        if(p->data.pid == pid){
            printStatusProcess(p);
            return;
        }
    }
    printf("ERROR: The process doesn't exist\n");
}


bool processExist(int pid, tListP L){
    for(tPosLP p = L->next; p != NULL; p = p->next){
        if(pid == p->data.pid){
            return true;
        }
    }

    return false;
}


void clearListP(tListP *L){
    //it removes all the elements except the head nodeM
    tPosLP p = (*L)->next;
    
    while (p != NULL) {
        RemoveElementP(L, p);
        p = (*L)->next;
    }

   
    (*L)->next = NULL;
}

void clearListTerminatedSignaledP(tListP *L, bool isSignal){
    tPosLP p = (*L)->next;
    while(p != NULL){
        actualiceStateP(p);
        if(isSignal){
            if(!strcmp(p->data.status, "SIGNALED")){
                RemoveElementP(L, p);
                p = (*L)->next;
            }else{
                p = p->next;
            }
        }else{
            if(!strcmp(p->data.status, "FINISHED")){
                RemoveElementP(L, p);
                p = (*L)->next;
            }else{
                p = p->next;
            }
        }
    
    }

    printListP(*L);

}



void removeHeadP(tListP *L){
    if ((*L)->next == NULL) {
        free(*L);
    } else
        printf("This can only remove the head nodeP not the whole list.");
}


