/*
 Diego Dopazo Garcia : diego.dopazo.garcia@udc.es
 Marcos Seco Anderson: marcos.seco.anderson@udc.es
 */

#include "libraries-headers.h"


struct CMD c[] = {
        {"autores", "autores [-n|-l]\tDisplays the names &/or of the authors",                                                                                                                                                                                        cmd_autores},
        {"carpeta", "carpeta [dir]\tChanges (or Displays) the current shell directory",                                                                                                                                                                               cmd_carpeta},
        {"ayuda",   "ayuda [cmd]\tDisplays help for all commands\n""\t\tcmd: cmd command info",                                                                                      cmd_ayuda},
        {"pid",     "pid [-p]\tDisplays the pid of the shell or the pid of its father",                                                                                                                                                                               cmd_pid},
        {"fecha",   "fecha [-d|-h\tDisplays the date or the current time",                                                                                                                                                                                            cmd_fecha},
        {"infosis", "infosis \tDisplays information about the machine where the shell is running in",                                                                                                                                                                 cmd_infosis},
        {"create",  "create [-f] [name]\tCreates a directory or a file (-f)",                                                                                                                                                                                         cmd_create},
        {"stat",    "stat [-long] [-acc] [-link] name1 name2 ...   lists files; \n\t\t\t-long: long listing \n\t\t\t-acc: acesstime\n\t\t\t-link: if the link is symbolic, the path is contained",                                                                    cmd_stat},
        {"list",    "list [-reca] [-recb] [-hid] [-long] [-acc] [-link] n1 n2 ... lists the content of the directories; \n\t\t\t-hid: includes hidden files\n\t\t\t-reca: recursive (before) \n\t\t\t-recb: recursive (after)\n\t\t\trest of parameters as in stat.", cmd_list},
        {"deltree", "deltree [name1 name2 ...] Deletes non empty directories recursively",                                                                                                                                                                            cmd_deltree},
        {"delete",  "delete [name1 name2 ...] Deletes empty files and directories",                                                                                                                                                                                   cmd_delete},
        {"memfill", "memfill addr cont byte   Fills the mememory since addr with byte", cmd_memfill},
        {"memdump", "memdump addr cont   Shows the contents of addr in screen", cmd_memdump},
        {"recursiva", "recursiva [n] Invoques the recursive function n times", cmd_recursiva},
        {"i-o", "i-o [read|write] [-o] fiche addr cont\n\t\t read fiche addr cont: Reads cont bytes from fiche to addr\n\t\t write [-o] fich addr cont: Writes cont bytes from addr to fich. -o for overwrite\n\t\t\t addr is a memory address\n", cmd_i_o},
        {NULL, NULL, NULL}
};

int TrocearCadena(char *cadena, char *trozos[]) {
    int i = 1;

    if ((trozos[0] = strtok(cadena, " \n\t")) == NULL)
        return 0;
    while ((trozos[i] = strtok(NULL, " \n\t")) != NULL)
        i++;
    return i;
}

void ProcesarEntrada(char *tr[], tList *L, tListM *LM) {
    int i;
    if (tr[0] == NULL)
        return;
    if (!strcmp(tr[0], "hist")) {//here we check for the hist command
        cmd_hist(tr + 1, L);

        return;
    } else if (!strcmp(tr[0], "comando")) {//here we check the command
        cmd_command(tr + 1, L, LM);
        return;

    } else if (!strcmp(tr[0], "fin") || !strcmp(tr[0], "bye") || !strcmp(tr[0], "salir"))
        cmd_fin(tr + 1, L, LM);

    else if(!strcmp(tr[0], "allocate"))
        cmd_allocate(tr + 1, LM);
    else if(!strcmp(tr[0], "memory"))
        cmd_memory(tr + 1, *LM);
    else if(!strcmp(tr[0], "deallocate"))
        cmd_deallocate(tr + 1, LM);
    else{
        for (i = 0; c[i].name != NULL; i++) {//here we check for the rest of the commands
            if (!strcmp(c[i].name, tr[0])) {

                (*c[i].func)(tr + 1);
                return;
            }
        }
        printf("Command not found\n");

    }
}

int main(int argc, char *argv[]) {
    char entrada[MAX];
    char *trozos[MAX / 2];
    tList L;
    tListM LM;
    
    CreateEmptyList(&L);
    CreateEmptyListM(&LM);

    while (1) {
        printf("->");
        fgets(entrada, MAX, stdin);
        InsertElement(&L, entrada);
        if (TrocearCadena(entrada, trozos) == 0)
            continue;
        ProcesarEntrada(trozos, &L, &LM);

    }
}
