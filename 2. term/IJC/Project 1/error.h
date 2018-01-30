/** Soubor: error.h
* Reseni IJC-DU1, priklad a), b)
* Datum: 26. 3. 2015
* Autor: Nikola Valesova, xvales02, FIT
* Prelozeno: gcc 4.8
* Deklarace funkci pro vypis chybovych hlaseni
**/


#ifndef __ERRORH__
    #define __ERRORH__

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>


void Warning(const char *fmt, ...);
void FatalError(const char *fmt, ...);

#endif

