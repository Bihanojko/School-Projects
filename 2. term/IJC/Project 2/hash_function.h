/** Soubor: hash_function.h
* Reseni IJC-DU2, priklad 2
* Datum: 24. 4. 2015
* Autor: Nikola Valesova, xvales02, FIT
* Prelozeno: gcc 4.8
* Rozhrani s rozptylovaci funkci
**/


#ifndef HASH_FUNCTION_H_INCLUDED
#define HASH_FUNCTION_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>


/* deklarace vhodne rozptylovaci funkce */
unsigned int hash_function(const char *str, unsigned htab_size);

#endif // HASH_FUNCTION_H_INCLUDED
