/** Soubor: htab_foreach.c
* Reseni IJC-DU2, priklad 2
* Datum: 24. 4. 2015
* Autor: Nikola Valesova, xvales02, FIT
* Prelozeno: gcc 4.8
* Modul s funkci na volani funkce pro vsechny prvky
**/


#include "htable.h"

/* funkce na volani funkce zadane parametrem pro vsechny prvky tabulky */
void htab_foreach(htab_t *t, void (*function)(const char *, unsigned int))
{
    if (t == NULL)	/* zkontroluje platnost parametru */
    {
        fprintf(stderr, "CHYBA: Neplatna tabulka!\n");
        return;
    }

    struct htab_listitem *tmp = NULL;	/* pomocna struktura na ulozeni adresy nasledujiciho prvku */

    for (unsigned int i = 0; i < t->htab_size; i++)	/* pro kazdy seznam v tabulce */
    {
        tmp = t->ptr[i];	/* nacti adresu aktualniho seznamu */
        while (tmp != NULL)		/* dokud neni konec seznamu */
        {
            function(tmp->key, tmp->data);	/* zavolej funkci pro aktualni prvek */
            tmp = tmp->next;	/* presun se na dalsi */
        }
    }

    return;
}
