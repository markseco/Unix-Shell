#include "AuxiliarFunctions.h"
#include "list_blocks.h"
#include "libraries-headers.h"
#define TAMANO 2048


char LetraTF (mode_t m)
{
    switch (m&S_IFMT) { /*and bit a bit con los bits de formato,0170000 */
        case S_IFSOCK: return 's'; /*socket */
        case S_IFLNK: return 'l'; /*symbolic link*/
        case S_IFREG: return '-'; /* normal file*/
        case S_IFBLK: return 'b'; /*block device*/
        case S_IFDIR: return 'd'; /*directory */
        case S_IFCHR: return 'c'; /*char device*/
        case S_IFIFO: return 'p'; /*pipe*/
        default: return '?'; /*unknown, shouldn't appear*/
    }
}


char * ConvierteModo (mode_t m, char *permisos)
{
    strcpy (permisos,"---------- ");

    permisos[0]=LetraTF(m);
    if (m&S_IRUSR) permisos[1]='r';    /*owner*/
    if (m&S_IWUSR) permisos[2]='w';
    if (m&S_IXUSR) permisos[3]='x';
    if (m&S_IRGRP) permisos[4]='r';    /*group*/
    if (m&S_IWGRP) permisos[5]='w';
    if (m&S_IXGRP) permisos[6]='x';
    if (m&S_IROTH) permisos[7]='r';    /*rest*/
    if (m&S_IWOTH) permisos[8]='w';
    if (m&S_IXOTH) permisos[9]='x';
    if (m&S_ISUID) permisos[3]='s';    /*setuid, setgid y sticky bit*/
    if (m&S_ISGID) permisos[6]='s';
    if (m&S_ISVTX) permisos[9]='t';

    return permisos;
}

struct sl_params aux_stat_list(char *tr[]){
    //this function returns a structure of type sl_params

    struct sl_params ret;
    ret.lon = false;
    ret.acc = false;
    ret.link = false;
    ret.hid = false;
    ret.reca = false;
    ret.recb = false;
    ret.start_of_files = 0;
    int i = 0;

    if(tr == NULL)
        return ret;

    while(tr[i] != NULL){
        if(strlen(tr[i]) <= 0)
            return ret;
        else if(tr[i][0] != '-')
            return ret;
        else if(!strcmp("-long", tr[i]))
            ret.lon = true;
        else if(!strcmp("-acc", tr[i]))
            ret.acc = true;
        else if(!strcmp("-link", tr[i]))
            ret.link = true;
        else if(!strcmp("-hid", tr[i]))
            ret.hid = true;
        else if(!strcmp("-reca", tr[i]))
            ret.reca = true;
        else if(!strcmp("-recb", tr[i]))
            ret.recb = true;

        ret.start_of_files++;
        i++;
    }

    return ret;
}


int length_name_file(char *tr){
    int i = strlen(tr);
    int j = i - 1;

    if(tr == NULL)
        return 0;
    if(j <= 0)
        return 0;

    while(tr[j] != '/'){
        j--;
        if(j <= 0)
            return 0;
    }
    return (i - j);
}

void format_str(int size, char *name1, char *name){
    int sf;
    if(size != 0){
        sf = strlen(name1) - size + 1;
        strncpy(name, name1 + sf, size);
        name[size] = '\0';
    }else{
        name = strdup(name1);
    }

}

char *file_name(char *path){
    char *file[MAX/2];
    int i = 1;

    if ((file[0] = strtok(path,"/"))==NULL)
        return file[0];

    while ((file[i]=strtok(NULL,"/\n"))!=NULL)
        i++;

    return file[i - 1];
}

void doStat(char *name, struct sl_params params){

    struct stat buffer;
    struct passwd *uname = NULL;
    struct group *gname = NULL;
    char *user,*group, permissions[12], enlace[MAX],acctime[100];
    int n;
    char *print_name = strdup(name);

    if(lstat(name, &buffer) == -1){
        printf ("Cannot access %s: %s\n", name,strerror(errno));
        return;
    }

    if (!params.lon){
        printf ("\t%s: %ld\n", file_name(print_name), buffer.st_size);
        return;
    }

    //here we get the string of the permissions
    ConvierteModo (buffer.st_mode, permissions);

    if(params.acc)
        strftime(acctime, 100, "%Y/%m/%d-%H:%M",localtime(&buffer.st_atime));
    else
        strftime(acctime, 100, "%Y/%m/%d-%H:%M",localtime(&buffer.st_mtime));

    //here we get the username and the group name

    user=(uname = getpwuid(buffer.st_uid)) == NULL?"UNKNOWN":uname->pw_name;

    group=(gname = getgrgid(buffer.st_gid)) == NULL?"UNKNOWN":gname-> gr_name;

    printf("   %s \t %4ld \t (%8ld) \t %10s \t %10s \t %s \t %9ld \t %s",
           acctime, buffer.st_nlink, buffer.st_ino, user, group, permissions, buffer.st_size, file_name(print_name));

    if(params.link && S_ISLNK(buffer.st_mode)){
        n=readlink(name, enlace,MAX);
        if (n!=-1){
            enlace[n]='\0';
            printf ("-->%s \n",enlace);
        }
    }
    free(print_name);
    printf("\n");

}


