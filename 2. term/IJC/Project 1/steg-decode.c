/** Soubor: steg-decode.c
* Reseni IJC-DU1, priklad b)
* Datum: 26. 3. 2015
* Autor: Nikola Valesova, xvales02, FIT
* Prelozeno: gcc 4.8
* Definice funkce, ktera rozlusti tajnou zpravu
**/


#include "bit-array.h"
#include "eratosthenes.h"
#include "error.h"
#include "ppm.h"


static void steg_decode(const char * filename);


/* funkce zkontroluje pocet argumentu a zavola steg_decode */
int main(int argc, char *argv[])
{
    if (argc != 2)
        FatalError("Spatny pocet argumentu.");

    steg_decode(argv[1]);

    return 0;
}


/* funkce, ktera rozlusti a vypise tajnou zpravu */
static void steg_decode(const char * filename)
{
    struct ppm * p_ppm;

    /* nacteni obsahu souboru do struktury a kontrola */
    if ((p_ppm = ppm_read(filename)) == NULL)
        exit (1);

    T max_count = 3 * p_ppm->xsize * p_ppm->ysize;
    T counter = 0;

    /* vytvoreni pole o pozadovane velikosti */
    BA_create(pole, max_count);
    /* nastaveni neprvociselnych indexu na 1 */
	Eratosthenes(pole);

    /* prvni prvek obsahuje velikost pole, druhy 0 */
    unsigned char buffer[2] = {CHAR_BIT};

    T i;
    for (i = 2; i < max_count; i++)
    {
        /* jestlize je index prvocislem */
        if (!(BA_get_bit(pole, i)))
        {
            /* predani LSb do bufferu */
            BA_set_bit(buffer,counter,(p_ppm->data[i] & 1));

            /* jestlize jeste nebyl nacten jeden znak */
            if (counter < (T) (buffer[0] - 1))
                counter++;

            /* pokud byl nacten koncovy znak retezce */
            else if (buffer[1] == '\0')
            {
                printf("\n");
                break;
            }


            /* pokud byl nacten jeden cely znak */
            else if (counter == (T) (buffer[0] - 1))
            {
                /* jestlize se jedna o netisknutelny znak */
                if (!isprint(buffer[1]))
                {
                    free(p_ppm);
                    FatalError("Nacten netisknutelny znak.");
                }
                printf("%c",buffer[1]);
                counter = 0;
            }
        }
    }

    /* kontrola, zda byla zprava ukoncena nulovym znakem */
    if (i == max_count)
    {
        free(p_ppm);
        FatalError("Zprava nebyla ukoncena nulovym znakem.");
    }

    free(p_ppm);
    return;
}
