/** Soubor: prvocisla.c
* Reseni IJC-DU1, priklad a)
* Datum: 26. 3. 2015
* Autor: Nikola Valesova, xvales02, FIT
* Prelozeno: gcc 4.8
* Vypis poslednich deseti prvocisel
**/


#include "bit-array.h"
#include "error.h"
#include "eratosthenes.h"

#define N 201000000
#define COUNT 10


/* funkce na vypis poslednich deseti prvocisel */
int main(void)
{
    /* vytvoreni pole o pozadovane velikosti */
    BA_create(pole, (N+1));
    /* nastaveni neprvociselnych indexu na 1 */
    Eratosthenes(pole);

    T j = pole[0];

    /* nalezeni indexu 10. prvocisla od konce */
    for (T i = 0; i < COUNT; i++, j--)
    {
        for ( ; BA_get_bit(pole,j); j--)
            ;
    }

    j++;

    /* vypis poslednich deseti prvocisel */
    for (T i = 0; i < COUNT; j++)
    {
        if (!(BA_get_bit(pole,j)))
        {
            printf("%lu\n", j);
            i++;
        }
    }

    return 0;
}

