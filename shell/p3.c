#define MAX 1024
#define TAMANO 2048

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <time.h>
#include <sys/utsname.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <pwd.h>
#include <grp.h>
#include <dirent.h>
#include <errno.h>
#include <sys/shm.h>
#include <sys/resource.h>
#include "list_blocks.h"
#include "head_list.h"
#include "list_process.h"

extern char **__environ;

int TrocearCadena(char *cadena, char *trozos[]);

void ProcesarEntrada(char *tr[], char *argv3[], tList *L, tListM *LM, tListP *LP);

void cmd_autores(char *tr[]);

void cmd_pid(char *tr[]);

void cmd_carpeta(char *tr[]);

void cmd_ayuda(char *tr[]);

void cmd_fecha(char *tr[]);

void cmd_infosis(char *tr[]);

void cmd_hist(char *tr[], tList *L);

void cmd_command(char *tr[], char *argv3[], tList *L, tListM *LM, tListP *LP);

void cmd_fin(char *tr[], tList *L, tListM *LM, tListP *LP);

void cmd_create(char *tr[]);

void cmd_stat(char *tr[]);

void cmd_list(char *tr[]);

void cmd_delete(char *tr[]);

void cmd_deltree(char *tr[]);

void cmd_allocate(char *tr[], tListM *LM);

void cmd_memfill(char *tr[]);

void cmd_i_o(char *tr[]);

void cmd_memdump(char *tr[]);

void cmd_memory(char *tr[], tListM LM);

void cmd_recursiva(char *tr[]);

void cmd_priority(char *tr[]);

void cmd_showvar(char *tr[], char *arg3[]);

void cmd_fork (char *tr[], tListP *LP);

void cmd_job(char *tr[], tListP *LP);

//definition of the structure that is used to store the name of the command its use and the function that implements it
struct CMD {
    char *name;
    char *ayuda;

    void (*func)(char **);
};


//definition of the structure that is used to store the parameters that are passed to the functions cmd_stat and cmd_list
struct sl_params {
    bool lon;
    bool acc;
    bool link;
    bool hid;
    bool reca;
    bool recb;
    int start_of_files;

};

struct CMD c[] = {
        {"autores",   "autores [-n|-l]\tDisplays the names &/or of the authors",                                                                                                                                                                                        cmd_autores},
        {"carpeta",   "carpeta [dir]\tChanges (or Displays) the current shell directory",                                                                                                                                                                               cmd_carpeta},
        {"ayuda",     "ayuda [cmd]\tDisplays help for all commands\n""\t\tcmd: cmd command info",                                                                                                                                                                       cmd_ayuda},
        {"pid",       "pid [-p]\tDisplays the pid of the shell or the pid of its father",                                                                                                                                                                               cmd_pid},
        {"fecha",     "fecha [-d|-h\tDisplays the date or the current time",                                                                                                                                                                                            cmd_fecha},
        {"infosis",   "infosis \tDisplays information about the machine where the shell is running in",                                                                                                                                                                 cmd_infosis},
        {"create",    "create [-f] [name]\tCreates a directory or a file (-f)",                                                                                                                                                                                         cmd_create},
        {"stat",      "stat [-long] [-acc] [-link] name1 name2 ...   lists files; \n\t\t\t-long: long listing \n\t\t\t-acc: acesstime\n\t\t\t-link: if the link is symbolic, the path is contained",                                                                    cmd_stat},
        {"list",      "list [-reca] [-recb] [-hid] [-long] [-acc] [-link] n1 n2 ... lists the content of the directories; \n\t\t\t-hid: includes hidden files\n\t\t\t-reca: recursive (before) \n\t\t\t-recb: recursive (after)\n\t\t\trest of parameters as in stat.", cmd_list},
        {"deltree",   "deltree [name1 name2 ...] Deletes non empty directories recursively",                                                                                                                                                                            cmd_deltree},
        {"delete",    "delete [name1 name2 ...] Deletes empty files and directories",                                                                                                                                                                                   cmd_delete},
        {"memfill",   "memfill addr cont byte   Fills the mememory since addr with byte",                                                                                                                                                                               cmd_memfill},
        {"memdump",   "memdump addr cont   Shows the contents of addr in screen",                                                                                                                                                                                       cmd_memdump},
        {"recursiva", "recursiva [n] Invoques the recursive function n times",                                                                                                                                                                                          cmd_recursiva},
        {"i-o",       "i-o [read|write] [-o] fiche addr cont\n\t\t read fiche addr cont: Reads cont bytes from fiche to addr\n\t\t write [-o] fich addr cont: Writes cont bytes from addr to fich. -o for overwrite\n\t\t\t addr is a memory address\n",                cmd_i_o},
        {"priority", "priority [pid] [value] Shows or changes the priority of the process pid to value\n", cmd_priority},
        {NULL, NULL, NULL}
};

