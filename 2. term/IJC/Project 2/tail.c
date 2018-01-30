/** Soubor: tail.c
* Reseni IJC-DU2, priklad 1 a)
* Datum: 24. 4. 2015
* Autor: Nikola Valesova, xvales02, FIT
* Prelozeno: gcc 4.8
* Implementace POSIX prikazu tail
**/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_LENGTH 510	//maximalni delka radku vcetne nuloveho znaku
#define LINES_COUNT 10	//implicitni pocet tisknutych radku


//funkce vypise chybove hlaseni a program ukonci
void print_error(char *hlaseni)
{
    fprintf(stderr, "CHYBA: %s\n", hlaseni);
    exit (1);
}


//funkce vypise chybove hlaseni
void warning(char *hlaseni)
{
    fprintf(stderr, "%s\n", hlaseni);
}


//funkce na vyber funkce dle zadanych argumentu
int set_params(int argc, char *argv[])
{
    if (argc > 4)		//prilis mnoho parametru
	return 0;

    if (argc == 1)		//spusteni: tail
	return 2;

    if (argc == 2)		//spusteni: tail soubor
	return 1;

    if ((strcmp("-n", argv[1])) == 0)
    {
	if (argv[2][0] == '+')
	{
	    if (argc == 3)
	        return 3;	//spusteni: tail -n +x
	    if (argc == 4)
		return 5;	//spusteni: tail -n +x soubor
	}
	else
	{
	    if (argc == 3)
	        return 4;	//spusteni: tail -n x
	    if (argc == 4)
		return 6;	//spusteni: tail -n x soubor
	}

    }

    return 0;
}


//funkce na vypis nekolika poslednich radku
void print_lines(FILE *fr, long int count)
{
    char buffer[count][MAX_LENGTH];
    int c;			//pro nacitani po znacich
    long int pos = 0;		//ktery prvek bufferu bude adresovan
    bool printed = false;	//zda byla vypsana chyba pri preteceni radku

    if (count < 0)		//kontrola volby poctu radku
    {
	fclose(fr);
	print_error("Zaporny argument poctu radku!");
    }

    if (count == 0)		//nula radku = nic nevypisujeme
	return;

    while (fgets(buffer[pos%count], MAX_LENGTH, fr) != NULL)   	//dosazen konec suboru, ukonci nacitani
    {
	if (strchr(buffer[pos%count], '\n') == NULL)		//radek se nevlezl do bufferu
	{
	    while ((c = fgetc(fr)) != '\n' && c != EOF)		//preskoceni pretecenych znaku
		;
	}

	pos++;
    }

    if (pos < count)		//bylo nacteno mene radku nez chceme vypsat
	count = pos;

    for (long int i = 0; i < count; i++)
    {
	if (strchr(buffer[pos%count], '\n') == NULL)		//radek se nevlezl do bufferu
	{
	    if (!printed)	//pokud jeste nebylo vypsano chybove hlaseni
	    {
		warning("Prekrocena maximalni delka radku, dojde k oriznuti!");
		printed = true;
	    }

            buffer[pos%count][MAX_LENGTH - 2] = '\n';		//pred koncovou nulu vloz znak konce radku
	}

	printf("%s", buffer[pos%count]);
	pos++;
    }
    return;
}


//funkce na vypis od daneho radku
void print_from_line(FILE *fr, long int count)
{
    char buffer[MAX_LENGTH];
    int c;			//pro nacitani po znacich
    bool printed = false;	//zda byla vypsana chyba pri preteceni radku
    long int i;			//pocitadlo cyklu for

    if (count < 0)		//kontrola volby poctu radku
    {
	fclose(fr);
	print_error("Zaporny argument poctu radku!");
    }

    for (i = 0; i < (count - 1); )	//pro kazdy radek, ktery chceme preskocit
    {
	if ((c = fgetc(fr)) != '\n')	//jestlize nenarazi na znak konce radku
	{
	    if (c == EOF)
	    {
		count = i;
	    }
	}
	else				//jinak inkrementuj pocet nactenych radku
	    i++;
    }

    while (fgets(buffer, MAX_LENGTH, fr) != NULL)	//nacti platny radek
    {
	if (strchr(buffer, '\n') == NULL)		//radek se nevlezl do bufferu
	{
	    while ((c = fgetc(fr)) != '\n' && c != EOF)	//preskoceni pretecenych znaku
		;

	    if (!printed)	//pokud jeste nebylo vypsano chybove hlaseni
	    {
		warning("Prekrocena maximalni delka radku, dojde k oriznuti!");
		printed = true;
	    }

            buffer[MAX_LENGTH - 2] = '\n'; //pred koncovou nulu zapis znak konce radku
	}

	printf("%s", buffer);	//vypis radek
    }
    return;
}


//volani funkci na zpracovani argumentu a vypis radku
int main(int argc, char *argv[])
{
    FILE *fr;
    char *endptr;
    endptr = NULL;

    fr = stdin;
    long int count = LINES_COUNT;			//pozadovany pocet radku, implicitne 10

    int sort = set_params(argc, argv);			//zpracuje zadane argumenty

    switch (sort) {	//vybere pozadovany typ funkce
	case 1:						//tail soubor
	    if ((fr = fopen(argv[1], "r")) == NULL)
                print_error("Soubor se nepodarilo otevrit!");
	case 2:						//tail
	    print_lines(fr, count);
	    break;
	case 5:						//tail -n +x soubor
	    if ((fr = fopen(argv[3], "r")) == NULL)
                print_error("Soubor se nepodarilo otevrit!");
	case 3:						//tail -n +x
	    count = strtol(argv[2], &endptr, 10);
            if (*endptr != '\0')
            {
                warning("Spatne zadany ciselny argument!");
            	break;
            }
	    print_from_line(fr, count);
	    break;
	case 6:						//tail -n x soubor
	    if ((fr = fopen(argv[3], "r")) == NULL)
                print_error("Soubor se nepodarilo otevrit!");
	case 4:						//tail -n x
	    count = strtol(argv[2], &endptr, 10);
            if (*endptr != '\0')
            {
            	warning("Spatne zadany ciselny argument!");
            	break;
            }
	    print_lines(fr, count);
	    break;
	default:					//chybny stav
	    print_error("Spatne zadane argumenty!");
	    break;
    }

    if (fclose(fr) == EOF)				//uzavreni souboru
        print_error("Soubor se nepodarilo uzavrit!");

    return 0;
}
