#define MAX 1024
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <sys/utsname.h>
#include "head_list.h"

void cmd_autores(char * tr []);
void cmd_carpeta(char * tr[]);
void cmd_ayuda(char *tr[]);
void cmd_pid(char * tr[]);
void cmd_fin(char * tr [], tList *L);
void cmd_hist(char * tr[], tList *L);
void cmd_command(char * tr[], tList *L);
void cmd_fecha(char * tr[]);
void cmd_infosis(char *tr[]);
void cmd_create(char *tr[]);
void ProcesarEntrada(char * tr[], tList * L);
int TrocearCadena(char * cadena, char * trozos[]);


//definition of the structure that is used to store the name of the command its use and the function that implements it
struct CMD{
    char *name;
    char * ayuda;
    void (*func)(char **);
};

struct CMD c[]={
        {"autores","Prints the names and logins of the program authors",cmd_autores},
        {"carpeta","Changes the current working directoyof the shell",cmd_carpeta},
        {"ayuda", "Displays a list of available commands", cmd_ayuda},
        {"pid","Prints the pid of the process executing the shell",cmd_pid},
        {"fecha", "It prints both the current date and the current time.\n fecha -d: prints the current date.\n fecha -h prints the current time", cmd_fecha},
        {"infosis", "Prints information on the machine runnig the shell", cmd_infosis},
        {"create", "create [-f] [name]	Crea un directorio o un fichero (-f)", cmd_create},
        {NULL, NULL,NULL},

};

void cmd_autores(char * tr []){
	
    if (tr[0] == NULL){
    //prints the names and the logins of the autors
        printf("Diego Dopazo Garcia\n");
        printf("diego.dopazo.garcia@udc.es\n");
        printf("\n");
        printf("Marcos Seco Anderson\n");
        printf("marcos.seco.anderson@udc.es\n");
    }
    else if(!strcmp(tr[0], "-l")){
    //prints the logins of the autors
        printf("diego.dopazo.garcia@udc.es\n");
        printf("marcos.seco.anderson@udc.es\n");
    }
    else if(!strcmp(tr[0], "-n")){
    //prints the names of the autors
        printf("Diego Dopazo Garcia\n");
        printf("Marcos Seco Anderson\n");
    }else
    	printf("Wrong parameter, try '-l' (for the logins) or '-n' (for the names) or without parameters (for both optiions)\n");

}

void cmd_pid(char * tr[]){
    if (tr[0] == NULL){
    //prints the process pid
        int pid = getpid();
        printf("Process pid: %d \n", pid);

    }else if(!strcmp(tr[0], "-p")){
    //prints the parent process' pid
        int ppid = getppid();
        printf("Parent process pid: %d \n", ppid);
    }else
    	printf("Wrong parameter, try '-p' for the parent's process pid or without parameters for the process' pid\n");
}



void cmd_carpeta(char * tr[]){
    char dir[MAX];
    if(tr[0]==NULL){
    //shows the current directory
        printf("%s", getcwd(dir, MAX));
        printf("\n");
    }
    else if(chdir(tr[0]) == -1){
        printf("Cannot change directory.\n");
        printf(" ");
    }

}

void cmd_ayuda(char *tr[]){

    if(tr[0] == NULL){
        //prints the name of the commands
        for(int i=0;c[i].name != NULL; i++) {
            printf("%s\n", c[i].name);
           }
            printf("hist\n");
            printf("comando\n");
    }else if(tr[0] != NULL){
        //prints the name of the commands and some information about them
        if(strcmp(tr[0], "hist") == 0){
        	printf("hist: shows the commands used, with '-c' clears the history of the commans used");
            	printf(", with a number 'n' shows the first 'n'  command\n");
            	return;
        }else if(strcmp(tr[0], "comando") == 0){
        	printf("comando 'n': repeats the command number 'n'.\n");
        	return;
        }
        
        
        for(int i=0;c[i].name != NULL; i++) {
        	if (strcmp(tr[0], c[i].name) == 0){
            		printf("%s : %s\n", c[i].name, c[i].ayuda);
            		return;
            	}
           }
           
            
    }else
    	printf("Wrong parameter try withot parameters for the names or with 'cmd-name' for some information also\n");
}

void cmd_fecha(char * tr[]){
    //type of arithmetic time
    time_t pressent;
    //the function obtains the actual time
    time(&pressent);

    //converts the 'pressent' variable to a struct
    struct tm * local = localtime(&pressent);

    if(tr[0] == NULL){
        //prints both date and time
        //the function converts the time to de date time format
        printf("Today is: %s", ctime(&pressent));
    }else if(!strcmp(tr[0], "-d")){
        //prints the current date
        int day, month, year;
        day = local->tm_mday;
        month = local->tm_mon + 1;
        year = local->tm_year + 1900;
        printf("%d/%d/%d \n", day, month, year);

    }else if(!strcmp(tr[0], "-h")){
        //prints the current hour
        int hours, minutes, seconds;
        hours = local->tm_hour;
        minutes = local->tm_min;
        seconds = local->tm_sec;
        printf("%d:%d:%d \n", hours, minutes, seconds);
    }else
    	printf("Wrong parameter, try '-h' (for hour) or '-d' (for date) or without parameters for everything\n");

}

