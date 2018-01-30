/** Soubor: htab_free.c
* Reseni IJC-DU2, priklad 2
* Datum: 24. 4. 2015
* Autor: Nikola Valesova, xvales02, FIT
* Prelozeno: gcc 4.8
* Modul s funkci na zruseni cele tabulky
**/


#include "htable.h"

/* funkce na uvolneni pameti cele tabulky */
void htab_free(htab_t *t)
{
    if (t != NULL)	/* overi platnost vstupniho parametru */
    {
        htab_clear(t);	/* zavola funkci odstraneni obsahu cele tabulky */
        free(t);		/* uvolni ukazatel na tabulku */
        t = NULL;	/* zahodi ukazatel, aby se s nim dale nedalo pracovat */
    }

    return;
}



