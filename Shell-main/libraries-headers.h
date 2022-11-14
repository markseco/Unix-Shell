#ifndef SHELL_LIBRARIES_HEADERS_H
#define SHELL_LIBRARIES_HEADERS_H

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
#include <sys/shm.h>
#include "list_blocks.h"
#include "head_list.h"
#include "commands.h"
#include "AuxiliarFunctions.h"


//definition of the structure that is used to store the name of the command its use and the function that implements it
struct CMD {
    char *name;
    char *ayuda;

    void (*func)(char **);
};

extern struct CMD c[];
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

void ProcesarEntrada(char *tr[], tList *L, tListM *LM);

int TrocearCadena(char *cadena, char *trozos[]);

#endif //SHELL_LIBRARIES_HEADERS_H
