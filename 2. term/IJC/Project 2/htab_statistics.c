/** Soubor: htab_statistics.c
* Reseni IJC-DU2, priklad 2
* Datum: 24. 4. 2015
* Autor: Nikola Valesova, xvales02, FIT
* Prelozeno: gcc 4.8
* Modul s funkci na vypis prumerne, minimalni
* a maximalni delky seznamu v tabulce
**/


#include "htable.h"

/* funkce na serazeni dvou parametru */
void my_sort(unsigned int *a, unsigned int *b);

/* funkce, ktera vypise nejkratsi, nejdelsi a prumernou delku seznamu */
void htab_statistics(htab_t *t)
{
    if (t == NULL)  /* overeni platnosti parametru */
    {
        fprintf(stderr, "CHYBA: Neplatne parametry funkce htab_statistics!\n");
        return;
    }

    unsigned int lengths[t->htab_size]; /* pole integeru pro ulozeni delek seznamu */
    unsigned int count = 0; /* pocitadlo zaznamu v jednom seznamu */
    struct htab_listitem *tmp = NULL;   /* pomocna struktura na ulozeni aktualniho zaznamu */

    for (unsigned int i = 0; i < t->htab_size; i++) /* pro kazdy seznam v hash tabulce */
    {
        tmp = t->ptr[i];    /* prejde na dalsi zaznam */

        while (tmp != NULL) /* dokud neni na konci seznamu */
        {
            count++;    /* inkrementuje pocet zaznamu */
            tmp = tmp->next;    /* prejde na dalsi prvek */
        }

        lengths[i] = count; /* uloz delku i-teho seznamu */
        count = 0;
    }

    for (unsigned int i = 0; i < t->htab_size; i++) /* seradi pole delek seznamu */
    {
        for (unsigned int j = i + 1; j < t->htab_size; j++)
            my_sort(&lengths[i], &lengths[j]);
    }

    unsigned int sum = 0;   /* pomocna promenna na vypocet vsech zaznamu */

    for (unsigned int i = 0; i < t->htab_size; i++) /* secti vsechny delky */
        sum += lengths[i];



    printf("Min. delka seznamu:\t%u\n", lengths[0]);
    printf("Prumerna delka seznamu:\t%.3g\n", (double) sum/t->htab_size);
    printf("Max. delka seznamu:\t%u\n", lengths[t->htab_size - 1]);

    return;
}


void my_sort(unsigned int *a, unsigned int *b)
{
    if (*a > *b)
    {
        unsigned int tmp = *a;
        *a = *b;
        *b = tmp;
    }
}
