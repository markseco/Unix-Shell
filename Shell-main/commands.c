#include "commands.h"
#include "list_blocks.h"
#include "libraries-headers.h"

void cmd_autores(char *tr[]) {

    if (tr[0] == NULL) {
        //prints the names and logins of the authors
        printf("Diego Dopazo Garcia: diego.dopazo.garcia@udc.es\n");
        printf("Marcos Seco Anderson: marcos.seco.anderson@udc.es\n");
    } else if (!strcmp(tr[0], "-l")) {
        //prints the logins of the authors
        printf("diego.dopazo.garcia@udc.es\n");
        printf("marcos.seco.anderson@udc.es\n");
    } else if (!strcmp(tr[0], "-n")) {
        //prints the names of the authors
        printf("Diego Dopazo Garcia\n");
        printf("Marcos Seco Anderson\n");
    } else {
        printf("Wrong parameter, try '-l' (for the logins) or '-n' (for the names) or without parameters (for both options)\n");
    }
}

void cmd_pid(char *tr[]) {
    if (tr[0] == NULL) {
        //prints the process pid
        int pid = getpid();
        printf("Shell's pid: %d \n", pid);

    } else if (!strcmp(tr[0], "-p")) {
        //prints the parent process' pid
        int ppid = getppid();
        printf("Shell's parent pid: %d \n", ppid);
    } else {
        printf("Wrong parameter, try '-p' for the parent's process pid or without parameters for the process' pid\n");
    }
}

void cmd_carpeta(char *tr[]) {
    char dir[MAX];
    if (tr[0] == NULL) {
        //shows the current working directory
        printf("%s", getcwd(dir, MAX));
        printf("\n");
    } else if (chdir(tr[0]) == -1) {
        printf("Cannot change directory.\n");
        printf(" ");
    }

}

void cmd_ayuda(char *tr[]) {

    if (tr[0] == NULL) {
        //prints the name of the commands
        for (int i = 0; c[i].name != NULL; i++) {
            printf("%s\n", c[i].name);
        }
        printf("hist\n");
        printf("comando\n");
        printf("allocate\n");
        printf("deallocate\n");
        printf("memory\n");
    } else if (tr[0] != NULL) {
        //prints the name of the commands and some information about them
        if (strcmp(tr[0], "hist") == 0) {
            printf("hist [-c|-N]\tShows (or deletes) the command history\n" "\t\t-N: shows the N first\n" "\t\t-c: deletes the history\n");
            return;
        } else if (strcmp(tr[0], "comando") == 0) {
            printf("comando [-N]\tRepeats the content of N (from the history)\n");
            return;
        }else if(strcmp(tr[0], "allocate") == 0){
            printf("allocate [-malloc|-shared|-createshares|-mmap]... Asigns a memory block\n \t\t\t -malloc tam: asigns a malloc block of size tam \n \t\t\t -createshared cl tam: asigns (creating) the shared memory block of key cl and size tam \n \t\t\t -shared cl: asigns the shared memory block (already exists) of key cl \n \t\t\t -mmap fich perm: maps the file fich, perm are the permissions\n");
            return;
        }else if(strcmp(tr[0], "memory") == 0){
            printf("memory [-blocks|-funcs|-vars|-all|-pmap] .. Shows details about the process' memory\n \t\t\t -blocks: asigned memory blocks\n \t\t\t -funcs:the addres of the functions\n \t\t\t -all: everything\n \t\t\t -pmap: shows the output of the command pmap(or similar)\n");
            return;
        }else if(strcmp(tr[0], "deallocate") == 0){
            printf("deallocate [-malloc|-shared|-delkey|-mmap|addr].. Deallocates a memory block\n\t\t -malloc tam: deallocates the block of size tam\n\t\t -shared cl: deallocates (unmap) the shared memory block of key cl\n\t\t -delkey cl: deletes from system (without unmapping) the memory key cl\n\t\t -mmap fich: unmaps the mapped file fich\n\t\t addr: deallocates the memory block at the address addr\n");
        }

        for (int i = 0; c[i].name != NULL; i++) {
            if (strcmp(tr[0], c[i].name) == 0) {
                printf("%s\n", c[i].ayuda);
                return;
            }
        }
    } else
        printf("Error: Wrong parameters. Try again.\n");
}