/* PRACTICA 0*/

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
        printf("showvar\n");
        printf("changevar\n");
        printf("showenv\n");
        printf("execute\n");
        printf("fork\n");
        printf("listjobs\n");
        printf("jobs\n");
        printf("deljobs\n");
        printf("job\n");
        
    } else if (tr[0] != NULL) {
        //prints the name of the commands and some information about them
        if (strcmp(tr[0], "hist") == 0) {
            printf("hist [-c|-N]\tShows (or deletes) the command history\n" "\t\t-N: shows the N first\n" "\t\t-c: deletes the history\n");
            return;
        } else if (strcmp(tr[0], "comando") == 0) {
            printf("comando [-N]\tRepeats the content of N (from the history)\n");
            return;
        } else if (strcmp(tr[0], "allocate") == 0) {
            printf("allocate [-malloc|-shared|-createshares|-mmap]... Asigns a memory block\n \t\t\t -malloc tam: asigns a malloc block of size tam \n \t\t\t -createshared cl tam: asigns (creating) the shared memory block of key cl and size tam \n \t\t\t -shared cl: asigns the shared memory block (already exists) of key cl \n \t\t\t -mmap fich perm: maps the file fich, perm are the permissions\n");
            return;
        } else if (strcmp(tr[0], "memory") == 0) {
            printf("memory [-blocks|-funcs|-vars|-all|-pmap] .. Shows details about the process' memory\n \t\t\t -blocks: asigned memory blocks\n \t\t\t -funcs:the addres of the functions\n \t\t\t -all: everything\n \t\t\t -pmap: shows the output of the command pmap(or similar)\n");
            return;
        } else if (strcmp(tr[0], "deallocate") == 0) {
            printf("deallocate [-malloc|-shared|-delkey|-mmap|addr].. Deallocates a memory block\n\t\t -malloc tam: deallocates the block of size tam\n\t\t -shared cl: deallocates (unmap) the shared memory block of key cl\n\t\t -delkey cl: deletes from system (without unmapping) the memory key cl\n\t\t -mmap fich: unmaps the mapped file fich\n\t\t addr: deallocates the memory block at the address addr\n");
            return;
        }else if(strcmp(tr[0], "showvar") == 0){
            printf("showvar var    Shows the value and the directions of the enviroment variable var\n");
            return;
        }else if(strcmp(tr[0], "changevar") == 0){
            printf("changevar [-a | -e | -p] var value     Changes the value of an enviroment variable\n\t\t -a: accesses by main's third argument\n\t\t -e: accesses by environ\n\t\t -p: accesses by putenv\n");
            return;
        }else if(strcmp(tr[0], "showenv") == 0){
            printf("showenv [-environ | -addr]  Shows the enviroment of the process\n\t\t -environ: accesses using environ (instead of using main's third argument)\n\t\t -addr: shows the value and where are stored environ and main's third argument\n");
            return;
        }else if(strcmp(tr[0], "fork") == 0){
            printf("fork  The shell does a fork and waits until his son has finished\n");
            return;
        }else if(strcmp(tr[0], "execute") == 0){
            printf("execute VAR1 VAR2 ... prog args ... [@prio] executes without creating a process the programe prog with argumens args and with enviroment variables VAR1 VAR2 ...\n");
            return;
        }else if(strcmp(tr[0], "jobs") == 0 || strcmp(tr[0], "listjobs") == 0){
            printf("%s Lists the processes on the background\n", tr[0]);
            return;
        }else if(strcmp(tr[0], "deljobs") == 0){
            printf("deljobs [-term][-sig] Deletes from the processes list the background processes\n\t\t -term: the ones that are terminated\n\t\t -sig:the ones ended by a signal\n");
            return;
        }else if(strcmp(tr[0], "job") == 0){
            printf("job [-fg] pid  Shows information about the process pid\n\t\t -fg changes it to the foreground\n");
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
    printf("%s (%s), OS: %s %s %s\n", inf_uname.nodename, inf_uname.machine, inf_uname.sysname, inf_uname.release,
           inf_uname.version);
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

void cmd_command(char *tr[], char *argv3[], tList *L, tListM *LM, tListP *LP){
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
        printf("Executing hist(%d): ", index);

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

    if (!strcmp(trozos[0], "comando")) {
        if (trozos[1] != NULL && atoi(trozos[1]) >= data.index) {
            printf("Error, you use a NULL index or you are calling to the index of this command or to a greater index which can lead to an error.\n");
            free(data2);
            return;
        }
    }

    printf("%s\n", trozos[0]);
    ProcesarEntrada(trozos, argv3, L, LM, LP);

    free(data2);
}

void cmd_fin(char *tr[], tList *L, tListM *LM, tListP *LP) {
    clearList(L);
    removeHead(L);

    clearListM(LM);
    removeHeadM(LM);

    clearListP(LP);
    removeHeadP(LP);

    exit(0);
}


/* PRACTICA 1*/

char LetraTF(mode_t m) {
    switch (m & S_IFMT) { /*and bit a bit con los bits de formato,0170000 */
        case S_IFSOCK:
            return 's'; /*socket */
        case S_IFLNK:
            return 'l'; /*symbolic link*/
        case S_IFREG:
            return '-'; /* normal file*/
        case S_IFBLK:
            return 'b'; /*block device*/
        case S_IFDIR:
            return 'd'; /*directory */
        case S_IFCHR:
            return 'c'; /*char device*/
        case S_IFIFO:
            return 'p'; /*pipe*/
        default:
            return '?'; /*unknown, shouldn't appear*/
    }
}

char *ConvierteModo(mode_t m, char *permisos) {
    strcpy(permisos, "---------- ");

    permisos[0] = LetraTF(m);
    if (m & S_IRUSR)
        permisos[1] = 'r'; /*owner*/
    if (m & S_IWUSR)
        permisos[2] = 'w';
    if (m & S_IXUSR)
        permisos[3] = 'x';
    if (m & S_IRGRP)
        permisos[4] = 'r'; /*group*/
    if (m & S_IWGRP)
        permisos[5] = 'w';
    if (m & S_IXGRP)
        permisos[6] = 'x';
    if (m & S_IROTH)
        permisos[7] = 'r'; /*rest*/
    if (m & S_IWOTH)
        permisos[8] = 'w';
    if (m & S_IXOTH)
        permisos[9] = 'x';
    if (m & S_ISUID)
        permisos[3] = 's'; /*setuid, setgid y sticky bit*/
    if (m & S_ISGID)
        permisos[6] = 's';
    if (m & S_ISVTX)
        permisos[9] = 't';

    return permisos;
}

struct sl_params aux_stat_list(char *tr[]) {
    // this function returns a structure of type sl_params

    struct sl_params ret;
    ret.lon = false;
    ret.acc = false;
    ret.link = false;
    ret.hid = false;
    ret.reca = false;
    ret.recb = false;
    ret.start_of_files = 0;
    int i = 0;

    if (tr == NULL)
        return ret;

    while (tr[i] != NULL) {
        if (strlen(tr[i]) <= 0)
            return ret;
        else if (tr[i][0] != '-')
            return ret;
        else if (!strcmp("-long", tr[i]))
            ret.lon = true;
        else if (!strcmp("-acc", tr[i]))
            ret.acc = true;
        else if (!strcmp("-link", tr[i]))
            ret.link = true;
        else if (!strcmp("-hid", tr[i]))
            ret.hid = true;
        else if (!strcmp("-reca", tr[i]))
            ret.reca = true;
        else if (!strcmp("-recb", tr[i]))
            ret.recb = true;

        ret.start_of_files++;
        i++;
    }

    return ret;
}

int length_name_file(char *tr) {
    int i = strlen(tr);
    int j = i - 1;

    if (tr == NULL)
        return 0;
    if (j <= 0)
        return 0;

    while (tr[j] != '/') {
        j--;
        if (j <= 0)
            return 0;
    }
    return (i - j);
}

void format_str(int size, char *name1, char *name) {
    int sf;
    if (size != 0) {
        sf = strlen(name1) - size + 1;
        strncpy(name, name1 + sf, size);
        name[size] = '\0';
    } else {
        name = strdup(name1);
    }
}

char *file_name(char *path) {
    char *file[MAX / 2];
    int i = 1;

    if ((file[0] = strtok(path, "/")) == NULL)
        return file[0];

    while ((file[i] = strtok(NULL, "/\n")) != NULL)
        i++;

    return file[i - 1];
}

void doStat(char *name, struct sl_params params) {

    struct stat buffer;
    struct passwd *uname = NULL;
    struct group *gname = NULL;
    char *user, *group, permissions[12], enlace[MAX], acctime[100];
    int n;
    char *print_name = strdup(name);

    if (lstat(name, &buffer) == -1) {
        printf("Cannot access %s: %s\n", name, strerror(errno));
        free(print_name);
        return;
    }

    if (!params.lon) {
        printf("\t%s: %ld\n", file_name(print_name), buffer.st_size);
        free(print_name);
        return;
    }

    // here we get the string of the permissions
    ConvierteModo(buffer.st_mode, permissions);

    if (params.acc)
        strftime(acctime, 100, "%Y/%m/%d-%H:%M", localtime(&buffer.st_atime));
    else
        strftime(acctime, 100, "%Y/%m/%d-%H:%M", localtime(&buffer.st_mtime));

    // here we get the username and the group name

    user = (uname = getpwuid(buffer.st_uid)) == NULL ? "UNKNOWN" : uname->pw_name;

    group = (gname = getgrgid(buffer.st_gid)) == NULL ? "UNKNOWN" : gname->gr_name;

    printf("   %s \t %4ld \t (%8ld) \t %10s \t %10s \t %s \t %9ld \t %s",
           acctime, buffer.st_nlink, buffer.st_ino, user, group, permissions, buffer.st_size, file_name(print_name));

    if (params.link && S_ISLNK(buffer.st_mode)) {
        n = readlink(name, enlace, MAX);
        if (n != -1) {
            enlace[n] = '\0';
            printf("-->%s \n", enlace);
        }
    }
    free(print_name);
    printf("\n");
}

void sl_params_to_tr(struct sl_params params, char *path, char *tro[]) {
    char str[MAX] = " ";
    // this function is used to pass the parameters to the function cmd_stat when called from the print_dir function

    if (params.lon) {
        strcat(str, " -long");
    }

    if (params.acc) {
        strcat(str, " -acc");
    }

    if (params.link) {
        strcat(str, " -link");
    }

    strcat(str, " ");
    strcat(str, path);

    if (TrocearCadena(str, tro) == 0)
        printf("Error at processing the string in the function sl_params_to_tr");
}

int lstat_check(char *path, struct stat *st) {

    if (lstat(path, st) == -1) {
        printf("Could not access %s: %s\n", path, strerror(errno));
        return -1;
    }

    return 0;
}

void print_dir(struct sl_params params, char *path_name) {
    struct stat st;
    char new_path[MAX];

    if (lstat(path_name, &st) == -1) {
        perror("ERROR");
        return;
    }

    printf("************%s\n", path_name);

    if ((st.st_mode & S_IFMT) == S_IFDIR) {

        DIR *d = opendir(path_name);
        struct dirent *ent;

        if (d == NULL) {
            printf("Could not open %s: %s\n", path_name, strerror(errno));
            return;
        }

        while ((ent = readdir(d)) != NULL) {
            if (ent->d_name[0] == '.' && !params.hid)
                continue;
            sprintf(new_path, "%s/%s", path_name, ent->d_name);
            doStat(new_path, params);
        }

        closedir(d);
    } else {
        doStat(path_name, params);
    }
}

void reca(struct sl_params params, char *path_name) {

    struct stat st;
    struct stat dir;

    // gets the status of the path and if it cant it specifies the error and exits the function
    if (lstat_check(path_name, &st) == -1)
        return;

    print_dir(params, path_name);

    if ((st.st_mode & S_IFMT) == S_IFDIR) { // path is a directory
        DIR *d;
        struct dirent *ent;

        if ((d = opendir(path_name)) == NULL) {
            printf("Could not open %s: %s\n", path_name, strerror(errno));
            return;
        }

        // in this while loop we read the elements that are in a directory
        while ((ent = readdir(d)) != NULL) {
            char new_path[MAX];
            if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0) // we ignore these directories
                continue;
            sprintf(new_path, "%s/%s", path_name, ent->d_name);
            if (lstat_check(new_path, &dir) == -1)
                return;
            if (S_ISDIR(dir.st_mode) == 0)
                continue;

            reca(params, new_path);
        }
        closedir(d);
    }
}

void recb(struct sl_params params, char *path_name) {
    struct stat st;
    struct stat dir;
    char new_path[MAX];

    // gets the status of the path and if it cant it specifies the error and exits the function
    if (lstat_check(path_name, &st) == -1)
        return;

    if ((st.st_mode & S_IFMT) == S_IFDIR) { // path is a directory
        DIR *d;
        struct dirent *ent;

        if ((d = opendir(path_name)) == NULL) {
            printf("Could not open %s: %s\n", path_name, strerror(errno));
            return;
        }

        // in this while loop we read the elements that are in a directory
        while ((ent = readdir(d)) != NULL) {
            if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0) // we ignore these directories
                continue;
            sprintf(new_path, "%s/%s", path_name, ent->d_name);
            if (lstat_check(new_path, &dir) == -1)
                return;
            if (S_ISDIR(dir.st_mode) == 0)
                continue;

            recb(params, new_path);
        }

        closedir(d);
    }
    print_dir(params, path_name);
}

