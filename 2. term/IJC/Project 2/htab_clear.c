/** Soubor: htab_clear.c
* Reseni IJC-DU2, priklad 2
* Datum: 24. 4. 2015
* Autor: Nikola Valesova, xvales02, FIT
* Prelozeno: gcc 4.8
* Modul s funkci na zruseni vsech polozek v tabulce
**/


#include "htable.h"

/* funkce, ktera zrusi vsechny polozky hash tabulky */
void htab_clear(htab_t *t)
{
    if (t == NULL || t->ptr == NULL)	/* kontrola platnosti parametru */
        return;

    for (unsigned int i = 0; i < t->htab_size; i++)	/* pro kazdy seznam v tabulce */
    {
        if (t->ptr[i] == NULL)	/* jestlize je dany seznam prazdny */
            continue;		/* pokracuje na dalsi */

        struct htab_listitem *data = t->ptr[i];	/* pomocna iteracni struktura */

        while (data != NULL)	/* dokud nenarazi na konec seznamu */
        {
            if (data->key != NULL)	/* jestlize je v zaznamu klic */
                free(data->key);		/* uvolni pamet */

            struct htab_listitem *tmp = data->next;	/* ulozi dalsi prvek seznamu */

            free(data);	/* uvolni pamet celeho zaznamu */
            data = tmp;	/* posun na dalsi prvek */
        }

        t->ptr[i] = NULL;	/* smaze adresu seznamu z tabulky */
    }
    return;
}
