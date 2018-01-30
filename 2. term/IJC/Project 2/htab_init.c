/** Soubor: htab_init.c
* Reseni IJC-DU2, priklad 2
* Datum: 24. 4. 2015
* Autor: Nikola Valesova, xvales02, FIT
* Prelozeno: gcc 4.8
* Modul s funkci navytvoreni a inicializaci tabulky
**/


#include "htable.h"

/* funkce, ktera vytvori a inicializuje celou tabulku */
htab_t *htab_init(unsigned int size)
{
    htab_t *new_tab = malloc(sizeof(htab_t) + (size * sizeof(struct htab_listitem *)));	/* alokuje pamet pro celou tabulku */

    if (new_tab == NULL)	/* overeni uspesnosti alokace */
        return NULL;

    for (unsigned int i = 0; i < size; i++)		/* kazdemu seznamu v tabulce */
        new_tab->ptr[i] = NULL;	/* prirad prazdny ukazatel */

    new_tab->htab_size = size;	/* do prvniho prvku tabulky uloz jeji velikost (pocet seznamu) */

    return new_tab;
}
