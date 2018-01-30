/** Soubor: htable.h
* Reseni IJC-DU2, priklad 2
* Datum: 24. 4. 2015
* Autor: Nikola Valesova, xvales02, FIT
* Prelozeno: gcc 4.8
* Rozhrani s operacemi s hash tabulkou
**/


#ifndef HTAB_H_INCLUDED
#define HTAB_H_INCLUDED


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "hash_function.h"


/* struktura popisujici jeden zaznam */
struct htab_listitem {
    char *key;
    unsigned int data;
    struct htab_listitem *next;
};

/* struktura popisujici celou hash tabulku */
typedef struct htable {
    unsigned int htab_size;
    struct htab_listitem *ptr[];
} htab_t;


/* prototypy funkci */
htab_t *htab_init(unsigned int size);
struct htab_listitem *htab_lookup(htab_t *t, const char *key);
void htab_foreach(htab_t *t, void (*function)(const char *, unsigned int));
void htab_remove(htab_t *t, const char *key);
void htab_clear(htab_t *t);
void htab_free(htab_t *t);
void htab_statistics(htab_t *t);


#endif // HTAB_H_INCLUDED
