/** Soubor: wordcount.c
* Reseni IJC-DU2, priklad 2
* Datum: 24. 4. 2015
* Autor: Nikola Valesova, xvales02, FIT
* Prelozeno: gcc 4.8
* Program na vypis slov v souboru a jejich cetnost vyskytu
* Pozn. Zadne funkce se nevyplatilo implementovat jako inline,
* byly stejne rychle nebo dokonce mirne pomalejsi
**/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "htable.h"
#include "io.h"


#define MAX_CHARS 127
#define SIZE 5003
/** Pro kvalitnejsi rozlozeni je lepsi pouzit nejake prvocislo **/
/** Snazime se najit kompromis mezi velikosti pameti zabrane tabulkou **/
/** a rychlosti prochazeni (cim vic zaznamu v seznamu, tim je program pomalejsi) **/
/** Zvolila jsem takove cislo, ktere nebude zabirat prilis mnoho pameti pro male soubory **/
/** a zaroven bude obsahovat dostatecny pocet seznamu pro soubory vetsi **/


/* funkce na vypis poctu vyskytu a dane slovo */
void my_printf(const char *key, unsigned int value)
{
    printf("%s\t%u\n", key, value);
}

/* hlavni funkce na vypis cetnosti slov */
int main()
{
    htab_t *table = htab_init(SIZE);    /* vytvoreni a inicializace tabulky */

    if (table == NULL)  /* zkontroluje platnost parametru */
    {
        fprintf(stderr, "CHYBA: Neuspesne vytvoreni a inicializace tabulky!\n");
        return 1;
    }

    char str[MAX_CHARS];    /* pole charu na ulozeni nacteneho slova */
    struct htab_listitem *tmp;  /* pomocna struktura na pruchod seznamem */

    while (fgetw(str, MAX_CHARS, stdin) != EOF) /* dokud nenarazi na konec souboru */
    {
        tmp = htab_lookup(table,str);   /* vyhledani zaznamu s danym slovem */
        if (tmp == NULL)    /* overeni uspesnosti pripadne alokace pameti */
        {
            htab_free(table);
            return 2;
        }
        tmp->data += 1; /* inkrementuje pocet vyskytu slova */
    }

    htab_foreach(table,my_printf);  /* vypis obsahu pro kazdy zaznam */

    htab_free(table);   /* uvolni pametovy prostor tabulky */

    return 0;
}