void cmd_fecha(char *tr[]) {
    //type of arithmetic time
    time_t pressent;
    //the function obtains the actual time
    time(&pressent);

    //converts the 'pressent' variable to a struct
    struct tm *local = localtime(&pressent);
    int day, month, year, hours, minutes, seconds;
    day = local->tm_mday;
    month = local->tm_mon + 1;
    year = local->tm_year + 1900;
    hours = local->tm_hour;
    minutes = local->tm_min;
    seconds = local->tm_sec;

    if (tr[0] == NULL) {
        //prints both date and time
        printf("%d:%d:%d \n%d/%d/%d \n", hours, minutes, seconds, day, month, year);
    } else if (!strcmp(tr[0], "-d")) {
        printf("%d/%d/%d \n", day, month, year);
    } else if (!strcmp(tr[0], "-h")) {
        printf("%d:%d:%d \n", hours, minutes, seconds);
    } else {
        printf("Wrong parameter, try '-h' (for hour) or '-d' (for date) or without parameters for everything\n");
    }
}

void cmd_infosis(char *tr[]) {

    struct utsname inf_uname;

    if (uname(&inf_uname) == -1) {
        printf("ERROR\n");
        return;
    }
    printf("%s (%s), OS: %s %s %s\n", inf_uname.nodename,inf_uname.machine,inf_uname.sysname,inf_uname.release,inf_uname.version);
}

void cmd_hist(char *tr[], tList *L) {
    char *endPtr;
    if (tr[0] == NULL) {
        //prints all the commands
        printList(*L, -1);
    } else if (!strcmp(tr[0], "-c")) {
        //clears the list
        clearList(L);

    } else if (tr[0][0] == '-') {
        //print the first N commands
        tr[0][0] = '0';
        if (strtol(tr[0], &endPtr, 10) > 0) {
            int num = strtol(tr[0], &endPtr, 10);
            printList(*L, num);
        }
    } else {
        printf("Wrong parameter, try with '-c' to clear the history of commands, ");
        printf("without parameters to show the complete history or a number 'n' as ");
        printf("parameter to see the first 'n' commands\n");
    }

}

void cmd_command(char *tr[], tList *L, tListM *LM){
    char *endPtr2;
    tIteml data;
    int index;
    char *trozos[MAX / 2];
    const int lastIndex = retLastIndex(*L);


    if (tr[0] == NULL || atoi(tr[0]) == lastIndex) {
        printf("Error, you use a NULL index or you are calling to the index of this command.\n");
        return;
    } else if (strtol(tr[0], &endPtr2, 10) > -1) {
        /*An element is selected in a given position if it is indicated a possible index (greater or equal to 0)
         then a function is used to select at position which returns a struct with the command, and its index.
         If the index given doesn't exist, it returns an element with index -1*/
        index = strtol(tr[0], &endPtr2, 10);
        data = selectAtPosition(index, *L);
        printf("Executing hist(%d): ",index);

    } else {
        printf("Not a valid command number try a positive number.\n");
        return;
    }


    if (data.index == -1) {
        printf("That position doesn't exist\n");
        return;
    }

    char *data2 = malloc(strlen(data.data) * sizeof(char) + 1);
    strcpy(data2, data.data);

    if (TrocearCadena(data2, trozos) == 0) {
        return;
    }
    
    if(!strcmp(trozos[0], "comando")){
    	if(trozos[1] != NULL && atoi(trozos[1]) == data.index){
    		printf("Error, you use a NULL index or you are calling to the index of this command.\n");
    		free(data2);
    		return;	
    	}
    }
    
    printf("%s\n",trozos[0]);
    ProcesarEntrada(trozos, L, LM);

    free(data2);
}

