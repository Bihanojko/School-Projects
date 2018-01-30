/** Soubor: htab_remove.c
* Reseni IJC-DU2, priklad 2
* Datum: 24. 4. 2015
* Autor: Nikola Valesova, xvales02, FIT
* Prelozeno: gcc 4.8
* Modul s funkci na odstraneni dane polozky z tabulky
**/


#include "htable.h"

/* funkce, ktera odstrani danou polozku z tabulky */
void htab_remove(htab_t *t, const char *key)
{
    if (t == NULL || key == NULL)	/* kontrola platnosti parametru */
    {
        fprintf(stderr, "CHYBA: Neplatne parametry funkce htab_remove!\n");
        return;
    }

    unsigned int index = hash_function(key, t->htab_size);	/* ziskani indexu do hash tabulky */

    struct htab_listitem *tmp = t->ptr[index];	/* pomocna struktura k prochazeni seznamem zaznamu */
    struct htab_listitem *prev = NULL;		/* ulozeni predchoziho zaznamu */

    while (tmp != NULL)	/* dokud nedojde na konec seznamu */
    {
        if (strcmp(tmp->key, key) == 0)	/* jestlize jsi nasel zaznam */
        {
            if (prev != NULL)	/* jestlize zaznam neni prvni v seznamu */
                prev->next = tmp->next;	/* do predchoziho uloz adresu nasledujiciho zaznamu, uvolneni aktualniho */

            else		/* zaznam je prvni v seznamu */
                t->ptr[index] = tmp->next;	/* do tabulky uloz adresu nasledujiciho zaznamu, uvolneni aktualniho */

            if (tmp->key != NULL)	/* jestlize je v mazanem zaznamu nejaky klic */
                free(tmp->key);	/* uvolni pamet */
            free(tmp);	/* uvolni pamet celeho zaznamu */
            return;
        }
        else	/* shoda se zaznamem neprobehla */
            prev = tmp;	/* prejdi na dalsi zaznam */
            tmp = tmp->next;
    }

    if (tmp == NULL)	/* nalezen konec seznamu */
        fprintf(stderr, "CHYBA: Polozka k odstraneni nenalezena!\n");
    return;
}
