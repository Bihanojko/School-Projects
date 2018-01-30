/** Soubor: eratosthenes.c
* Reseni IJC-DU1, priklad a), b)
* Datum: 26. 3. 2015
* Autor: Nikola Valesova, xvales02, FIT
* Prelozeno: gcc 4.8
* Definice funkce implementující Eratosthenovo síto
**/


#include "eratosthenes.h"
#include "bit-array.h"


/* funkce, ktera nastavi bity na neprvociselnych indexech na jednicku */
void Eratosthenes(BitArray_t pole)
{
    /* cyklus staci opakovat do odmocniny z daneho cisla */
    T horni_mez = sqrt(pole[0]);

    for (T i = 2; i <= horni_mez; i++)
    {
        /* jesltize bylo nalezeno prvocislo */
        if (!(BA_get_bit(pole,i)))
        {
            /* vsechny jeho nasobky nastav na jednicku */
            for (T j = 2; i*j <= pole[0]; j++)
                BA_set_bit(pole,i*j,1);
        }
    }
}