void cmd_create(char *tr[]) {
    if (tr[0] == NULL) {
        //if no name for the directory is specified then the current working directory is printed on the screen
        char dir[MAX];
        printf("%s\n", getcwd(dir, MAX));
        return;
    }
    if (strcmp(tr[0], "-f") == 0) {
        //if there is a parameter '-f' then we create a file
        if (tr[0] != NULL) {
            /*if a name for the file is specified then we create it using the system call open()
              its parameters are the name of the file, 'O_CREAT | O_EXCL' and the last parameter are de permissions.
              If an error occurs the system call returns -1 */
            int fd;
            if ((fd = open(tr[1], O_CREAT | O_EXCL, 0777)) == -1) {
                perror("ERROR");
                return;
            }

            close(fd); //we close the file
        } else {
            //if no name is specified for the file then the current working directory is shown in the screen
            cmd_carpeta(NULL);

        }
    } else {
        /*A directory is created with the system call 'mkdir', which parameters are the name of the file and the
        permissions. If an error occurs the system call returns -1*/
        if (mkdir(tr[0], 0777) == -1) {

            perror("ERROR");
        }

    }

}

void cmd_stat(char *tr[]) {

    struct  sl_params params;

    params = aux_stat_list(tr);
    tr = tr + params.start_of_files;

    for (int i = 0; tr[i] != NULL; i++)
        doStat(tr[i], params);
}

void cmd_list(char *tr[]) {
    struct sl_params params;

    params = aux_stat_list(tr);
    int i = params.start_of_files;

    while (tr[i] != NULL) {
        if (!params.reca && !params.recb)
            print_dir(params, tr[i]);
        else if (params.reca)
            reca(params, tr[i]);
        else if (params.recb)
            recb(params, tr[i]);
        else
            printf("Invalid options\n");

        i++;
    }
}

void cmd_delete(char *tr[]) {

    while (tr[0] != NULL) {

        if (remove(tr[0]) == -1) {
            printf("Could not delete %s: %s\n", tr[0], strerror(errno));
        }

        tr = tr + 1;
    }

}

void cmd_deltree(char *tr[]) {
    /*This loop calls the function aux_deltree(char *path), which deletes a directory recursively, using as a
    parameter all the paths names indicated by the user.*/
    while (tr[0] != NULL) {
        aux_deltree(tr[0]);
        tr = tr + 1;
    }
}

void cmd_allocate(char *tr[], tListM *LM){
    size_t size;
    if(tr[0] == NULL){
        printListM(*LM, 'a'); //prints all the list 
    }else{
        if(!strcmp(tr[0], "-malloc")){
            if(tr[1] == NULL)
                printListM(*LM, 'M');
            else{
                
                if((size= (size_t) atoi(tr[1])) > 0)
                    doMalloc(size, LM);
                else
                    printf("Can't allocote 0 bytes or a negative number of bytes");

            }
        }else if(!strcmp(tr[0], "-createshared")){
            if(tr[1] == NULL || tr[2] == NULL)
                printListM(*LM, 'S');
            else{
                do_AllocateCreateshared(tr + 1, LM);
            }
        }else if(!strcmp(tr[0], "-shared")){
            if(tr[1] == NULL){
                printListM(*LM, 'S');
            }else{
                do_Shared(tr + 1, LM);
            }

        }else if(!strcmp(tr[0], "-mmap")){
            do_AllocateMmap(tr + 1, LM);
        }

    }
}