void aux_deltree(char *path) {
    struct stat st;

    // gets the status of the path and if it cant it specifies the error and exits the function
    if (lstat_check(path, &st) == -1)
        return;

    if ((st.st_mode & S_IFMT) == S_IFDIR) { // path is a directory
        DIR *d;
        struct dirent *ent;

        if ((d = opendir(path)) == NULL) {
            printf("Could not open %s: %s\n", path, strerror(errno));
            return;
        }

        // in this while loop we read the elements that are in a directory
        while ((ent = readdir(d)) != NULL) {
            char new_path[MAX];

            if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0) // we ignore these directories
                continue;

            sprintf(new_path, "%s/%s", path, ent->d_name);

            // we call the function recursively in all the files and directories
            aux_deltree(new_path);
        }
        closedir(d);
    }

    /*if it is a file or an empty directory (which it is because we have deleted all of its elements in the while loop)
      the function remove deletes it*/
    if (remove(path) == -1) {
        printf("Could not delete %s: %s\n", path, strerror(errno));
    }

    printf("Delete %s\n", path);
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

    struct sl_params params;

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

/* PRACTICA 2*/

void doMalloc(size_t size, tListM *L) {
    void *p;
    if(size <= 0){
        printf("Not posible to allocate 0 or a negative number of bytes\n");
        return;
    }
    p = malloc(size);
    if (p != NULL) {
        InsertElementM(L, p, size, "malloc", 0, " ", 0, 0);
        printf("Inserted %ld bytes at %p\n", size, p);
    } else
        perror("Not enough memory");
}

