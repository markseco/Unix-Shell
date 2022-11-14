#ifndef SHELL_AUXILIARFUNCTIONS_H
#define SHELL_AUXILIARFUNCTIONS_H


#include "libraries-headers.h"


char LetraTF(mode_t m);

char *ConvierteModo(mode_t m, char *permisos);

void aux_deltree(char *path);

struct sl_params aux_stat_list(char *tr[]);

int length_name_file(char *tr);

void format_str(int size, char *name1, char *name);

void doStat(char *name, struct sl_params params);

void sl_params_to_tr(struct sl_params params, char *path, char *tro[]);

int lstat_check(char *path, struct stat *st);

void print_dir(struct sl_params params, char *path_name);

void reca(struct sl_params params, char *path_name);

void recb(struct sl_params params, char *path_name);

char *file_name(char *path);

void doMalloc(size_t size, tListM *L);

void * ObtenerMemoriaShmget (key_t clave, size_t tam, tListM * LM);

void do_AllocateCreateshared (char *tr[], tListM * LM);

void LlenarMemoria (void *p, size_t cont, unsigned char byte);

void dumpMemory (void *p, size_t cont);

void Recursiva (int n);

#endif //SHELL_AUXILIARFUNCTIONS_H
