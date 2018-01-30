/** Soubor: ppm.h
* Reseni IJC-DU1, priklad b)
* Datum: 26. 3. 2015
* Autor: Nikola Valesova, xvales02, FIT
* Prelozeno: gcc 4.8
* Deklarace funkci pro nacteni a zapis ppm souboru,
**/


#ifndef __PPMH__
    #define __PPMH__

#include <stdio.h>
#include <stdlib.h>

#include "bit-array.h"
#include "eratosthenes.h"
#include "error.h"

#define RGB 255

/* struktura k nacteni obsahu ppm souboru */
struct ppm {
    unsigned long xsize;
    unsigned long ysize;
    char data[];
};

struct ppm * ppm_read(const char * filename);
int ppm_write(struct ppm *p, const char * filename);

#endif