void *ObtenerMemoriaShmget(key_t clave, size_t tam, tListM *LM) {
    void *p;
    int aux, id, flags = 0777;
    struct shmid_ds s;

    if (tam) /*tam distito de 0 indica crear */
        flags = flags | IPC_CREAT | IPC_EXCL;
    if (clave == IPC_PRIVATE) /*no nos vale*/
    {
        errno = EINVAL;
        return NULL;
    }
    if ((id = shmget(clave, tam, flags)) == -1)
        return (NULL);
    if ((p = shmat(id, NULL, 0)) == (void *) -1) {
        aux = errno;
        if (tam)
            shmctl(id, IPC_RMID, NULL);
        errno = aux;
        return (NULL);
    }
    shmctl(id, IPC_STAT, &s);
    InsertElementM(LM, p, tam, "shared", clave, " ", 0, 0); // InsertarNodoShared (&L, p, s.shm_segsz, clave);
    return (p);
}

void do_AllocateCreateshared(char *tr[], tListM *LM) {
    key_t cl;
    size_t tam;
    void *p;

    if (tr[0] == NULL || tr[1] == NULL) {
        return;
    }

    cl = (key_t) strtoul(tr[0], NULL, 10);
    tam = (size_t) strtoul(tr[1], NULL, 10);
    if (tam == 0) {
        printf("No se asignan bloques de 0 bytes\n");
        return;
    }
    if ((p = ObtenerMemoriaShmget(cl, tam, LM)) != NULL)
        printf("Asignados %lu bytes en %p\n", (unsigned long) tam, p);
    else
        printf("Imposible asignar memoria compartida clave %lu:%s\n", (unsigned long) cl, strerror(errno));
}

void do_Shared(char *tr[], tListM *LM) {
    if (tr[0] != NULL) {
        key_t key = (key_t) strtoul(tr[0], NULL, 10);
        ObtenerMemoriaShmget(key, 0, LM);
    }

}

void *MapearFichero(char *fichero, int protection, tListM *LM) {
    int df, map = MAP_PRIVATE, modo = O_RDONLY;
    struct stat s;
    void *p;

    if (protection & PROT_WRITE)
        modo = O_RDWR;
    if (stat(fichero, &s) == -1 || (df = open(fichero, modo)) == -1)
        return NULL;
    if ((p = mmap(NULL, s.st_size, protection, map, df, 0)) == MAP_FAILED)
        return NULL;
    /*InsertarNodoMmap (&L,p, s.st_size,df,fichero); */
    InsertElementM(LM, p, s.st_size, "mmap", 0, fichero, df, 0);
    return p;
}

void do_AllocateMmap(char *arg[], tListM *LM) {
    char *perm;
    void *p;
    int protection = 0;

    if (arg[0] == NULL) {
        printListM(*LM, 'P');
        return;
    }
    if ((perm = arg[1]) != NULL && strlen(perm) < 4) {
        if (strchr(perm, 'r') != NULL)
            protection |= PROT_READ;
        if (strchr(perm, 'w') != NULL)
            protection |= PROT_WRITE;
        if (strchr(perm, 'x') != NULL)
            protection |= PROT_EXEC;
    }
    if ((p = MapearFichero(arg[0], protection, LM)) == NULL)
        perror("Imposible mapear fichero");
    else
        printf("Fichero %s mapeado en %p\n", arg[0], p);
}

void delMallocSize(tListM *LM, size_t size) {
    if (!isEmptyListM(*LM)) {
        for (tPosLM p = (*LM)->next; p != NULL; p = p->next) {
            if (!strcmp(p->data.allocation_type, "malloc") && (p->data.size == size)) {
                RemoveElementM(LM, p, 0);
                return;
            }
        }
    }
}

void do_DeallocateDelkey(char *args[]) {
    key_t clave;
    int id;
    char *key = args[0];

    if (key == NULL || (clave = (key_t) strtoul(key, NULL, 10)) == IPC_PRIVATE) {
        printf("      delkey necesita clave_valida\n");
        return;
    }
    if ((id = shmget(clave, 0, 0666)) == -1) {
        perror("shmget: imposible obtener memoria compartida");
        return;
    }
    if (shmctl(id, IPC_RMID, NULL) == -1)
        perror("shmctl: imposible eliminar memoria compartida\n");
}

