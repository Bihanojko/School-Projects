/*
 * Soubor:  proj1.c
 * Datum:   2014/11/16
 * Autor:   Nikola Valesova, xvales02@stud.fit.vutbr.cz
 * Projekt: Vypocty v tabulce, projekt c. 1 pro predmet IZP
 * Popis:   Program provadi tisk, vypocet maxima, minima, souctu
 * a aritmetickeho prumeru vybranych bunek v tabulce
 */

const char *help =
"Program Vypocty v tabulce\n"
"Autor: Nikola Valesova (c) 2014\n"
"Program provadi tisk, vypocet maxima, minima, souctu a aritmetickeho "
"prumeru vybranych bunek v tabulce.\n"
"Pouziti: proj1 --help\n"
"         proj1 operace vyber_bunek\n\n"
"Popis parametru:\n"
"Program se spousti zadanim vyse uvedenych parametru. Parametr --help vytiskne "
"napovedu a program ukonci, parametr operace predstavuje jednu z nasledujicich moznosti:\n"
"select pro vypsani vybranych dat tabulky\n"
"min pro urceni nejmensi z vybranych hodnot\n"
"max pro urceni nejvetsi z vybranych hodnot\n"
"sum pro urceni souctu vybranych hodnot a\n"
"avg pro urceni aritmetickeho prumeru vybranych hodnot.\n\n"
"Vyber_bunek reprezentuje rozsah tabulky, ze ktereho chceme data ziskat. "
"Rozsah je definovan jednou z nasledujicich moznosti:\n"
"row X – vyber vsech bunek na radku X (X > 0)\n"
"col X – vyber vsech bunek ve sloupci X (X > 0)\n"
"rows X Y – vyber vsech bunek od radku X (vcetne) az po Y (vcetne), 0 < X <= Y\n"
"cols X Y – vyber vsech bunek od sloupce X (vcetne) az po Y (vcetne), 0 < X <= Y\n"
"range A B X Y – vyber bunek od radku A po radek B a od sloupce X po sloupec Y\n"
"(vcetne danych radku a sloupcu), 0 < A <= B, 0 < X <= Y.\n";


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define argc_help   2                                                       /* definice poctu argumentu pro dany zpusob vyberu bunek */
#define argc_row    4
#define argc_col    4
#define argc_rows   5
#define argc_cols   5
#define argc_range  7

#define ROW_LENGTH  1024                                                    /* definice maximalni delky jednoho radku */

                                                                            /* vycet chybovych hlaseni */
const char *err_dataformat = "Vybrana data neobsahuji pouze ciselne hodnoty.\n";
const char *err_argformat = "Nevhodny format argumentu vyberu bunek.\n";
const char *err_decarg = "Argument vyberu bunek nesmi byt desetinne cislo.\n";
const char *err_negarg = "Argument vyberu bunek musi byt vetsi nez nula.\n";
const char *err_argrange = "Argument vyberu bunek lezi mimo rozsah tabulky.\n";
const char *err_argc = "Nevhodny pocet argumentu.\n";
const char *err_rowlen = "Maximalni delka radku byla prekrocena.\n";
const char *err_missdata = "Ve vybrane oblasti chybi data.\n";
const char *err_emptyrow = "Prazdny radek.\n";


struct Arguments                                                            /* struktura obsahujici argumenty vyberu bunek a zvolenou operaci */
{
    long int row_beg;
    long int row_end;
    long int col_beg;
    long int col_end;
    int operation;
} arguments;

                                                                            /* deklarace funkci */
int get_params(int argc, char *argv[], struct Arguments *arguments);
int arg_test(int argc, int c, char *argv[]);
int read_line(char *buffer);
int do_op(char *buffer, int cont, struct Arguments arguments);



int main(int argc, char *argv[])
{
    char buffer[ROW_LENGTH];

    if (get_params(argc, argv, &arguments))
        return 1;

    while (1)
    {
        int cont;

        cont = read_line(buffer);
        if (do_op(buffer, cont, arguments))
            return 1;
    }

    return 0;
}

