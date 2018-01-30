/** Soubor: io.c
* Reseni IJC-DU2, priklad 2
* Datum: 24. 4. 2015
* Autor: Nikola Valesova, xvales02, FIT
* Prelozeno: gcc 4.8
* Modul s funkci na nacteni jednoho slova ze souboru
**/


#include "io.h"

/* funkce, ktera nacte jedno slovo ze souboru */
/* vraci pocet nactenych znaku, nebo EOF */
int fgetw(char *s, int max, FILE *f)
{
    static bool printed = false;    /* pomocna promenna na vypis chyboveho hlaseni */

    if (s == NULL || f == NULL || max < 0)  /* overeni platnosti parametru */
    {
        fprintf(stderr, "CHYBA: Neplatne parametry funkce fgetw!\n");
        return EOF;
    }

    int c;
    int counter = 0;

    while (((c = fgetc(f)) != EOF) && (isspace(c)))     /* preskoci vsechny bile znaky */
        ;

    if (c == EOF)   /* jestlize narazil na konec souboru */
        return EOF;

    while ((!isspace(c)) && (counter < max))    /* dokud neni bily znak, nebo prekrocena delka pole */
    {
        if (c == EOF)   /* jestlize je na konci souboru */
        {
            s[counter] = '\0';  /* zapise koncovou nulu */
            return EOF;
        }

        s[counter++] = (char) c;    /* do pole zapise znak, inkrementuje pocet zapsanych znaku */
        c = fgetc(f);   /* nacte dalsi znak */
    }

    if (counter == max) /* jestlize bylo nacteno maximum znaku */
    {
        if (!printed)   /* jestlize chybove hlaseni jeste nebylo vypsano */
        {
            printed = true;
            fprintf(stderr, "Prekrocena maximalni delka radku, dojde k oriznuti!\n");
        }

        while (!isspace(c = fgetc(f)))  /* preskoceni nebilych znaku - oriznuti slova */
            ;

        counter--;  /* dekrementace citace */
    }

    s[counter] = '\0';  /* zapsani koncove nuly */

    return counter;
}