void cmd_deallocate(char *tr[], tListM *LM){
    size_t size;
    if(tr[0] == NULL){
        printListM(*LM, 'a'); //prints all the list 
    }else{
        if(!strcmp(tr[0], "-malloc")){
            if(tr[1] == NULL)
                printListM(*LM, 'M');
            else{
                
                if((size= (size_t) atoi(tr[1])) > 0)
                    delMallocSize(LM, size);
                else
                    printf("Can't deallocote a block with 0 bytes or a negative number of bytes");

            }
        }else if(!strcmp(tr[0], "-delkey")){
            if(tr[1] == NULL)
                printListM(*LM, 'S');
            else{
                do_DeallocateDelkey(tr + 1);
            }
        }else if(!strcmp(tr[0], "-shared")){
            if(tr[1] == NULL){
                printListM(*LM, 'S');
            }else{
                do_DelShared(tr + 1, LM);
            }

        }else if(!strcmp(tr[0], "-mmap")){
            do_UnMap(tr + 1, LM);
        }else{
            //delAtAddress
        }

    }

}

void cmd_memfill(char *tr[]){

    void * mem;
    size_t cont;
    unsigned char byte = ' ';
    //char *ptr;
    
    if(!(tr[0] == NULL || tr[1] == NULL || tr[2] == NULL)){
        //mem = (void *)strtoul(tr[0], &ptr, 0);
        sscanf(tr[0], "%p", &mem);
        sscanf(tr[1], "%zu", &cont);
        byte = (unsigned char) tr[2][1];
        printf("Filling %zu bytes of memory with the byte %c(%x) from the memory position %p\n", cont, byte, byte, mem);
        LlenarMemoria (mem, cont, byte);

    }
}

void cmd_i_o(char *tr[]){
    if(tr[0] == NULL){
        printf("Missing parameters\n");
        return;
    }
    if(!strcmp(tr[0], "read")){
        do_I_O_read(tr + 1);
    }else if(!strcmp(tr[0], "write")){
        do_I_O_write(tr + 1);
    }

}

void cmd_memdump(char *tr[]){
    void * mem;
    size_t cont;
    //char *ptr;
    
    if(!(tr[0] == NULL || tr[1] == NULL)){
        //mem = (void *)strtoul(tr[0], &ptr, 0);
        sscanf(tr[0], "%p", &mem);
        sscanf(tr[1], "%zu", &cont);
        printf("Dumping %zu byte of memory from memory position %p\n", cont, mem);
        dumpMemory(mem, cont);

    }

}

int glob1 = 1;
int glob2 = 2;
int glob3 = 3;

void cmd_memory(char *tr[], tListM LM){
    int x, y, z;
    static int x1, y1, z1;
    
    if(tr[0] == NULL || strcmp(tr[0], "-all") == 0 || !strcmp(tr[0], "-vars")){

        printf("Local variables   \t%p,\t%p,\t%p\n", &x, &y, &z);
        printf("Global variables   \t%p,\t%p,\t%p\n", &glob1, &glob2, &glob3);
        printf("Static variables   \t%p,\t%p,\t%p\n", &x1, &y1, &z1);

    }
    
    if(tr[0] == NULL || strcmp(tr[0], "-all") == 0 || !strcmp(tr[0], "-funcs")){

        printf("Program functions \t%p,\t%p,\t%p\n", cmd_autores, cmd_ayuda, cmd_pid);
        printf("Library functions \t%p,\t%p,\t%p\n", stat, open, remove);
        

    }
    
    if(tr[0] != NULL && !strcmp(tr[0], "-pmap")){

        Do_pmap();

    }

    if(tr[0] == NULL || strcmp(tr[0], "-all") == 0 || !strcmp(tr[0], "-blocks")){

        printListM(LM, 'A');
    
    }
}

void cmd_recursiva(char *tr[]){
    if(tr[0] != NULL){
        Recursiva(atoi(tr[0]));
    }
}

void cmd_fin(char *tr[], tList *L, tListM *LM) {
    clearList(L);
    removeHead(L);

    clearListM(LM);
    removeHeadM(LM);
    
    exit(0);
}