int get_params(int argc, char *argv[], struct Arguments *arguments)         /* funkce, ktera ulozi argumenty do promennych */
{
    if (strcmp("select", argv[1]) == 0)                                     /* do promenne operation konstantu symbolizujici vybranou operaci */
            arguments->operation = 1;

    else if (strcmp("avg", argv[1]) == 0)
            arguments->operation = 2;

    else if (strcmp("sum", argv[1]) == 0)
            arguments->operation = 3;

    else if (strcmp("min", argv[1]) == 0)
            arguments->operation = 4;

    else if (strcmp("max", argv[1]) == 0)
            arguments->operation = 5;

                                                                            /* pro druhy argument "--help" vypise napovedu a program ukonci */
    else if (strcmp("--help", argv[1]) == 0 && argc == argc_help)
    {
        printf("%s", help);
        return 1;
    }


    else                                                                    /* pokud se druhy argument nerovna zadne ze zadanych moznosti, */
    {
        fprintf(stderr, "%s", err_argformat);                               /*   vypise chybove hlaseni a program ukonci */
        return 1;
    }

                                                                            /* podle zvoleneho zpusobu vyberu bunek priradi hodnoty promennym */
                                                                            /*   row_beg = prvni radek vyberu, row_end = posledni radek vyberu */
                                                                            /*   col_beg = prvni sloupec vyberu, col_end = posledni sloupec vyberu */
    if (strcmp("row", argv[2]) == 0)
    {
        if (arg_test(argc, argc_row, argv) == 1)
            return 1;
        arguments->row_beg = strtol(argv[argc_row - 1], NULL, 10);
        arguments->row_end = arguments->row_beg;
        arguments->col_beg = 1;
        arguments->col_end = ROW_LENGTH;
    }

    else if (strcmp("col", argv[2]) == 0)
    {
        if (arg_test(argc, argc_col, argv) == 1)
            return 1;
        arguments->row_beg = 1;
        arguments->row_end = LONG_MAX;
        arguments->col_beg = strtol(argv[argc_col - 1], NULL, 10);
        arguments->col_end = arguments->col_beg;
    }

    else if (strcmp("rows", argv[2]) == 0)
    {
        if (arg_test(argc, argc_rows, argv) == 1)
            return 1;
        arguments->row_beg = strtol(argv[argc_rows - 2], NULL, 10);
        arguments->row_end = strtol(argv[argc_rows - 1], NULL, 10);
        arguments->col_beg = 1;
        arguments->col_end = ROW_LENGTH;
    }

    else if (strcmp("cols", argv[2]) == 0)
    {
        if (arg_test(argc, argc_cols, argv) == 1)
            return 1;
        arguments->row_beg = 1;
        arguments->row_end = LONG_MAX;
        arguments->col_beg = strtol(argv[argc_cols - 2], NULL, 10);
        arguments->col_end = strtol(argv[argc_cols - 1], NULL, 10);
    }

    else if (strcmp("range", argv[2]) == 0)
    {
        if (arg_test(argc, argc_range, argv) == 1)
            return 1;
        arguments->row_beg = strtol(argv[argc_range - 4], NULL, 10);
        arguments->row_end = strtol(argv[argc_range - 3], NULL, 10);
        arguments->col_beg = strtol(argv[argc_range - 2], NULL, 10);
        arguments->col_end = strtol(argv[argc_range - 1], NULL, 10);
    }

    else                                                                    /* pokud se prvni argument nerovna zadne ze zadanych moznosti, */
    {
        fprintf(stderr, "%s", err_argformat);                               /*   vypise chybove hlaseni a program ukonci */
        return 1;
    }

    return 0;
}

int arg_test(int argc, int c, char *argv[])                                 /* funkce overujici platnost argumentu */
{
    int i;

    if (argc != c)                                                          /* kontrola poctu argumentu */
    {
        fprintf(stderr, "%s", err_argc);
        return 1;
    }

    for (i = 3; i < argc; i++)                                              /* pro vsechny ciselne argumenty zkontroluj, */
    {
        double argument;

        if ((argument = strtod(argv[i], NULL)) == 0)                        /* zda jsou opravdu ciselne */
        {
            fprintf(stderr, "%s", err_argformat);
            return 1;
        }

        if (argument != (int) argument)                                     /* zda jsou to cela cisla */
        {
            fprintf(stderr, "%s", err_decarg);
            return 1;
        }

        if (argument < 1)                                                   /* ze nejsou rovny nule, nebo zaporne */
        {
            fprintf(stderr, "%s", err_negarg);
            return 1;
        }

        if (i == argc_rows - 1 || i == argc_cols - 1 || i == argc_range - 1)    /* ze pro vyber vice radku, ci sloupcu */
        {
            if (argv[i - 1] > argv[i])                                          /* neni druhy argument vyberu mensi nez prvni */
            {
                fprintf(stderr, "%s", err_argrange);
                return 1;
            }
        }
    }
    return 0;
}