void cmd_infosis(char *tr[]){

    struct utsname inf_uname;

    if(uname(&inf_uname) == -1){
        printf("ERROR\n");
        return;
    }

    printf("Sysname: %s\n", inf_uname.sysname);
    printf("Node name: %s\n", inf_uname.nodename);
    printf("Operating system release: %s\n", inf_uname.release);
    printf("Version: %s\n", inf_uname.version);
    printf("Hardware identifier: %s\n", inf_uname.machine);
}

void cmd_hist(char * tr[], tList *L){
    char* endPtr;
    if(tr[0] == NULL){
        //prints all the commands
        printList(*L, -1);
    }
    else if(!strcmp(tr[0], "-c")){
        //clears the list
        clearList(L);

    }else if(strtol(tr[0],&endPtr,10) > 0){
        //print the first N commands
        long int num = strtol(tr[0],&endPtr,10);
        printList(*L, num);
    }else{
    	printf("Wrong parameter, try with '-c' to clear the history of commands, ");
    	printf("without parameters to show the complete history or a number 'n' as ");
    	printf("parameter to see the first 'n' commands\n");
    }

}



void cmd_command(char *tr[], tList *L){
    char* endPtr2;
    tIteml data;
    int index;
    char *trozos[MAX/2];
    const int lastIndex = retLastIndex(*L);
		
    
    
    if(tr[0] == NULL || atoi(tr[0]) == lastIndex){
    	printf("Error, you use a NULL index or you are calling to the index of this commnad.\n");
        return;
    }else if(strtol(tr[0],&endPtr2,10) > - 1){
        /*here we select a element in a given position if it is indicated a possible index (greater or iqual to 0)
         then we use the function select at position which returns a struct with the command, and its index.
         If the index given didn't exist, it returns an element with index -1*/
        index = strtol(tr[0],&endPtr2,10);
	data = selectAtPosition(index, *L);
	
	}else{
		printf("Not a valid command number try a possitive number.\n");
		return;
	}

	
	if(data.index == -1){
	    printf("That position doesn't exist\n");
	    return;
	}

	if(TrocearCadena(data.data, trozos) == 0)
        	return; 
	
	ProcesarEntrada(trozos, L);
}




void cmd_create(char *tr[]){
	if(tr[0] == NULL){
		//if no name for the directory is spacified then the current working directory is printed on the screen
		cmd_carpeta(NULL);
		return;
	}
	
	if(strcmp(tr[0], "-f") == 0){
		//if there is a parameter '-f' then we create a file 
		if(tr[0] != NULL){
		/*if a name for the file is specified then we create it using the systemcall open()
		  its parameters are the name of the file, 'O_CREAT | O_EXCL' tells to create and the file if 
		  it doesn't exist and to raise an error if the file already exists, and the last parameter are de permissions.
		  If an error occurs the the systemcall returns -1 */
		  
			int fd;
			if((fd = open(tr[1], O_CREAT | O_EXCL,0777)) == -1){
				perror("ERROR");
				return;
			}
			
			close(fd); //we close the file
		}else{
			//if no name is specified for the file then the current working directory is shown in the screen
			cmd_carpeta(NULL);	
			
		}
	}else{
		/*we create a directory with the systemcall 'mkdir', whose parameters are the name of the file and the 
		permissions. If an error occurs the the systemcall returns -1*/
		if(mkdir(tr[0], 0777) == -1){
		
			perror("ERROR");
		}
	
	}
	
}
	

void cmd_fin(char * tr [], tList *L){
    clearList(L);
    removeHead(L);
    exit(0);
}





int TrocearCadena(char * cadena, char * trozos[]){
    int i=1;
    
    
    if ((trozos[0]=strtok(cadena," \n\t"))==NULL)
        return 0;
    while ((trozos[i]=strtok(NULL," \n\t"))!=NULL)
        i++;
    return i;
}


void ProcesarEntrada(char * tr[], tList * L){
    int i;
    if(tr[0] == NULL)
        return;
    if(!strcmp(tr[0], "hist")) {//here we check for the hist command
        cmd_hist(tr + 1, L);

        return;
    }else if(!strcmp(tr[0], "comando")){//here we check for the commnand command
        cmd_command(tr + 1, L);
        return;
    
    }else if(!strcmp(tr[0], "fin") || !strcmp(tr[0], "bye") || !strcmp(tr[0], "salir"))
    	cmd_fin(tr + 1, L);
    
    {
        for(i=0;c[i].name != NULL; i++){//here we check for the rest of the commands
            if(!strcmp(c[i].name, tr[0])) {
            
                (*c[i].func)(tr + 1);
                return;
            }
        }
        printf("Command not found\n");

    }
}

int main( int argc, char * argv [] ){
    char entrada[MAX];
    char *trozos[MAX/2];
    tList L;
    CreateEmptyList(&L);	
	
    while (1){
        printf("--->");
        fgets(entrada, MAX, stdin);
        InsertElement(&L, entrada);
        if(TrocearCadena(entrada, trozos) == 0)
            continue;
        ProcesarEntrada(trozos, &L);
        
    }
}

                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           
