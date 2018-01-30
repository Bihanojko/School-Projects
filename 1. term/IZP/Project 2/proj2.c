/*
 * Soubor:  proj2.c
 * Datum:   2014/11/29
 * Autor:   Nikola Valesova, xvales02@stud.fit.vutbr.cz
 * Projekt: Iteracni vypocty, projekt c. 2 pro predmet IZP
 * Popis:   Program urcuje vzdalenost a vysku mereneho objektu
 */

const char *help =
"Program Iteracni vypocty\n"
"Autor: Nikola Valesova (c) 2014\n\n"
"Program pocita vzdalenost a vysku mereneho objektu.\n"
"Pouziti: ./proj2 --help\n"
"         ./proj2 --tan A N M\n"
"         ./proj2 [-c X] -m A [B]\n\n"
"Popis parametru:\n"
"Program se spousti zadanim vyse uvedenych parametru. Parametr --help vytiskne "
"napovedu a program ukonci. Parametr --tan srovna presnost vypoctu tangens "
"uhlu A (v radianech) mezi vypoctem tangens pomoci Taylorova polynomu, "
"zretezeneho zlomku a volanim tan z matematicke knihovny.\n"
"Argumenty N a M udavaji, v kolikatych iteracich iteracniho vypoctu ma "
"srovnani probihat, pricemz 0 < N <= M < 14.\n"
"-m vypocita a zmeri vzdalenosti. "
"Argument A udava velikost uhlu alfa v radianech. Program vypocita a vypise "
"vzdalenost mereneho objektu. 0 < A <= 1.4 < pi/2"
"Pokud je zadan argument B, udava velikost uhlu beta v radianech. Program "
"spocita a vypise i vysku mereneho objektu. 0 < B <= 1.4 < pi / 2\n"
"Argument -c nastavuje vysku mericiho pristroje (X), kde 0 < X <= 100. "
"Tento argument je volitelny a pokud neni zadan, pouzije se implicitni "
"vyska 1.5 metru.\n";


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


#define ARGC_HELP   2                                                       /* definice poctu argumentu pro dany zpusob vyberu bunek */
#define ARGC_TAN    5
#define ARGC_M      4
#define ARGC_C      6

#define ITERATION_COUNT     13                                              /* pocet iteraci pro vypocet Taylorova polynomu */
#define MAX_HEIGHT          100.0                                           /* maximalni vyska mericiho pristroje */
#define DEFAULT_HEIGHT      1.5                                             /* implicitni vyska mericiho pristroje */
#define D_ITER		        8						                        /* pocet iteraci pro funkci cfrac_tan */

    /* chybove stavy */
const char *err_argformat = "Nevhodny format argumentu.\n";
const char *err_negarg = "Argument je roven nule, nebo zaporny.\n";
const char *err_argc = "Nevhodny pocet argumentu.\n";
const char *err_argrange = "Argument vyberu iterace lezi mimo povoleny rozsah.\n";
const char *err_decarg = "Argument vyberu nesmi byt desetinne cislo.\n";

    /* prototypy funkci */
int choose_op(int argc, char *argv[], int *operation, double *b);
int arg_test(int argc, int c, char *argv[], int *operation);
double taylor_tan(double x, unsigned int n);
double cfrac_tan(double x, unsigned int n);
void tan_comparison(double a, unsigned int n, unsigned int m);
double absolute_value(double a);
void count_distance(double c, double a, double b);


int main(int argc, char *argv[])
{
    int operation;
    double b = 0.0;

    if (choose_op(argc, argv, &operation, &b))
        return 1;

    switch (operation)
    {
        case 0:
            printf("%s", help); break;                                      /* vypise napovedu a program ukonci */
        case 1:
            tan_comparison(atof(argv[2]), atoi(argv[3]), atoi(argv[4]));    /* volani funkce na vypocet hodnoty tangens */
            break;
        case 2:
            count_distance(DEFAULT_HEIGHT, atof(argv[2]), b); break;        /* volani funkce na vypocet vzdalenosti (a vysky) objektu */
        case 3:
            count_distance(atof(argv[2]), atof(argv[4]), b); break;         /* volani funkce na vypocet vzdalenosti (a vysky) objektu se zadanou vyskou mericiho pristroje */
        default:
            fprintf(stderr, "%s", err_argformat); break;                    /* vypise chybove hlaseni a program ukonci */
    }

    return 0;
}

int choose_op(int argc, char *argv[], int *operation, double *b)            /* funkce, ktera zpracovava argument vyberu operace */
{
    if (strcmp("--help", argv[1]) == 0 && argc == ARGC_HELP)                /* druhy argument "--help" -> operace 0 */
        *operation = 0;

    else if (strcmp("--tan", argv[1]) == 0)                                 /* druhy argument "--tan" -> operace 1 */
    {
        *operation = 1;
        if (arg_test(argc, ARGC_TAN, argv, operation))
            return 1;
    }

    else if (strcmp("-m", argv[1]) == 0)                                    /* druhy argument "-m" -> operace 2 */
    {
        *operation = 2;
        if (arg_test(argc, ARGC_M, argv, operation) == 1)
            return 1;
        if (argc == ARGC_M)
            *b = atof(argv[3]);
    }

    else if (strcmp("-c", argv[1]) == 0 && strcmp("-m", argv[3]) == 0)      /* druhy argument "-c" a ctvrty "-m" -> operace 3 */
    {
        *operation = 3;
        if (arg_test(argc, ARGC_C, argv, operation) == 1)
            return 1;
        if (argc == ARGC_C)
            *b = atof(argv[5]);
    }

    else                                                                    /* druhy se argument nerovna zadne ze zadanych moznosti */
        *operation = 4;                                                     /* -> operace 4 */

    return 0;
}