int read_line(char *buffer)                                      /* funkce pro nacteni jednoho radku do pole znaku - bufferu */
{
     signed int character = 0;
     int char_pos = 0;

     while (character != '\n' && character != EOF)                          /* dokud nenarazi na konec radku, nebo celeho souboru */
     {
        character = getchar();                                              /* nacita jednotlive znaky */

        if (character != '\t' && character != ' ')                          /* pokud tento znak neslouzi k oddeleni dvou bunek */
            buffer[char_pos++] = character;                                 /* ulozi jej na pozici char_pos do bufferu a inkrementuje char_pos */

        else if ((buffer[char_pos - 1]) != ' ')                             /* kdyz znak oddeluje dve bunky, podiva se na predchozi znak ulozeny v bufferu, */
            buffer[char_pos++] = ' ';                                       /* a kdyz ten je jeste soucasti predchozi bunky, zapise mezeru pro oddeleni */

        if (char_pos >= ROW_LENGTH - 1)                                     /* kontrola prekroceni maximalni delky radku */
        {
            fprintf(stderr, "%s", err_rowlen);
            return 1;
        }
     }

     if (character == EOF)
        return 1;
     return 0;
}

int do_op(char *buffer, int cont, struct Arguments arguments)    /* funkce, ktera vybere bunky a nad nimi vykona danou operaci */
{
     char *token;
     const char s[] = " ";
     long double character = 0;
     static double pom = 0;
     static int row_counter = 1;
     int col_counter = 0;
     static int data_counter = 0;

     if (*(token = strtok(buffer, s)) == '\n')                              /* jestlize je prvni znak na radku znak konce radku, */
     {
         fprintf(stderr, "%s", err_emptyrow);                               /* vypise chybove hlaseni a ukonci program */
         return 1;
     }

     while (token != NULL)                                                  /* dokud lze z bufferu nacitat dalsi bunky*/
     {
         col_counter++;                                                     /* inkrementuj pocitadlo aktualniho sloupce  */
         if (row_counter >= arguments.row_beg && row_counter <= arguments.row_end       /* porovnani pozice soucasne bunky */
             && col_counter >= arguments.col_beg && col_counter <= arguments.col_end)	/* se zvolenymi hodnotami radku a sloupcu */
         {
                if (arguments.operation != 1 && (character = strtod(token, NULL)) == 0) /* pokud zvolenou operaci neni select */
                {                                                                       /* a bunka neobsahuje pouze cislice */
                    fprintf(stderr, "%s", err_dataformat);                              /* vypise chybu a ukonci program */
                    return 1;
                }

                switch (arguments.operation)                                /* rozdeleni podle operace */
                {
                        case 1:                                             /* 1 -----> select */
                                printf("%s\n", token);
                                break;
                        case 2:                                             /* 2 -----> avg */
                        case 3:                                             /* 3 -----> sum */
                                pom += character;
                                break;
                        case 4:                                             /* 4 -----> min */
                                if (data_counter == 0)                              /* pokud se jedna o prvni bunku ve vybrane oblasti */
                                    pom = character;                                /* ulozi aktualni hodnotu jako nejmensi */
                                else if (pom > character)                           /* jinak ji porovna s drive ulozenou nejmensi hodnotou */
                                    pom = character;
                                break;
                        case 5:                                             /* 5 -----> max */
                                if (data_counter == 0)                              /* pokud se jedna o prvni bunku ve vybrane oblasti */
                                    pom = character;                                /* ulozi aktualni hodnotu jako nejvetsi */
                                else if (pom < character)                           /* jinak ji porovna s drive ulozenou nejvetsi hodnotou */
                                    pom = character;
                                break;
                        default: break;
                }
                data_counter++;                                             /* inkrementuje pocitadlo platnych zpracovanych bunek */
         }
         token = strtok(NULL, s);                                           /* nacte nasledujici bunku */
     }

     if (col_counter < arguments.col_end && arguments.col_end != ROW_LENGTH) /* jestlize radek neobsahuje pozadovany pocet bunek */
     {
         fprintf(stderr, "%s", err_missdata);                               /* vypise chybove hlaseni a program ukonci */
         return 1;
     }

     if (cont == 1)                                                         /* jestlize program konci (cont = continue) */
     {
         if (row_counter < arguments.row_end && arguments.row_end != LONG_MAX) /* a tabulka neobsahovala dostatecny pocet radku */
         {
             fprintf(stderr, "%s", err_argrange);                           /* vypise chybove hlaseni a ukonci program */
             return 1;
         }

         if (arguments.operation == 2)                                      /* kdyz bylo zvolenou operaci avg */
            printf("%.10g\n", pom/data_counter);

         else if (arguments.operation > 2)                                  /* kdyz bylo zvolenou operaci sum, min, nebo max */
            printf("%f\n", pom);
         return 1;
     }

     row_counter++;                                                         /* inkrementuj pocitadlo aktualniho radku */
     return 0;
}