void do_DelShared(char *tr[], tListM *LM) {

    if (tr[0] != NULL) {
        key_t key = (key_t) strtoul(tr[0], NULL, 10);
        tPosLM p;

        for (p = (*LM)->next; p != NULL; p = p->next) {
            if (p->data.key == key) {
                if (shmdt(p->data.address) == -1) {
                    if (shmctl(p->data.shmid, IPC_RMID, NULL) == -1)
                        printf("Imposible to deallocate the shared memory\n");
                }
                RemoveElementM(LM, p, 0);
                break;
            }

        }
    }
}

void do_UnMap(char *tr[], tListM *LM) {

    if (tr[0] == NULL) {
        printListM(*LM, 'P');
        return;
    }

    for (tPosLM p = (*LM)->next; p != NULL; p = p->next) {

        if (!strcmp(tr[0], p->data.file_name)) {
            if (munmap(p->data.address, p->data.size) == -1)
                perror("Error");

            RemoveElementM(LM, p, 0);
            return;
        }
    }

    printf("File %s not mapped\n", tr[0]);
}

void delAtAddres(char *tr[], tListM *LM) {
    void *p;

    if (tr[0] == NULL) {
        printf("There is no address\n");
        return;
    }

    sscanf(tr[0], "%p", &p);

    for (tPosLM pos = (*LM)->next; pos != NULL; pos = pos->next) {
        if (pos->data.address == p) {
            RemoveElementM(LM, pos, 0);
            return;
        }
    }

    printf("Memory %p not assigned with malloc, shared or mmap\n", p);
}

void LlenarMemoria(void *p, size_t cont, unsigned char byte) {
    unsigned char *arr = (unsigned char *) p;
    size_t i;

    for (i = 0; i < cont; i++)
        arr[i] = byte;
}

ssize_t LeerFichero(char *f, void *p, size_t cont) {
    struct stat s;
    ssize_t n;
    int df, aux;

    if (stat(f, &s) == -1 || (df = open(f, O_RDONLY)) == -1)
        return -1;
    if (cont == -1) /* si pasamos -1 como bytes a leer lo leemos entero*/
        cont = s.st_size;
    if ((n = read(df, p, cont)) == -1) {
        aux = errno;
        close(df);
        errno = aux;
        return -1;
    }
    close(df);
    return n;
}

void do_I_O_read(char *ar[]) {
    void *p;
    size_t cont = -1;
    ssize_t n;
    if (ar[0] == NULL || ar[1] == NULL) {
        printf("Parameters missing\n");
        return;
    }

    sscanf(ar[1], "%p", &p); /*convertimos de cadena a puntero*/
    if (ar[2] != NULL)
        cont = (size_t) atoll(ar[2]);

    if ((n = LeerFichero(ar[0], p, cont)) == -1)
        perror("Imposible to read the file");
    else
        printf("Read %lld bytes of %s in %p\n", (long long) n, ar[0], p);
}

ssize_t EscribirFichero(char *f, void *p, size_t cont, int overwrite) {
    ssize_t n;
    int df, aux, flags = O_CREAT | O_EXCL | O_WRONLY;

    if (overwrite)
        flags = O_CREAT | O_WRONLY | O_TRUNC;

    if ((df = open(f, flags, 0777)) == -1)
        return -1;

    if ((n = write(df, p, cont)) == -1) {
        aux = errno;
        close(df);
        errno = aux;
        return -1;
    }
    close(df);
    return n;
}

void do_I_O_write(char *tr[]) {
    void *p;
    size_t cont;
    ssize_t n;
    int overwrite;

    if (tr[0] == NULL) {
        printf("Missing arguments\n");
        return;
    }

    if (!strcmp(tr[0], "-o")) {
        if (tr[1] != NULL && tr[2] != NULL && tr[3] != NULL) {
            overwrite = 1;
            sscanf(tr[2], "%p", &p);
            cont = strtoul(tr[3], NULL, 10);
            EscribirFichero(tr[1], p, cont, overwrite);
        }
    } else {
        if (tr[1] != NULL && tr[2] != NULL) {
            overwrite = 0;
            sscanf(tr[1], "%p", &p);
            cont = (size_t) strtoul(tr[2], NULL, 10);
            if((n =EscribirFichero(tr[0], p, cont, overwrite)) == -1){
                perror("Imposible to write the file");
            }else{
                printf("Wrote %lld bytes from %p in %s\n", (long long) n, p, tr[0]);
            }
        }
    }
}

void dumpMemory(void *p, size_t cont) {
    unsigned char *arr = (unsigned char *) p;
    size_t i;
    size_t j;
    size_t aux = 0;

    for (i = 0; i < cont; i++) {
        if (arr[i] != '0') {
            printf(" %c ", arr[i]);
        } else {
            printf("  ");
        }

        if (((i + 1) % 25 == 0) && (i != 0)) {

            printf("\n");

            for (j = aux; j <= i; j++) {
                printf("%2x ", arr[j]);

            }

            printf("\n");
            aux = i + 1;

        }
    }

    cont = cont % 25;

    if (cont != 0) {

        printf("\n");

        for (j = 0; j < cont; j++) {
            printf("%2x ", arr[j]);

        }
        printf("\n");

    }

    printf("\n");

}

void Do_pmap(void) /*sin argumentos*/
{
    pid_t pid; /*hace el pmap (o equivalente) del proceso actual*/
    char elpid[32];
    char *argv[4] = {"pmap", elpid, NULL};

    sprintf(elpid, "%d", (int) getpid());
    if ((pid = fork()) == -1) {
        perror("Imposible crear proceso");
        return;
    }
    if (pid == 0) {
        if (execvp(argv[0], argv) == -1)
            perror("cannot execute pmap (linux, solaris)");

        argv[0] = "procstat";
        argv[1] = "vm";
        argv[2] = elpid;
        argv[3] = NULL;
        if (execvp(argv[0], argv) == -1) /*No hay pmap, probamos procstat FreeBSD */
            perror("cannot execute procstat (FreeBSD)");

        argv[0] = "procmap", argv[1] = elpid;
        argv[2] = NULL;
        if (execvp(argv[0], argv) == -1) /*probamos procmap OpenBSD*/
            perror("cannot execute procmap (OpenBSD)");

        argv[0] = "vmmap";
        argv[1] = "-interleave";
        argv[2] = elpid;
        argv[3] = NULL;
        if (execvp(argv[0], argv) == -1) /*probamos vmmap Mac-OS*/
            perror("cannot execute vmmap (Mac-OS)");
        exit(1);
    }
    waitpid(pid, NULL, 0);
}