void sl_params_to_tr(struct sl_params params,  char * path, char *tro[]){
    char str[MAX] = " ";
    //this function is used to pass the parameters to the function cmd_stat when called from the print_dir function

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

int lstat_check(char *path, struct stat *st){

    if(lstat(path, st) == -1) {
        printf("Could not access %s: %s\n", path, strerror(errno));
        return -1;
    }

    return 0;
}

void print_dir(struct sl_params params, char *path_name){
    struct stat st;
    char new_path[MAX];

    if(lstat(path_name, &st) == -1){
        perror("ERROR");
        return;
    }

    printf("************%s\n", path_name);

    if((st.st_mode & S_IFMT) == S_IFDIR){

        DIR *d = opendir(path_name);
        struct dirent *ent;

        if(d == NULL) {
            printf("Could not open %s: %s\n", path_name, strerror(errno));
            return;
        }

        while((ent = readdir(d)) != NULL){
            if(ent->d_name[0] == '.' && !params.hid)
                continue;
            sprintf(new_path, "%s/%s", path_name, ent->d_name);
            doStat(new_path, params);
        }

        closedir(d);
    }else{
        doStat(path_name, params);
    }
}



void reca(struct sl_params params, char *path_name){

    struct stat st;
    struct stat dir;

    //gets the status of the path and if it cant it specifies the error and exits the function
    if(lstat_check(path_name, &st) == -1)
        return;


    print_dir(params, path_name);

    if((st.st_mode & S_IFMT) == S_IFDIR) { // path is a directory
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


            reca(params, new_path);

        }
        closedir(d);

    }
}


void recb(struct sl_params params, char *path_name){
    struct stat st;
    struct stat dir;
    char new_path[MAX];

    //gets the status of the path and if it cant it specifies the error and exits the function
    if(lstat_check(path_name, &st) == -1)
        return;

    if((st.st_mode & S_IFMT) == S_IFDIR) { // path is a directory
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

            recb(params, new_path);

        }

        closedir(d);

    }
    print_dir(params, path_name);

}

