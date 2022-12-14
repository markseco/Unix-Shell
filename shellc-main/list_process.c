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
        (*L)->data.datetime = NULL;
        (*L)->data.userid = NULL;
    
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
    struct passwd *uname = NULL;
    int i = 0;

    if(!createNodeP(&n))
        return false;

    n->next = NULL;
    n->data.pid = pid;
    n->data.datetime = malloc(sizeof(char) * 20);
    dateTimeP(n->data.datetime);

    n->data.userid = (uname = getpwuid(getuid())) == NULL ? "UNKNOWN" : uname->pw_name;
    
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

void RemoveElementP(tListP *L, tPosLP p, bool endSignal, bool atExit){
    tPosLP i;
    int status;
    //this function removes an element at a given position, it can't remove the head node
    if (p == NULL)
        return;

    if (p == *L) {
        return;
    } else if (p->next == NULL) {
        for (i = *L; i->next != p; i = i->next);
        waitpid(p->data.pid, &status, WNOHANG | WUNTRACED | WCONTINUED);

        if(!atExit){ //if we are exiting the program we delete all the elements of the list
            if(kill(p->data.pid, 0) == 0){
                return;
            }
            if(endSignal){//cheks for only eliminate the ones wiht and signal when asked for it

                if(!WIFSIGNALED(status)){//if its not signaled it doesn not delete it 
                    return;
                }
            }else if(!WIFEXITED(status)){ 
                return;
            }

        }
        
        i->next = NULL;
    } else {

        waitpid(p->data.pid, &status, WNOHANG | WUNTRACED | WCONTINUED);
        if(!atExit){ //if we are exiting the program we delete all the elements of the list
            if(kill(p->data.pid, 0) == 0){
                return;
            }
            if(endSignal){//cheks for only eliminate the ones wiht and signal when asked for it

                if(!WIFSIGNALED(status)){//if its not signaled it doesn not delete it 
                    return;
                }
            }else if(!WIFEXITED(status)){ 
                return;
            }

        }



        i = p->next;
        free(p->data.datetime);
        free(p->data.userid);
        p->data.pid = i->data.pid;
        p->data.datetime = i->data.datetime;
        p->data.userid = i->data.userid;
        strcpy(p->data.commandLine, i->data.commandLine);
        p->next = i->next;
        p = i;
    }
    
    free(p->data.datetime);
    free(p->data.userid);
    
    free(p);
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
 return ("SIGUNKNOWN");
}


void printStatusProcess(tPosLP p){
    char status[11];
    int stat, signal, prio;
    waitpid(p->data.pid, &stat, WNOHANG | WUNTRACED | WCONTINUED);
    prio = getpriority(PRIO_PROCESS, p->data.pid);
        
    if(kill(p->data.pid, 0) == 0){
        strcpy(status, "ACTIVO");
        signal = 0;
    }else if(WIFSTOPPED(stat)){
        strcpy(status,"STOPPED");
        signal = WSTOPSIG(stat);
    }else if(WIFSIGNALED(stat)){
        strcpy(status, "FINISHED");
        signal = WTERMSIG(stat);
    }else if(WIFEXITED(stat)){
        strcpy(status, "FINISHED");
        signal = WEXITSTATUS(stat);
    }

        printf("%d\t\t%s\tp=%d\t%s\t%s\t(%s)\t%s\n", p->data.pid, p->data.userid, prio, p->data.datetime, status, NombreSenal(signal),p->data.commandLine);
}

void printListP(tListP L){
    char status[11];
    int stat, signal, prio;

    
    
    for(tPosLP p = L->next; p!=NULL; p = p->next){
        waitpid(p->data.pid, &stat, WNOHANG | WUNTRACED | WCONTINUED);
        prio = getpriority(PRIO_PROCESS, p->data.pid);
        
        if(kill(p->data.pid, 0) == 0){
            strcpy(status, "ACTIVO");
            signal = 0;
        }else if(WIFSTOPPED(stat)){
            strcpy(status,"STOPPED");
            signal = WSTOPSIG(stat);
        }else if(WIFSIGNALED(stat)){
            strcpy(status, "FINISHED");
            signal = WTERMSIG(stat);
        }else if(WIFEXITED(stat)){
            strcpy(status, "FINISHED");
            signal = WEXITSTATUS(stat);
        }

        printf("%d\t\t%s\tp=%d\t%s\t%s\t(%s)\t%s\n", p->data.pid, p->data.userid, prio, p->data.datetime, status, NombreSenal(signal),p->data.commandLine);
    }
    

}


void deleteByPid(tListP LP){



}

void printInfoByPid(int pid, tListP LP){
    for(tPosLP p = LP->next; p != NULL; p = p->next){
        if(p->data.pid == pid){
            printStatusProcess(p);
            return;
        }
    }
}

void clearListP(tListP *L, bool endsignal, bool atExit){
    //it removes all the elements except the head nodeM
    tPosLP p = (*L)->next;
    tPosLP aux;
    while (p != NULL) {
        aux = p->next;
        RemoveElementP(L, p, endsignal, atExit);
        p = aux;
    }

    //(*L)->next = NULL;
}

void removeHeadP(tListP *L){
    if ((*L)->next == NULL) {
        free(*L);
    } else
        printf("This can only remove the head nodeP not the whole list.");
}