void Recursiva(int n) {
    char automatico[TAMANO];
    static char estatico[TAMANO];

    printf("parametro:%3d(%p) array %p, arr estatico %p\n", n, &n, automatico, estatico);

    if (n > 0)
        Recursiva(n - 1);
}

void cmd_allocate(char *tr[], tListM *LM) {
    size_t size;
    if (tr[0] == NULL) {
        printListM(*LM, 'a'); //prints all the list
    } else {
        if (!strcmp(tr[0], "-malloc")) {
            if (tr[1] == NULL)
                printListM(*LM, 'M');
            else {

                if ((size = (size_t) strtoul(tr[1], NULL, 10)) > 0)
                    doMalloc(size, LM);
                else
                    printf("Can't allocote 0 bytes or a negative number of bytes\n");

            }
        } else if (!strcmp(tr[0], "-createshared")) {
            if (tr[1] == NULL || tr[2] == NULL)
                printListM(*LM, 'S');
            else {
                do_AllocateCreateshared(tr + 1, LM);
            }
        } else if (!strcmp(tr[0], "-shared")) {
            if (tr[1] == NULL) {
                printListM(*LM, 'S');
            } else {
                do_Shared(tr + 1, LM);
            }

        } else if (!strcmp(tr[0], "-mmap")) {
            do_AllocateMmap(tr + 1, LM);
        }

    }
}

void cmd_deallocate(char *tr[], tListM *LM) {
    size_t size;
    if (tr[0] == NULL) {
        printListM(*LM, 'a'); //prints all the list
    } else {
        if (!strcmp(tr[0], "-malloc")) {
            if (tr[1] == NULL)
                printListM(*LM, 'M');
            else {

                if ((size = (size_t) atoi(tr[1])) > 0)
                    delMallocSize(LM, size);
                else
                    printf("Can't deallocote a block with 0 bytes or a negative number of bytes\n");

            }
        } else if (!strcmp(tr[0], "-delkey")) {
            if (tr[1] == NULL)
                printListM(*LM, 'S');
            else {
                do_DeallocateDelkey(tr + 1);
            }
        } else if (!strcmp(tr[0], "-shared")) {
            if (tr[1] == NULL) {
                printListM(*LM, 'S');
            } else {
                do_DelShared(tr + 1, LM);
            }

        } else if (!strcmp(tr[0], "-mmap")) {
            do_UnMap(tr + 1, LM);
        } else {
            delAtAddres(tr, LM);
        }

    }

}

void cmd_memfill(char *tr[]) {

    void *mem;
    size_t cont = 128; 
    unsigned char byte = 'A';

    if (!(tr[0] == NULL)) {
        sscanf(tr[0], "%p", &mem);
        
        if(tr[1] == NULL || tr[2] == NULL){
            printf("Filling %zu bytes of memory with the byte %c(%x) from the memory position %p\n", cont, byte, byte, mem);
            LlenarMemoria(mem, cont, byte);
        }else{
            cont = (size_t) strtoul(tr[1], NULL, 10);

            if(tr[2][0] == '\''){
                if(tr[2][1] != '\0')
                    byte = (unsigned char) tr[2][1];
            }else{
                byte = (unsigned char) strtoul(tr[2], NULL, 10);
            }
            printf("Filling %zu bytes of memory with the byte %c(%x) from the memory position %p\n", cont, byte, byte, mem);
            LlenarMemoria(mem, cont, byte);
        }
        

    }
}

void cmd_i_o(char *tr[]) {
    if (tr[0] == NULL) {
        printf("Missing parameters\n");
        return;
    }
    if (!strcmp(tr[0], "read")) {
        do_I_O_read(tr + 1);
    } else if (!strcmp(tr[0], "write")) {
        do_I_O_write(tr + 1);
    }

}

void cmd_memdump(char *tr[]) {
    void *mem;
    size_t cont; 
   
    if (!(tr[0] == NULL)) {
        //mem = (void *)strtoul(tr[0], &ptr, 0);
        sscanf(tr[0], "%p", &mem);
        if(tr[1] == NULL){
            cont = 25;
        }else{
            cont = (size_t) strtoul(tr[1], NULL, 10);
        }
        
        printf("Dumping %zu byte of memory from memory position %p\n", cont, mem);
        dumpMemory(mem, cont);

    }

}

int glob1 = 1;
int glob2 = 2;
int glob3 = 3;

void cmd_memory(char *tr[], tListM LM) {
    int x, y, z;
    static int x1, y1, z1;

    if (tr[0] == NULL || strcmp(tr[0], "-all") == 0 || !strcmp(tr[0], "-vars")) {

        printf("Local variables   \t%p,\t%p,\t%p\n", &x, &y, &z);
        printf("Global variables   \t%p,\t%p,\t%p\n", &glob1, &glob2, &glob3);
        printf("Static variables   \t%p,\t%p,\t%p\n", &x1, &y1, &z1);

    }

    if (tr[0] == NULL || strcmp(tr[0], "-all") == 0 || !strcmp(tr[0], "-funcs")) {

        printf("Program functions \t%p,\t%p,\t%p\n", cmd_autores, cmd_ayuda, cmd_pid);
        printf("Library functions \t%p,\t%p,\t%p\n", stat, open, remove);


    }

    if (tr[0] != NULL && !strcmp(tr[0], "-pmap")) {

        Do_pmap();

    }

    if (tr[0] == NULL || strcmp(tr[0], "-all") == 0 || !strcmp(tr[0], "-blocks")) {

        printListM(LM, 'A');

    }
}