void aux_deltree(char *path) {
    struct stat st;


    //gets the status of the path and if it cant it specifies the error and exits the function
    if(lstat_check(path, &st) == -1)
        return;


    if((st.st_mode & S_IFMT) == S_IFDIR) { // path is a directory
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

    /*if it is a file or an empty directory (which it is because we have deleted all of its elements in the while loop)
      the function remove deletes it*/
    if(remove(path) == -1) {
        printf("Could not delete %s: %s\n", path, strerror(errno));
    }

    printf("Delete %s\n", path);
}

void doMalloc(size_t size, tListM *L){
    void *p;
    p = malloc(size);
    if(p != NULL){
        InsertElementM(L, p, size, "malloc", 0, " ", 0);
        printf("Inserted %ld bytes at %p\n", size, p);
    }else
        perror("Not enough memory");
}

void * ObtenerMemoriaShmget (key_t clave, size_t tam, tListM * LM)
{
    void * p;
    int aux,id,flags=0777;
    struct shmid_ds s;

    if (tam)     /*tam distito de 0 indica crear */
        flags=flags | IPC_CREAT | IPC_EXCL;
    if (clave==IPC_PRIVATE)  /*no nos vale*/
        {errno=EINVAL; return NULL;}
    if ((id=shmget(clave, tam, flags))==-1)
        return (NULL);
    if ((p=shmat(id,NULL,0))==(void*) -1){
        aux=errno;
        if (tam)
             shmctl(id,IPC_RMID,NULL);
        errno=aux;
        return (NULL);
    }
    shmctl (id,IPC_STAT,&s);
    InsertElementM(LM, p, tam, "shared", clave, " ", 0); // InsertarNodoShared (&L, p, s.shm_segsz, clave); 
    return (p);
}


void do_AllocateCreateshared (char *tr[], tListM * LM)
{
   key_t cl;
   size_t tam;
   void *p;

   if (tr[0]==NULL || tr[1]==NULL) {
		return;
   }
  
   cl=(key_t)  strtoul(tr[0],NULL,10);
   tam=(size_t) strtoul(tr[1],NULL,10);
   if (tam==0) {
	printf ("No se asignan bloques de 0 bytes\n");
	return;
   }
   if ((p=ObtenerMemoriaShmget(cl,tam, LM))!=NULL)
		printf ("Asignados %lu bytes en %p\n",(unsigned long) tam, p);
   else
		printf ("Imposible asignar memoria compartida clave %lu:%s\n",(unsigned long) cl,strerror(errno));
}

void do_Shared(char *tr[], tListM *LM){
    key_t key = (key_t) strtoul(tr[0],NULL,10);
    int shmid = shmget(key, 0, 0);
    void *p;
    size_t size;

    if(shmid == -1){
        printf("Imposible to asing shared memory to the key %d", key);
        perror("");
        printf("\n");
        return;
    }

    if((p = shmat(shmid, NULL, 0)) == (void *) -1){
        printf("Imposible to asing shared memory to the key %d", key);
        perror("");
        printf("\n");
        return;
    }

    printf("Shared memory of key %d at %p\n", key, p);
    size = size_of_shared(*LM, key);
    InsertElementM(LM, p, size, "shared", key, " ", 0);

}

void * MapearFichero (char * fichero, int protection, tListM *LM)
{
    int df, map=MAP_PRIVATE,modo=O_RDONLY;
    struct stat s;
    void *p;

    if (protection&PROT_WRITE)
          modo=O_RDWR;
    if (stat(fichero,&s)==-1 || (df=open(fichero, modo))==-1)
          return NULL;
    if ((p=mmap (NULL,s.st_size, protection,map,df,0))==MAP_FAILED)
           return NULL;
/*InsertarNodoMmap (&L,p, s.st_size,df,fichero); */
    InsertElementM(LM, p, s.st_size, "mmap", 0, fichero, df);
    return p;
}

void do_AllocateMmap(char *arg[], tListM *LM)
{ 
     char *perm;
     void *p;
     int protection=0;
     
     if (arg[0]==NULL)
            {printListM(*LM, 'P'); return;}
     if ((perm=arg[1])!=NULL && strlen(perm)<4) {
            if (strchr(perm,'r')!=NULL) protection|=PROT_READ;
            if (strchr(perm,'w')!=NULL) protection|=PROT_WRITE;
            if (strchr(perm,'x')!=NULL) protection|=PROT_EXEC;
     }
     if ((p=MapearFichero(arg[0],protection, LM))==NULL)
             perror ("Imposible mapear fichero");
     else
             printf ("Fichero %s mapeado en %p\n", arg[0], p);
}


void LlenarMemoria (void *p, size_t cont, unsigned char byte)
{
  unsigned char *arr=(unsigned char *) p;
  size_t i;

  for (i=0; i<cont;i++)
		arr[i]=byte;
}

void dumpMemory (void *p, size_t cont){
    unsigned char *arr=(unsigned char *) p;
    size_t i;

    for (i=0; i<cont;i++){
        if(arr[i] != '0')
            printf("%c ", arr[i]);
        else
            printf("  ");

    }

    printf("\n");

    for (i=0; i<cont;i++){
        printf("%x ", arr[i]);
    }

    printf("\n");
}


void Do_pmap (void) /*sin argumentos*/
 { pid_t pid;       /*hace el pmap (o equivalente) del proceso actual*/
   char elpid[32];
   char *argv[4]={"pmap",elpid,NULL};
   
   sprintf (elpid,"%d", (int) getpid());
   if ((pid=fork())==-1){
      perror ("Imposible crear proceso");
      return;
      }
   if (pid==0){
      if (execvp(argv[0],argv)==-1)
         perror("cannot execute pmap (linux, solaris)");
         
      argv[0]="procstat"; argv[1]="vm"; argv[2]=elpid; argv[3]=NULL;   
      if (execvp(argv[0],argv)==-1)/*No hay pmap, probamos procstat FreeBSD */
         perror("cannot execute procstat (FreeBSD)");
         
      argv[0]="procmap",argv[1]=elpid;argv[2]=NULL;    
            if (execvp(argv[0],argv)==-1)  /*probamos procmap OpenBSD*/
         perror("cannot execute procmap (OpenBSD)");
         
      argv[0]="vmmap"; argv[1]="-interleave"; argv[2]=elpid;argv[3]=NULL;
      if (execvp(argv[0],argv)==-1) /*probamos vmmap Mac-OS*/
         perror("cannot execute vmmap (Mac-OS)");      
      exit(1);
  }
  waitpid (pid,NULL,0);
}


void Recursiva (int n)
{
  char automatico[TAMANO];
  static char estatico[TAMANO];

  printf ("parametro:%3d(%p) array %p, arr estatico %p\n",n,&n,automatico, estatico);

  if (n>0)
    Recursiva(n-1);
}
