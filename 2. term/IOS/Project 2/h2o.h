/** Soubor: h2o.h
* IOS, Projekt 2
* Datum: 3. 5. 2015
* Autor: Nikola Valesova, xvales02, FIT
* Prelozeno: gcc 4.8
* Rozhrani pro implementaci synchronizacniho problemu
* tvorby molekuly vody
**/


#ifndef __H2OH__INCLUDED
    #define __H2OH__INCLUDED


//	HLAVICKOVE SOUBORY
#include <fcntl.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/mman.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>


// nejdelsi mozna doba, po ktere je generovan novy proces
const long int max = 5001;


// struktura uchovavajici zadane parametry
typedef struct params
{
    long int o_count;		// v zadani: N
    long int h_max;		// v zadani: GH
    long int o_max;		// v zadani: GO
    long int bond_max;		// v zadani: B
    FILE *fw;			//ukazatel na soubor pro zapis
} Params;


// struktura obsahujici semafory
typedef struct synch_m
{
    sem_t *operation;   // pristup ke sdilene pameti + zajisteni atomicnosti
    sem_t *wrt;         // pristup k vystupnimu souboru a ke sdilene pameti 
    sem_t *oxygens;     // fronta kysliku
    sem_t *hydrogens;   // fronta vodiku
    sem_t *bond;	// cekani na navrat vsech tri atomu z funkce bond
    sem_t *bonding;	// zajistuje atomicnosti procesu spojeni atomu ve funkci bond
    sem_t *done;	// pockani na ostatni atomy pred ukoncenim funkce bond
    sem_t *ending;	// cekani na vytvoreni a spojeni vsech atomu
    FILE *fw;    	// ukazatel na soubor pro zapis
} Synch_molecule;


// struktura sdilenych promennych
typedef struct shared_mem
{
    long int *print_counter;	// citac vypisu
    long int *o_counter;	// citac procesu kysliku
    long int *h_counter;	// citac procesu vodiku
    long int *to_bond;		// citac atomu pripravenych ke spojeni
    long int *h_ready;		// citac pripravenych atomu vodiku
    long int *finished;		// citac dokoncenych atomu
} Shared_Mem;


// 	MAKRO NA UVOLNENI ALOKOVANE PAMETI
#define DE_ALLOC do {if (munmap (synch.operation, 8 * sizeof (sem_t *)) == -1)\
			    fprintf(stderr, "Chyba pri uvolnovani pameti.\n");\
                        if (munmap(shared_memory.print_counter, 6 * sizeof (long int)) == -1)\
			    fprintf(stderr, "Chyba pri uzavirani semaforu.\n");\
                        if (oxygen_pid != NULL)\
			    free(oxygen_pid);\
                        if (hydrogen_pid != NULL)\
			    free(hydrogen_pid);\
                        if (fclose(my_params.fw) == EOF)\
			    fprintf(stderr, "Chyba pri uzavirani souboru.\n"); } while (0)


//	MAKRO NA ZNICENI VSECH SEMAFORU
#define SEMDESTROY do { if (sem_destroy(synch.operation) == -1 ||\
			sem_destroy(synch.hydrogens) == -1 ||\
			sem_destroy(synch.oxygens) == -1 ||\
			sem_destroy(synch.wrt) == -1 ||\
			sem_destroy(synch.ending) == -1 ||\
			sem_destroy(synch.bond) == -1 ||\
			sem_destroy(synch.done) == -1 ||\
			sem_destroy(synch.bonding) == -1)\
				fprintf(stderr, "Chyba pri niceni semaforu.\n"); } while(0)

// 	PROTOTYPY FUNKCI
int checkparams(int argc, char *argv[], Params *my_params);
void hydrogen(Synch_molecule synch, Shared_Mem *shared_memory, long int max_sleep, int hydrogen_id);
void oxygen(Synch_molecule synch, Shared_Mem *shared_memory, long int max_sleep, int oxygen_id);
void bond(Synch_molecule synch, Shared_Mem *shared_memory, long int max_sleep, int my_id, char *me);


#endif //__H2OH__INCLUDED