void cmd_recursiva(char *tr[]) {
    if (tr[0] != NULL) {
        Recursiva(atoi(tr[0]));
    }
}

//lab 3


char * Ejecutable (char *s)
{
	char path[MAX];
	static char aux2[MAX];
	struct stat st;
	char *p;
	if (s==NULL || (p=getenv("PATH"))==NULL)
		return s;
	if (s[0]=='/' || !strncmp (s,"./",2) || !strncmp (s,"../",3))
        return s;       /*is an absolute pathname*/
	strncpy (path, p, MAX);
	for (p=strtok(path,":"); p!=NULL; p=strtok(NULL,":")){
       sprintf (aux2,"%s/%s",p,s);
	   if (lstat(aux2,&st)!=-1)
		return aux2;
	}
	return s;
}


int OurExecvpe(char *file, char *const argv[], char *const envp[])
{
   return (execve(Ejecutable(file),argv, envp));
}


void cmd_priority(char *tr[]){
    int which = PRIO_PROCESS;
    int pid;
    int prio;
    errno = 0;

    if(tr[0] == NULL){
        pid = getpid();
        prio = getpriority(which, pid);
        
        if(prio == -1 && errno != 0){
            perror("Can not show the priority");
            return;
        }
        printf("The priority of the process %d is %d\n", pid, prio);
    }else{
        pid = (int) strtoul(tr[0], NULL, 10);
        if(tr[1] == NULL){
            prio = getpriority(which, pid);
            
            if(prio == -1 && errno != 0){
                perror("Can not show the priority");
                return;
            }
            printf("The priority of the process %d is %d\n", pid, prio);
        }else{
            prio = (int) strtoul(tr[1], NULL, 10);

            if(setpriority(which, pid, prio) == -1){
                perror("Can not change the priority of the process");
            }
        }
    }

}

int BuscarVariable (char * var, char *e[])  /*busca una variable en el entorno que se le pasa como parametro*/
{
  int pos=0;
  char aux[MAX];
  
  strcpy (aux,var);
  strcat (aux,"=");
  
  while (e[pos]!=NULL)
    if (!strncmp(e[pos],aux,strlen(aux)))
      return (pos);
    else 
      pos++;
  errno=ENOENT;   /*no hay tal variable*/
  return(-1);
}


void onlyTheName(char *str){
    int i = 0;
    while(str[i] != '\0'){
        if(str[i] == '='){
            str[i] = '\0';
        }
        i++;
    }
}
//preguntar que es lo que se printea y getenv
void cmd_showvar(char *tr[], char *arg3[]){
    char **env = __environ;
    char *variable, *var;
    int posVar;
    int i = 0;
    if(tr[0] == NULL){
        while(arg3[i] != NULL){
            printf("%p->main argv3[%d]=(%p) %s\n", &arg3[i], i, arg3[i], arg3[i]);
            i++;
        }
    }else{
        if((posVar = BuscarVariable(tr[0], env)) == -1){
            perror("");
        }else{
            variable = strdup(arg3[posVar]);
            onlyTheName(variable);
            var = getenv(variable);
            printf("With arg3 %s (%p)  @%p\n", arg3[posVar], arg3[posVar], &arg3[posVar]);
            printf("With arg3 %s (%p)  @%p\n", env[posVar], env[posVar], &env[posVar]);
            printf("With getenv %s %p\n", var, var);
            free(variable);

        }

    }
}

int CambiarVariable(char * var, char * valor, char *e[]) 
{                                                       
  int pos;
  char *aux;
   
  if ((pos=BuscarVariable(var,e))==-1)
    return(-1);
 
  if ((aux=(char *)malloc(strlen(var)+strlen(valor)+2))==NULL)
	return -1;
  strcpy(aux,var);
  strcat(aux,"=");
  strcat(aux,valor);
  e[pos]=aux;
  return (pos);
}

void cmd_changevar(char *tr[], char *argv3[]){
    if(tr[0] != NULL && tr[1] != NULL && tr[2] != NULL){
        if(!strcmp(tr[0], "-a")){
            if(CambiarVariable(tr[1], tr[2], argv3) == -1)
                perror("Imposible to change variable");
            return;
        }else if(!strcmp(tr[0], "-e")){
            if(CambiarVariable(tr[1], tr[2], __environ) == -1)
                perror("Imposible to change variable");
            return;
        }else if(!strcmp(tr[0], "-p")){
            char var[MAX] = "";
            strcpy(var, tr[1]);
            strcat(var, "=");
            strcat(var, tr[2]);
            
            if(putenv(var) != 0)
                perror("Imposible to change variable");
            return;
        }
    }

    printf("Use: changevar [-a | -e | -p] var value\n");
}

void cmd_environ(char *tr[], char *argv3[]){
    if(tr[0] == NULL){
        cmd_showvar(tr, argv3);
    }else if(!strcmp(tr[0], "-environ")){
        int i = 0;
        while(__environ[i] != NULL){
            printf("%p->main env[%d]=(%p) %s\n", &__environ[i], i, __environ[i], __environ[i]);
            i++;
        }
    }else if(!strcmp(tr[0], "-addr")){
        printf("Environ: %p (stored at %p)\n", __environ, &__environ);
        printf("Main argv3: %p (stored at %p)\n", argv3, &argv3);

    }else{
        printf("Use: environ [-environ | -addr]\n");
    }
}

void cmd_fork (char *tr[], tListP *LP)
{
	pid_t pid;
	
	if ((pid=fork())==0){
        clearListP(LP);//clears the process list
		printf ("ejecutando proceso %d\n", getpid());
	}
	else if (pid!=-1)
		waitpid (pid,NULL,0);
}

void cmd_listjobs(char *tr[], tListP LP){
    printListP(LP);
}

void cmd_job(char *tr[], tListP *LP){
    int pid, st;
    if(tr[0] == NULL){
        printListP(*LP);
    }else{
        if(!strcmp(tr[0], "-fg") && tr[1] != NULL){//the parent process wait for the child to finish and then eliminate it from the list
            pid = strtoul(tr[1], NULL, 10);
            if(processExist(pid, *LP)){
                waitpid(pid, &st, 0);
                removeAtPidP(pid, LP);
            }else{
                printf("ERROR: The process doesn't exist\n");
            }
        }else if(strcmp(tr[0], "-fg")){//prints information about the child process
            pid = strtoul(tr[0], NULL, 10);
            printInfoByPid(pid, *LP);
        }
    }
}

