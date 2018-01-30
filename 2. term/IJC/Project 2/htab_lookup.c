/** Soubor: htab_lookup.c
* Reseni IJC-DU2, priklad 2
* Datum: 24. 4. 2015
* Autor: Nikola Valesova, xvales02, FIT
* Prelozeno: gcc 4.8
* Modul s funkci na vyhledani (prip. pridani) polozky
**/


#include "htable.h"

/* funkce, ktera vyhleda zadanou polozku, a pokud ji nenajde, prida ji na konec urceneho seznamu */
struct htab_listitem *htab_lookup(htab_t *t, const char *key)
{
    if (t == NULL || key == NULL)	/* kontrola platnosti parametru */
    {
        fprintf(stderr, "CHYBA: Neplatne parametry funkce htab_lookup!\n");
        return NULL;
    }

    unsigned int index = hash_function(key, t->htab_size);	/* ziskani indexu do hash tabulky */

    struct htab_listitem *tmp = t->ptr[index];	/* nacteni indexu seznamu z hash tabulky */

    while (tmp != NULL)	/* dokud nedojde na konec seznamu */
    {
        if (strcmp(tmp->key, key) == 0)	/* jestlize najde dany klic */
            return tmp;	/* vrati odkaz na aktualni zaznam */
        else
            if (tmp->next == NULL)	/* pokud narazil na posledni zaznam seznamu */
                break;
            tmp = tmp->next;	/* prejde na dalsi prvek */
    }

    struct htab_listitem *new = malloc(sizeof(*new));	/* alokuje pamet pro novy zaznam */

    if (new == NULL)	/* overi uspesnost */
    {
        fprintf(stderr, "CHYBA: Nezdarena alokace pameti pro dalsi prvek!\n");
        return NULL;
    }

    new->key = malloc(sizeof(char) * (strlen(key) + 1));	/* alokace pameti pro key - prvek tabulky */

    if (new->key == NULL)	/* overi uspesnost */
    {
        free(new);
        fprintf(stderr, "CHYBA: Nezdarena alokace pameti pro dalsi prvek!\n");
        return NULL;
    }

    new->data = 0;	/* pocet vyskytu nastavi na nulu */
    new->next = NULL;	/* dalsi zaznam neni zadny, pridavame na konec */
    (void) (strcpy(new->key, key));	/* zapiseme hledane slovo do nove vytvorene struktury */

    if (tmp == NULL)	/* jestlize se jedna o prvni zaznam v aktualnim seznamu */
        t->ptr[index] = new;	/* ulozi do tabulky adresu noveho zaznamu */
    else	/* jeslize uz v seznamu nejake zaznamy jsou */
        tmp->next = new;	/* do predchoziho ulozi odkaz na novy zaznam */

    return new;
}
