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
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <dirent.h>
#include <errno.h>
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
char LetraTF (mode_t m);
char * ConvierteModo (mode_t m, char *permisos);

void cmd_stat(char *tr[]);
void cmd_list(char *tr[]);
void cmd_delete(char *tr[]);
void aux_deltree(char *path);
void cmd_deltree(char *tr[]);
void ProcesarEntrada(char * tr[], tList * L);
int TrocearCadena(char * cadena, char * trozos[]);


//definition of the structure that is used to store the name of the command its use and the function that implements it
struct CMD{
    char *name;
    char * ayuda;
    void (*func)(char **);
};

//definition of the structure that is used to store the parameters that are passed to the functions cmd_stat and cmd_list
typedef struct sl_params{
	bool lon;
	bool acc;
	bool link;
	bool hid; 
	bool reca;
	bool recb;
	bool exterior_path;
	int start_of_files;

}sl_params;

struct CMD c[]={
        {"autores","Prints the names and logins of the program authors",cmd_autores},
        {"carpeta","Changes the current working directoyof the shell",cmd_carpeta},
        {"ayuda", "Displays a list of available commands", cmd_ayuda},
        {"pid","Prints the pid of the process executing the shell",cmd_pid},
        {"fecha", "It prints both the current date and the current time.\n fecha -d: prints the current date.\n fecha -h prints the current time", cmd_fecha},
        {"infosis", "Prints information on the machine runnig the shell", cmd_infosis},
        {"create", "create [-f] [name]	Crea un directorio o un fichero (-f)", cmd_create},
        {"stat", "stat [-long] [-acc] [-link] name1 name2 ... listaficheros; \n\t\t\t-long: listado largo \n\t\t\t-acc: acesstime\n\t\t\t-link: if is a symbolic link, the path is contained", cmd_stat},
        {"list", "list [-reca] [-recb] [-hid] [-long] [-acc] [-link] n1 n2 ... contentlist; \n\t\t\t-hid:includes hidden files\n\t\t\t-reca: recursive (before) \n\t\t\t-recb: recursive (after)\n\t\t\trest of parameters as in stat." ,cmd_list},
        {"deltree", "deltree [name1 name2 ...] deletes non empty directories recursively", cmd_deltree},
        {"delete", "delete [name1 name2 ...] deletes files and empty directories", cmd_delete},
        {NULL, NULL,NULL}
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
    //shows the current working directory
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
		//cmd_carpeta(NULL); preguntar porque no va
		printf("Error: lack of parameters.\n");
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





char LetraTF (mode_t m)
{
     switch (m&S_IFMT) { /*and bit a bit con los bits de formato,0170000 */
        case S_IFSOCK: return 's'; /*socket */
        case S_IFLNK: return 'l'; /*symbolic link*/
        case S_IFREG: return '-'; /* fichero normal*/
        case S_IFBLK: return 'b'; /*block device*/
        case S_IFDIR: return 'd'; /*directorio */ 
        case S_IFCHR: return 'c'; /*char device*/
        case S_IFIFO: return 'p'; /*pipe*/
        default: return '?'; /*desconocido, no deberia aparecer*/
     }
}


char * ConvierteModo (mode_t m, char *permisos)
{
    strcpy (permisos,"---------- ");
    
    permisos[0]=LetraTF(m);
    if (m&S_IRUSR) permisos[1]='r';    /*propietario*/
    if (m&S_IWUSR) permisos[2]='w';
    if (m&S_IXUSR) permisos[3]='x';
    if (m&S_IRGRP) permisos[4]='r';    /*grupo*/
    if (m&S_IWGRP) permisos[5]='w';
    if (m&S_IXGRP) permisos[6]='x';
    if (m&S_IROTH) permisos[7]='r';    /*resto*/
    if (m&S_IWOTH) permisos[8]='w';
    if (m&S_IXOTH) permisos[9]='x';
    if (m&S_ISUID) permisos[3]='s';    /*setuid, setgid y stickybit*/
    if (m&S_ISGID) permisos[6]='s';
    if (m&S_ISVTX) permisos[9]='t';
    
    return permisos;
}

sl_params aux_stat_list(char *tr[]){
	//this function returns a structure of type sl_params 
	
	sl_params ret;
	ret.lon = false;
	ret.acc = false;
	ret.link = false;
	ret.hid = false;
	ret.reca = false;
	ret.recb = false;
	ret.start_of_files = 0;
	
	while(tr[0] != NULL){
		if(tr[0][0] != '-')
			return ret;
		else if(!strcmp("-long", tr[0]))
			ret.lon = true;
		else if(!strcmp("-acc", tr[0]))
			ret.acc = true;
		else if(!strcmp("-link", tr[0]))
			ret.link = true;
		else if(!strcmp("-hid", tr[0]))
			ret.hid = true;
		else if(!strcmp("-reca", tr[0]))
			ret.reca = true;
		else if(!strcmp("-recb", tr[0]))
			ret.recb = true;
		
		ret.start_of_files++;	
		tr = tr + 1;
	}
	
	return ret;
}


void format_dir_output(char *tr){
	int i = strlen(tr);
	int j = i - 1;
	
	if(j < 0)
		return;
	
	while(tr[j] != '/'){
		j--;
		if(j < 0)
			return;
	}
	j++;
	
	 strncpy(tr, &tr[j], i - j);
    	 tr[i - j] = '\0';
	
}


void doStat(char *name, sl_params params, bool is_list){
	
	struct stat buffer;
	struct passwd *uname;
	struct group *gname;
	char *user,*group, permissions[12], enlace[MAX],acctime[100];
	int n;
	
	
	if(lstat(name, &buffer) == -1){
		printf ("Cannot access %s: %s\n", name,strerror(errno));
		return;
	}
	
	format_dir_output(name);
	
	if (!params.lon){
		printf ("%s: %lld\n",name, (long long) buffer.st_size);
		return;
	}
		
		//here we get the string of the permissions 
	ConvierteModo (buffer.st_mode, permissions);
	
	if(params.acc)
		strftime(acctime, 100, "%Y/%m/%d-%H:%M",localtime(&buffer.st_atime));
	else 
		strftime(acctime, 100, "%Y/%m/%d-%H:%M",localtime(&buffer.st_mtime));	
			//here we get the user name and the goupname
	
	user=(uname = getpwuid(buffer.st_uid)) == NULL?"UNKNOWN":uname->pw_name;
				
	group=(gname = getgrgid(buffer.st_gid)) == NULL?"UNKNOWN":gname-> gr_name;
	
	printf("%s \t %ld \t (%ld) \t %s \t %s \t %s \t %ld \t %s",
			 acctime, buffer.st_nlink, buffer.st_ino, user, group, permissions, buffer.st_size, name);
	
	if(params.link && S_ISLNK(buffer.st_mode)){
		n=readlink(name, enlace,MAX);
		if (n!=-1){
			enlace[n]='\0';
			printf ("-->%s \n",enlace);
			}
	}
	printf("\n");
	
	
}




void sl_params_to_tr(sl_params params,  char * path, char * tro[]){
	char str[MAX] = "";
	
	if(params.lon){
		strcat(str, " -long");
	}
	
	if(params.acc){
		strcat(str, " -acc");
	}
	
	if(params.link){
		strcat(str, " -link");
	}
	
	strcat(str, " ");
	strcat(str, path);
	
	if(TrocearCadena(str, tro) == 0)
		printf("Error at processing the string in the function sl_params_to_tr");
	
}



void cmd_stat (char *tr[]){

     sl_params params;
     
     params = aux_stat_list(tr);
     tr = tr + params.start_of_files;

     for (int i=0; tr[i]!=NULL; i++)
     	doStat (tr[i],params, true);
}



int lstat_check(char *path, struct stat *st){
		
	if(lstat(path, st) == -1) {
		printf("Could not access %s: %s\n", path, strerror(errno));
	        return -1;    
	}
	
	return 0;
}


void print_dir(sl_params params, char *path_name){

	struct stat st;
	char *tro[1];
	
	//gets the status of the path and if it can not it specifies the error and exits the funtion
	if(lstat_check(path_name, &st) == -1)
		return;
	printf("**************%s\n", path_name);

	
	if((st.st_mode & S_IFMT) == S_IFDIR) { // path es un directorio
		DIR *d;
		struct dirent *ent;

		if((d = opendir(path_name)) == NULL) {
			printf("Could not open %s: %s\n", path_name, strerror(errno));
			return;
		}
		
		//in this while loop we read the elements that are in a directory
		while((ent = readdir(d)) != NULL) {
			char new_path[MAX];
			if(ent->d_name[0] == '.' && !params.hid)//we ignore hidden directories if we are't asked to show them
				continue;
			
			sprintf(new_path, "%s/%s", path_name, ent->d_name);
			sl_params_to_tr(params, new_path, tro);
			cmd_stat(tro);
			
		}
		
		
		closedir(d);
		
	}else{  
		sl_params_to_tr(params, path_name, tro);
		cmd_stat(tro);
	}
	
}


void reca(sl_params params, char *path_name){
	
	struct stat st;
	struct stat dir;
	
	//gets the status of the path and if it can not it specifies the error and exits the funtion
	if(lstat_check(path_name, &st) == -1)
		return;
	
	
	print_dir(params, path_name);
	
	if((st.st_mode & S_IFMT) == S_IFDIR) { // path es un directorio
		DIR *d;
		struct dirent *ent;

		if((d = opendir(path_name)) == NULL) {
			printf("Could not open %s: %s\n", path_name, strerror(errno));
			return;
		}
		
		//in this while loop we read the elements that are in a directory
		while((ent = readdir(d)) != NULL) {
			char new_path[MAX];
			if(strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0)//we ignore these directories
				continue;
			sprintf(new_path, "%s/%s", path_name, ent->d_name);
			if(lstat_check(new_path, &dir) == -1)
				return;
			if(S_ISDIR(dir.st_mode) == 0)
				continue;
			
			
			sprintf(new_path, "%s/%s", path_name, ent->d_name);
			reca(params, new_path);
			
		}
		
		
		closedir(d);
		
	}
}


void recb(sl_params params, char *path_name){
		struct stat st;
		struct stat dir;
		char new_path[MAX];
	
	//gets the status of the path and if it can not it specifies the error and exits the funtion
	if(lstat_check(path_name, &st) == -1)
		return;
	
	if((st.st_mode & S_IFMT) == S_IFDIR) { // path es un directorio
		DIR *d;
		struct dirent *ent;

		if((d = opendir(path_name)) == NULL) {
			printf("Could not open %s: %s\n", path_name, strerror(errno));
			return;
		}
		
		//in this while loop we read the elements that are in a directory
		while((ent = readdir(d)) != NULL) {
			if(strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0)//we ignore these directories
				continue;
					sprintf(new_path, "%s/%s", path_name, ent->d_name);
			if(lstat_check(new_path, &dir) == -1)
				return;
			if(S_ISDIR(dir.st_mode) == 0)
				continue;
			
			
			sprintf(new_path, "%s/%s", path_name, ent->d_name);
			recb(params, new_path);
			
		}
		
		closedir(d);
		
	}
	print_dir(params, path_name);

}


void cmd_list(char *tr[]){
	sl_params params;
	
	params = aux_stat_list(tr);
	tr = tr + params.start_of_files;	
	
	while(tr[0] != NULL){
		if(!params.reca && !params.recb)
			print_dir(params, tr[0]);	
		else if(params.reca)
			reca(params, tr[0]);
		else if(params.recb)
			recb(params, tr[0]);
		else
			printf("Invalid options\n");
		
	
		tr = tr + 1;
		
		printf("******************************************************\n");
	}
}







void aux_deltree(char *path) {    
	struct stat st;
	
	
	//gets the status of the path and if it can not it specifies the error and exits the funtion
	if(lstat_check(path, &st) == -1)
		return;
	
	
	if((st.st_mode & S_IFMT) == S_IFDIR) { // path es un directorio
		DIR *d;
		struct dirent *ent;

		if((d = opendir(path)) == NULL) {
			printf("Could not open %s: %s\n", path, strerror(errno));
			return;
		}
		
		//in this while loop we read the elements that are in a directory
		while((ent = readdir(d)) != NULL) {
			char new_path[MAX];

			if(strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0)//we ignore these directories
				continue;

			sprintf(new_path, "%s/%s", path, ent->d_name);
			
			//we call the function recursively in all the files and directories
			aux_deltree(new_path);
		}
		closedir(d);
	}
	
	/*if it is a file or an empty directory(which it is because we have deleted all of its elements in the while loop)
	  the funtion remove deletes it*/
	if(remove(path) == -1) {
		printf("Could not delete %s: %s\n", path, strerror(errno));
	}

	printf("Borrar %s\n", path);
}


void cmd_delete(char *tr[]){

	while(tr[0] != NULL){
	
		if(remove(tr[0]) == -1) {
			printf("Could not delete %s: %s\n", tr[0], strerror(errno));
		}
		
		tr = tr + 1;
	}

}



void cmd_deltree(char *tr[]){
	/*This loop call the function aux_deltree(char *path), which deletes a directory recursively, using as a 
	parameter all of the paths names indicated by the user.*/
	while(tr[0] != NULL){
		aux_deltree(tr[0]);
		tr = tr + 1;
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

                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           
