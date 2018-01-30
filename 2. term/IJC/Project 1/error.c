/** Soubor: error.c
* Reseni IJC-DU1, priklad a), b)
* Datum: 26. 3. 2015
* Autor: Nikola Valesova, xvales02, FIT
* Prelozeno: gcc 4.8
* Definice funkci pro vypis chybovych hlaseni
**/


#include "error.h"


/* funkce, ktera vypise chybove hlaseni na stderr */
void Warning(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);

    fprintf(stderr, "CHYBA: " );
    vfprintf(stderr, fmt, args);  
    fprintf(stderr, "\n" );

    va_end(args);

}

/* funkce, ktera vypise chybove hlaseni na stderr */
/* a program ukonci */
void FatalError(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);

    fprintf(stderr, "CHYBA: " );
    vfprintf(stderr, fmt, args);
    fprintf(stderr, "\n" );

    va_end(args);
    exit(1);
}


