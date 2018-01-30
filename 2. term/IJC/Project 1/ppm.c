/** Soubor: ppm.c
* Reseni IJC-DU1, priklad b)
* Datum: 26. 3. 2015
* Autor: Nikola Valesova, xvales02, FIT
* Prelozeno: gcc 4.8
* Definice funkci pro nacteni a zapis ppm souboru
**/


#include "ppm.h"


/* funkce, ktera nacte obsah ppm souboru do dane struktury */
struct ppm * ppm_read(const char * filename)
{
     FILE *fr;

     /* otevreni souboru a jeho kontrola */
     if ((fr = fopen(filename, "rb")) == NULL)
          FatalError("Soubor se nepodarilo otevrit.");

     T x, y, rgb;

     /* nacteni hlavicky souboru a kontrola */
     if ((fscanf(fr, "P6 %lu %lu %lu", &x, &y, &rgb) != 3) || rgb != RGB)
     {
          Warning("Spatny format souboru.");
          fclose(fr);
          return NULL;
     }

    /* "preskoceni" pripadnych prazdnych radku */
    while (fgetc(fr) != '\n')
        ;

    struct ppm * p_ppm;

    /* alokace pameti pro strukturu a kotrola */
    if ((p_ppm = malloc(sizeof(struct ppm) + 3 * x * y)) == NULL)
    {
          Warning("Neuspesna alokace pametoveho prostoru.");
          fclose(fr);
          return NULL;
    }

    /* nacteni hlavniho obsahu souboru a kotrola */
    if (fread(p_ppm->data,1, 3 * x * y, fr) != 3 * x * y)
    {
          Warning("Neuspesne nacitani binarnich dat.");
          free(p_ppm);
          fclose(fr);
          return NULL;
    }

    /* kontrola, zda soubor neobsahuje vice dat */
    int c;
    if ((c = fgetc(fr)) != EOF)
    {
          Warning("Nadbytecna binarni data.");
          free(p_ppm);
          fclose(fr);
          return NULL;
    }

    p_ppm->xsize = x;
    p_ppm->ysize = y;

    /* zavreni souboru a kontrola */
    if (fclose(fr) == EOF)
    {
         Warning("Soubor se nepodarilo zavrit.");
         free(p_ppm);
         return NULL;
    }

    return p_ppm;
}


/* funkce, ktera zapise obsah struktury do ppm souboru */
int ppm_write(struct ppm *p, const char * filename)
{
    FILE *fw;

    /* otevreni souboru a kontrola */
    if ((fw = fopen(filename, "wb")) == NULL)
    {
         Warning("Soubor se nepodarilo otevrit.");
         return -1;
    }

    /* zapis hlavicky a kontrola */
    if (fprintf(fw, "P6\n%lu %lu\n255\n", p->xsize, p->ysize) < 0)
    {
        Warning("Neuspesny zapis do souboru.");
        fclose(fw);
        return -1;
    }


    /* zapis obsahu do souboru a kontrola */
    if (fwrite(p->data, 1, 3 * p->xsize * p->ysize, fw) != 3*p->xsize*p->ysize)
    {
        Warning("Neuspesny zapis do souboru.");
        fclose(fw);
        return -1;
    }

    /* zavreni souboru a kontrola */
    if (fclose(fw) == EOF)
    {
         Warning("Soubor se nepodarilo zavrit.");
         return -1;
    }

    return 0;
}
