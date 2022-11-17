#ifndef SHELL_COMMANDS_H
#define SHELL_COMMANDS_H

#include "libraries-headers.h"


void cmd_autores(char *tr[]);

void cmd_carpeta(char *tr[]);

void cmd_ayuda(char *tr[]);

void cmd_pid(char *tr[]);

void cmd_fin(char *tr[], tList *L,  tListM *LM);

void cmd_hist(char *tr[], tList *L);

void cmd_command(char *tr[], tList *L, tListM *LM);

void cmd_fecha(char *tr[]);

void cmd_infosis(char *tr[]);

void cmd_create(char *tr[]);

void cmd_stat(char *tr[]);

void cmd_list(char *tr[]);

void cmd_delete(char *tr[]);

void cmd_deltree(char *tr[]);

void cmd_allocate(char *tr[], tListM *LM);

void cmd_deallocate(char *tr[], tListM *LM);

void cmd_i_o(char *tr[]);

void cmd_memfill(char *tr[]);

void cmd_memdump(char *tr[]);

void cmd_memory(char *tr[], tListM LM);

void cmd_recursiva(char *tr[]);

#endif //SHELL_COMMANDS_H
