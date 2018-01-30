/** Soubor: hash_function.c
* Reseni IJC-DU2, priklad 2
* Datum: 24. 4. 2015
* Autor: Nikola Valesova, xvales02, FIT
* Prelozeno: gcc 4.8
* Definice vhodne rozptylovaci funkce
**/


#include "hash_function.h"


/* rozptylovaci funkce pro hash tabulku */
unsigned int hash_function(const char *str, unsigned htab_size)
{
    unsigned int h = 0;
    const unsigned char *p;
    for (p = (const unsigned char *) str; *p != '\0'; p++)
        h = 65599 * h + *p;
    return h % htab_size;
}