int arg_test(int argc, int c, char *argv[], int *operation)                 /* funkce overujici platnost argumentu */
{
    int i;
    double argument;

    if (argc != c)                                                          /* kontrola poctu argumentu */
    {
        if (*operation != 1 && argc == c - 1)                               /* pokud pouze chybi argument udavajici uhel beta */
            c -= 1;                                                         /* inkrementuje pocet argumentu */
        else
        {
            fprintf(stderr, "%s", err_argc);                                /* jinak vypise chybove hlaseni a program ukonci */
            return 1;
        }
    }

    if (*operation == 1 && (argv[c - 2] > argv[c - 1]))                     /* ze neni druhy argument vyberu mensi nez prvni */
    {
        fprintf(stderr, "%s", err_argrange);
        return 1;
    }

    for (i = 2; i < c; i++)
    {
        if ((argument = strtod(argv[i], NULL)) == 0)                        /* zda jsou argumenty opravdu ciselne */
        {
            if (*operation != 3 && i != 3)                                  /* pokud je neciselny argument ruzny od "-m" ve treti operaci */
            {
                fprintf(stderr, "%s", err_argformat);                       /* vypise chybove hlaseni a program ukonci */
                return 1;
            }
        }

        if (*operation == 1 && i > 2 && argument != (int) argument)         /* zda jsou agumenty cela cisla */
        {
            fprintf(stderr, "%s", err_decarg);
            return 1;
        }

        if (*operation == 1 && i > 2 && argument < 1)                       /* ze nejsou rovny nule, nebo zaporne */
        {
            fprintf(stderr, "%s", err_negarg);
            return 1;
        }

        if ((*operation == 1 && i > 2 && argument > ITERATION_COUNT) ||     /* ze nelezi mimo povolene rozsahy */
        ((*operation == 2  || (*operation == 3 && i > 3)) &&
            (argument <= 0 || argument > 1.4)) ||
        (*operation == 3 && i == 2 && argument > MAX_HEIGHT))
        {
            fprintf(stderr, "%s", err_argrange);
            return 1;
        }

        if (*operation == 3 && i == 2 && (argument <= 0))                  /* ze nejsou rovny nule, nebo zaporne */
        {
            fprintf(stderr, "%s", err_negarg);
            return 1;
        }
    }
    return 0;
}


double cfrac_tan(double x, unsigned int n)					                /* funkce na vypocet hodnoty tangens pomoci zretezenych zlomku */
{
    double cf = 0.0;
    int k;

    for (k = 2 * (n +1) - 1; k > 0; k -= 2)
        cf = k / x - (1/cf);

    return 1 / cf;
}


double taylor_tan(double x, unsigned int n)					                /* funkce na vypocet hodnoty tangens pomoci Taylorova polynomu */
{
    const double coef[ITERATION_COUNT] = {1.0,                              /* pole podilu citatelu a jmenovatelu Taylorova polynomu */
    0.33333333333333333333333333333333333333333333333333333333333333333,
    0.13333333333333333333333333333333333333333333333333333333333333333,
    0.05396825396825396825396825396825396825396825396825396825396825396,
    0.02186948853615520282186948853615520282186948853615520282186948853,
    0.00886323552990219656886323552990219656886323552990219656886323552,
    0.00359212803657248101692546136990581435025879470323914768359212803,
    0.00145583438705131826824948518070211191904313597435290556983678676,
    0.00059002744094558598137807599370002108875435984057210325495417309,
    0.00023912911424355248148573145888511280932056909764432019597893614,
    0.00009691537956929450325595875000388809377858755461215115208833120,
    0.00000231048964019510905693098176144686519251849075372293240286316,
    0.00001591890506932900509586911053577624007427852851971056200972486};

    unsigned int k;
    double a = x;
    double cf = 0.0;

    for (k = 0; k < n; k++)
    {
        cf += coef[k] * a;
        a *= x * x;
    }

    return cf;
}

void tan_comparison(double a, unsigned int n, unsigned int m)			    /* procedura, ktera vypise hodnoty tangens spocitane */
{										                                    /* tremi ruznymi zpusoby a jejich odchylky */
    for ( ; n <= m; n++)
    {
        double tan_m = tan(a);
        double tan_t = taylor_tan(a, n);
        double tan_c = cfrac_tan(a, n);

        printf("%d %e %e %e %e %e\n", n, tan_m, tan_t,
	absolute_value(tan_t - tan_m), tan_c, absolute_value(tan_c - tan_m));
    }

    return;
}

double absolute_value(double a)							                    /* funkce, ktera vraci absolutni hodnotu zadaneho parametru */
{
    return a < 0 ? -a : a;
}


void count_distance(double c, double a, double b)				            /* procedura, ktera pocita vzdalenost a vysku mereneho objektu */
{
    double d = c / cfrac_tan(a, D_ITER);
    printf("%.10e\n", d);

    if (b != 0)
        printf("%.10e\n", cfrac_tan(b, D_ITER) * d + c);

    return;
}