void cmd_deljobs(char *tr[], tListP *LP){
    if(tr[0] == NULL){
        printListP(*LP);
    }else if(!strcmp(tr[0], "-term")){
        clearListTerminatedSignaledP(LP, false);
    }else if(!strcmp(tr[0], "-sig")){
        clearListTerminatedSignaledP(LP, true);
    }
}

void execute(char *tr[], tList *L, tListM *LM, tListP *LP){
    char *envp[MAX/2], *argv[MAX/2];
    int i = 0, executablePos, ret, pos;

    //adding the enviroment variables that were specified to envp
    while((pos = BuscarVariable(tr[i], __environ)) != -1){
        envp[i] = __environ[pos];   
        i++;
    }
    envp[i] = NULL; //the last element has to be NULL
    
    executablePos = i;

    while(tr[i] != NULL){
        argv[i-executablePos] = tr[i];
        if(strcmp("&", tr[i]) == 0 || tr[i][0] == '@'){
            break;
        }
        i++;
    }
    argv[i-executablePos] = NULL;
    

    if(envp[0] == NULL){//if no enviroment variables were specified then all of them are passed
        ret = OurExecvpe(tr[executablePos], argv, __environ);     
    }else{
        ret = OurExecvpe(tr[executablePos], argv, envp);
    }
        

    if(ret == -1){
        perror("ERROR");
        return;
    }

}

char * checkPrio(char *tr[]){
    for(int i = 0; tr[i] != NULL; i++){
        if(tr[i][0] == '@'){
            return tr[i] + 1;
        }
    }
    return NULL;
}

bool checkBackground(char *tr[]){
    for(int i = 0; tr[i] != NULL; i++){
        if(!strcmp(tr[i], "&")){
            return true;
        }
    }
    return false;
}


void cmd_execute(char *tr[], bool isExecute, tList *L, tListM *LM, tListP *LP){
    char *prio = checkPrio(tr); //we get the priority
    bool background = checkBackground(tr); //we check if is a background process
    int pid_son;

    //if if the comand 'execute' just executes the programe pased in the arguments
    if(isExecute){
        execute(tr, L, LM, LP);
        return;
    }

    //If is not the command 'execute' the process is executed in the son process
    if((pid_son = fork()) == 0){
       execute(tr, L, LM, LP); 
    }else{
        if(background){//if it is a background proccess it is inserted in the list
            InsertElementP(LP, pid_son, tr);
        }else{//else it waits untill it has finished
            int st;
            waitpid(pid_son, &st, 0);
        }

        if(prio != NULL){//if there is a priority it is set 
            setpriority(PRIO_PROCESS, pid_son, (int) strtoul(prio, NULL, 10));
        }
    }

}





// TROCEAR Y PROCESAR


int TrocearCadena(char *cadena, char *trozos[]) {
    int i = 1;

    if ((trozos[0] = strtok(cadena, " \n\t")) == NULL)
        return 0;
    while ((trozos[i] = strtok(NULL, " \n\t")) != NULL)
        i++;
    return i;
}

void ProcesarEntrada(char *tr[], char *argv3[], tList *L, tListM *LM, tListP *LP) {
    int i;
    if (tr[0] == NULL)
        return;
    
    if (!strcmp(tr[0], "hist")) {
        cmd_hist(tr + 1, L);
    } else if (!strcmp(tr[0], "comando")) {
        cmd_command(tr + 1, argv3, L, LM, LP);
    } else if (!strcmp(tr[0], "fin") || !strcmp(tr[0], "bye") || !strcmp(tr[0], "salir"))
        cmd_fin(tr + 1, L, LM, LP);
    else if (!strcmp(tr[0], "allocate"))
        cmd_allocate(tr + 1, LM);
    else if (!strcmp(tr[0], "memory"))
        cmd_memory(tr + 1, *LM);
    else if (!strcmp(tr[0], "deallocate"))
        cmd_deallocate(tr + 1, LM);
    else if(!strcmp(tr[0], "showvar"))
        cmd_showvar(tr + 1, argv3);
    else if(!strcmp(tr[0], "changevar"))
        cmd_changevar(tr + 1, argv3);
    else if(!strcmp(tr[0], "showenv"))
        cmd_environ(tr + 1, argv3);
    else if(!strcmp(tr[0], "fork"))
        cmd_fork(tr + 1, LP);
    else if(!strcmp(tr[0], "jobs") || !strcmp(tr[0], "listjobs"))
        cmd_listjobs(tr + 1, *LP);
    else if(!strcmp(tr[0], "job"))
        cmd_job(tr + 1, LP);
    else if(!strcmp(tr[0], "deljobs"))
        cmd_deljobs(tr + 1, LP);
    else if(!strcmp(tr[0], "execute"))
        cmd_execute(tr + 1, true, L, LM, LP);
        
    else {
        for (i = 0; c[i].name != NULL; i++) {//here we check for the rest of the commands
            if (!strcmp(c[i].name, tr[0])) {

                (*c[i].func)(tr + 1);
                return;
            }
        }
       
       //if none of the shell comand was executed we try to execute an external programe
        cmd_execute(tr, false, L, LM, LP);
      
               
    }
}


int main(int argc, char *argv[], char *argv3[]) {
    char entrada[MAX];
    char *trozos[MAX / 2];
    tList L;
    tListM LM;
    tListP LP;

    CreateEmptyList(&L);
    CreateEmptyListM(&LM);
    CreateEmptyListP(&LP);
    while (1) {
        printf("->");
        fgets(entrada, MAX, stdin);
        InsertElement(&L, entrada);
        if (TrocearCadena(entrada, trozos) == 0)
            continue;


        ProcesarEntrada(trozos, argv3, &L, &LM, &LP);

    }
}
