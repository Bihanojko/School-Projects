/*
 * Soubor:  proj3.c
 * Datum:   2014/12/14
 * Autor:   Nikola Valesova, xvales02@fit.vutbr.cz
 * Projekt: Pruchod bludistem, projekt c. 3 pro predmet IZP
 * Popis:   Program, ktery najde pruchod ven z daneho bludiste
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>
#include <ctype.h>

#define ARGC_HELP   2
#define ARGC_TEST   3
#define ARGC_RPATH  5
#define ARGC_LPATH  5
#define ARGC_SHORT  5
#define MAX_LENGTH  50

typedef struct {
    int operation;
    int r_start;
    int c_start;
    char maze[MAX_LENGTH];
} Arguments;

typedef struct {
  int rows;
  int cols;
  unsigned char *cells;
} Map;

enum directions{
LEFT,
UP,
RIGHT,
DOWN,
};

void print_help();
int start_border(Map *map, int x, int y, int leftright);
bool isborder(Map *map, int r, int c, int border);
int path(Map *map, int r, int c, int leftright);
int shortest(Map *map, int r_start, int c_start);
int read_file(Map *map, Arguments *arguments);
int get_params(int argc, char *argv[], Arguments *arguments);
int test(Map *map, Arguments *arguments);
int JsemMimoPole(int r, int c, Map *map);


const char *HELPMSG =
"Program Pruchod bludistem\n"
"Autor: Nikola Valesova (c) 2014\n\n"
"Program, ktery najde cestu z bludiste.\n"
"Pouziti: ./proj3 --help\n"
"         ./proj3 --test soubor.txt\n"
"         ./proj3 --rpath R C bludiste.txt\n"
"         ./proj3 --lpath R C bludiste.txt\n\n"
"Popis argumentu:\n"
"Program se spousti zadanim vyse uvedenych argumentu.\n"
"--help vytiskne napovedu a program ukonci\n"
"--test overi, zda soubor.txt obsahuje validni mapu bludiste\n"
"--rpath najde cestu z bludiste z pozice: radek R a sloupec C "
"podle pravidla prave ruky\n"
"--lpath najde cestu z bludiste z pozice: radek R a sloupec C "
"podle pravidla leve ruky\n";

const char *ECODEMSG[] =                                                        /* Chybova hlaseni */
{
	/* EOK */
	"Vse probehlo v poradku.\n",
	/* EARGS */
	"Chybne zadane argumenty.\n",
	/* EFILE */
	"Chyba pri nacitani souboru.\n",
	/* EMAT */
	"Chybny format matice.\n",
	/* EOVER */
	"Zadany bod lezi mimo matici.\n",
	/* EINPUT */
	"Chybny vstup.\n",
	/* EALLOC */
	"Chyba pri alokaci pole.\n",
	/* ECLOSE */
	"Chyba pri zavirani souboru.\n",
};


enum tecodes
{
	EOK = 0,     /* Bez chyby */
	EARGS,       /* Chybne argumenty */
	EFILE,       /* Chybny vstupni soubor */
	EMAT,        /* Chybny format matice */
	EOVER,       /* Zadany bod mimo matici */
	EINPUT,      /* Chyba na vstupu */
	EALLOC,      /* Chyba pri alokaci */
	ECLOSE,      /* Chyba pri uzavirani souboru */
};

int main(int argc, char *argv[])
{
    Map map;
    Arguments arguments;
    int errnum = EOK;
    map.cells = NULL;

    errnum = get_params(argc, argv, &arguments);                                    /* zpracuj argumenty */

    if (!errnum && arguments.operation > 1)
        errnum = read_file(&map, &arguments);                                       /* nacti data z externiho souboru */

    if (!errnum && arguments.operation > 2)
        errnum = test(&map, &arguments);

    if (!errnum && arguments.operation > 2 && arguments.operation < 5)
    {
        if ((arguments.r_start != 1 && arguments.r_start != map.rows) ||
            (arguments.c_start != 1 && arguments.c_start != map.cols))
                return EINPUT;
    }

    if (!errnum)
    {
        switch (arguments.operation)
        {
            case 1: print_help(); break;                                            /* volba vypsani napovedy */
            case 2:                                                                 /* volba otestovani validnosti mapy */
                test(&map, &arguments) ? printf("Invalid\n") : printf("Valid\n");
                break;
            case 3:                                                                 /* volba nalezeni cesty pomoci prave ruky */
                errnum = path(&map, arguments.r_start-1,
                arguments.c_start-1, RIGHT); break;
            case 4:                                                                 /* volba nalezeni cesty pomoci leve ruky */
                errnum = path(&map, arguments.r_start-1,
                arguments.c_start-1, LEFT); break;
            case 5:                                                                 /* volba nalezeni nejkratsi cesty z bludiste */
                errnum = shortest(&map, arguments.r_start - 1,
                arguments.c_start - 1); break;
            default: break;
        }
    }

    if (map.cells != NULL)
        free(map.cells);

    if (errnum != EOK)                                                          /* vypis chybu */
        fprintf(stderr, "%s", ECODEMSG[errnum]);

    return EOK;
}

int get_params(int argc, char *argv[], Arguments *arguments)                    /* funkce na zpracovani argumentu */
{
    if (strcmp("--help", argv[1]) == 0 && argc == ARGC_HELP)                    /* prvni argument "--help" */
        arguments->operation = 1;
    else if (strcmp("--test", argv[1]) == 0 && argc == ARGC_TEST)               /* prvni argument "--test" */
    {
        arguments->operation = 2;
        strcpy(arguments->maze, argv[2]);
    }
    else if (strcmp("--rpath", argv[1]) == 0 && argc == ARGC_RPATH)             /* prvni argument "--rpath" */
        arguments->operation = 3;
    else if (strcmp("--lpath", argv[1]) == 0 && argc == ARGC_LPATH)             /* prvni argument "--lpath" */
        arguments->operation = 4;
    else if (strcmp("--shortest", argv[1]) == 0 && argc == ARGC_SHORT)          /* prvni argument "--shortest" */
        arguments->operation = 5;
    else
        return EARGS;

    if (arguments->operation > 2)
    {
        arguments->r_start = atoi(argv[2]);
        arguments->c_start = atoi(argv[3]);
        strcpy(arguments->maze, argv[4]);
    }
    return EOK;
}

void print_help()                                                               /* procedura na vypsání nápovìdy */
{
    printf("%s", HELPMSG);
}

int read_file(Map *map, Arguments *arguments)                                   /* funkce na nacteni mapy ze souboru */
{
    FILE *fr;

    if ((fr = fopen(arguments->maze, "r")) ==  NULL)                            /* jestlize se otevreni souboru nezdarilo, */
        return EFILE;                                                           /* vrat chybu */

    if (fscanf(fr, "%d", &map->rows) == 0 ||                                    /* jestlize se nacteni poctu radku */
        fscanf(fr, "%d", &map->cols) == 0 ||                                    /* nebo poctu sloupcu nezdrilo */
        map->rows < 0 || map->cols < 0)                                         /* nebo jsou tato cisla zaporna */
        {
            fclose(fr);
            return EMAT;                                                        /* vrat chybu */
        }

    if ((map->cells = malloc(map->rows * map->cols *                            /* alokuj pole tolika prvku, kolik ma mapa bunek */
        sizeof(unsigned char))) == NULL)
        {
            fclose(fr);
            return EALLOC;
        }

    int a;
    for (int i = 0; i < map->rows * map->cols; i++)                             /* pro kazdy prvek mapy */
    {
        if (fscanf(fr, "%d", &a) == 0)                                          /* nacti dalsi cislo ze souboru */
        {
            fclose(fr);
            return EMAT;
        }
        map->cells[i] = a;                                                      /* a uloz na nasledujici pozici pole */
    }

    if (fclose(fr) == EOF)                                                      /* uzavri soubor */
        return ECLOSE;

    return EOK;
}

int path(Map *map, int r, int c, int leftright)                                 /* funkce na urceni cesty z bludiste */
{
    int direction = start_border(map, r, c, leftright);                         /* urci pocatecni smer */

    do
    {
        if (direction == LEFT)                                                  /* jestlize ma jit doleva, */
        {
            if (!isborder(map, r, c, LEFT))                                     /* zkontroluj, zda je vlevo zed */
            {
                printf("%d,%d\n", r + 1, c + 1);                                /* vypis aktualni pozici */
                c -= 1;                                                         /* posun se na dalsi bunku */
                if ((r + c) % 2 == 0)                                           /* jestlize trojuhelnik smeruje dolu, */
                    direction = (leftright == RIGHT) ? UP : LEFT;               /* prirad novy smer */
                else                                                            /* jestlize trojuhelnik smeruje nahoru */
                    direction = (leftright == RIGHT) ? LEFT : DOWN;             /* prirad novy smer */
            }
            else                                                                /* kdyz se posun do dalsi bunky nezdaril */
            {
                if ((r + c) % 2 == 0)                                           /* jestlize trojuhelnik smeruje dolu, */
                    direction = (leftright == RIGHT) ? RIGHT : UP;              /* prirad novy smer */
                else                                                            /* jestlize smeruje nahoru */
                    direction = (leftright == RIGHT) ? DOWN : RIGHT;            /* prirad jiny novy smer */
            }
        }

        else if (direction == UP)                                                /* jestlize ma jit nahoru */
        {
            if (!isborder(map, r, c, UP))
            {
                printf("%d,%d\n", r + 1, c + 1);
                r -= 1;
                direction = (leftright == RIGHT) ? RIGHT : LEFT;
            }
            else
                direction = (leftright == RIGHT) ? LEFT : RIGHT;

        }

        else if (direction == RIGHT)                                            /* jestlize ma jit doprava */
        {
            if (!isborder(map, r, c, RIGHT))
            {
                printf("%d,%d\n", r + 1, c + 1);
                c += 1;
                if ((r + c) % 2 == 0)
                    direction = (leftright == RIGHT) ? RIGHT : UP;
                else
                    direction = (leftright == RIGHT) ? DOWN : RIGHT;
            }
            else
            {
                if ((r + c) % 2 == 0)
                    direction = (leftright == RIGHT) ? UP : LEFT;
                else
                    direction = (leftright == RIGHT) ? LEFT : DOWN;
            }
        }

        else if (direction == DOWN)                                             /* jestlize ma jit dolu */
        {
            if (!isborder(map, r, c, DOWN))
            {
                printf("%d,%d\n", r + 1, c + 1);
                r += 1;
                direction = (leftright == RIGHT) ? LEFT : RIGHT;
            }
            else
                direction = (leftright == RIGHT) ? RIGHT : LEFT;
        }
        else
            return EINPUT;
    } while (r != -1 && r != map->rows && c != -1 && c != map->cols);           /* dokud nevyjdes z bludiste */
    return EOK;
}

int shortest(Map *map, int r_start, int c_start)                                /* funkce na nalezeni nejkratsi cesty */
{
    unsigned char *array;                                                       /* pole, do ktereho se budou ukladat vzdalenosti bodu od pocatku */
    unsigned char value = 0;
    int counter = 0;
    int array_values[(map->rows + map->cols) * 2];                              /* kdyz je nalezen vychod z bludiste, ulozi se sem jeho vzdalenost od pocatku */
    int array_max_r[(map->rows + map->cols) * 2];                               /* kdyz je nalezen vychod z bludiste, ulozi se sem index jeho radku */
    int array_max_c[(map->rows + map->cols) * 2];                               /* kdyz je nalezen vychod z bludiste, ulozi se sem index jeho sloupce */

    if ((array = malloc(map->rows * map->cols * sizeof(unsigned char)))         /* alokace pameti pro pole array */
    == NULL)
        return EALLOC;

    for (int i = 0; i < map->rows * map->cols; i++)                             /* kazdemu prvku pole array */
        array[i] = UCHAR_MAX;                                                   /* se priradi maximalni mozna hodnota */

    array[r_start * map->cols + c_start] = value;                               /* pocatecnimu bodu se priradi hodnota nula */

    for (int i = 0; i < UCHAR_MAX; i++)                                         /* pro vsechny mozne hodnoty/vzdalenosti od pocatku */
    {
        value++;
        for (int r = 0; r < map->rows; r++)
        {                                                                       /* pro kazdy prvek pole array */
            for (int c = 0; c < map->cols; c++)
            {
                if (array[r * (map->rows + 1) + c] == value - 1)                                                /* jestlize ma prave zpracovavanou hodnotu */
                {
                    if ((!isborder(map, r, c, LEFT) && JsemMimoPole(r, c - 1, map)) ||                          /* vlevo se nachazi vychod z bludiste */
                        (!isborder(map, r, c, RIGHT) && JsemMimoPole(r, c + 1, map)) ||                         /* vpravo se nachazi vychod z bludiste */
                        ((r + c) % 2 == 0 && !isborder(map, r, c, UP) && JsemMimoPole(r - 1, c, map)) ||        /* nahore se nachazi vychod z bludiste */
                        ((r + c) % 2 == 1 && !isborder(map, r, c, DOWN) && JsemMimoPole(r + 1, c, map)))        /* dole se nachazi vychod z bludiste */
                    {
                        array_values[counter] = value - 1;                      /* do pole array_values se pripise vzdalenost tohoto vychodu */
                        array_max_r[counter] = r;                               /* do pole array_max_r se pripise index radku tohoto vychodu */
                        array_max_c[counter] = c;                               /* do pole array_max_c se pripise index sloupce tohoto vychodu */
                        counter++;
                    }

                    else if (!isborder(map, r, c, LEFT) &&                              /* jestlize vlevo neni zed */
                    array[r * (map->rows + 1) + c - 1] > value)                         /* jestlize bunka vlevo obsahuje vyssi hodnotu */
                        array[r * (map->rows + 1) + c - 1] = value;                     /* prirad do bunky vedle o jedno vetsi hodnotu nez ma soucasna */

                    else if (!isborder(map, r, c, RIGHT) &&                             /* jestlize vpravo neni zed */
                    array[r * (map->rows + 1) + c + 1] > value)                         /* jestlize bunka vpravo obsahuje vyssi hodnotu */
                        array[r * (map->rows + 1) + c + 1] = value;                     /* prirad do bunky vedle o jedno vetsi hodnotu nez ma soucasna */

                    else if ((r + c) % 2 == 0 && !isborder(map, r, c, UP) &&            /* kdyz trojuhelnik smeruje dolu a nahore neni zed */
                    array[(r - 1) * (map->rows + 1) + c] > value)                       /* jestlize bunka nahore obsahuje vyssi hodnotu */
                        array[(r - 1) * (map->rows + 1) + c] = value;                   /* prirad do bunky vedle o jedno vetsi hodnotu nez ma soucasna */

                    else if ((r + c) % 2 == 1 && !isborder(map, r, c, DOWN) &&          /* kdyz trojuhelnik smeruje nahoru a dole neni zed */
                    array[(r + 1) * (map->rows + 1) + c] > value)                       /* jestlize bunka dole obsahuje vyssi hodnotu */
                        array[(r + 1) * (map->rows + 1) + c] = value;                   /* prirad do bunky vedle o jedno vetsi hodnotu nez ma soucasna */

                }
            }
        }
    }

    int index;
    unsigned char min = UCHAR_MAX;
    for (int i = 0; i < counter; i++)                                           /* nalezeni nejmensiho ohodnoceni vychodu z bludiste */
    {
        if (array_values[i] < min)
        {
            min = array_values[i];
            index = i;
        }
    }

    if (min == 0 || min == UCHAR_MAX)
        return EINPUT;

    unsigned char r = array_max_r[index];
    unsigned char c = array_max_c[index];
    unsigned char array_r[map->rows * map->cols];
    unsigned char array_c[map->rows * map->cols];

    for (int x = array_values[index]; x > 0; x--)
    {
        array_r[x] = r;
        array_c[x] = c;
        if (array[r * (map->rows + 1) + c - 1] == x - 1 && !isborder(map, r, c, LEFT))
            c -= 1;
        if (array[r * (map->rows + 1) + c + 1] == x - 1 && !isborder(map, r, c, RIGHT))
            c += 1;
        if ((r + c) % 2 == 0 && array[(r - 1) * (map->rows + 1) + c] == x - 1 && !isborder(map, r, c, UP))
            r -= 1;
        if ((r + c) % 2 == 1 && array[(r + 1) * (map->rows + 1) + c] == x - 1 && !isborder(map, r, c, DOWN))
            r += 1;
    }
    array_r[0] = r;
    array_c[0] = c;

    for (int i = 0; i <= array_values[index]; i++)
    {
        printf("%d, %d\n", array_r[i] + 1, array_c[i] + 1);
    }

    free(array);
    return EOK;
}

int JsemMimoPole(int r, int c, Map *map)
{
    if (r == -1 || r == map->rows || c == -1 || c == map->cols)
        return 1;
    else return 0;
}

int test(Map *map, Arguments *arguments)                                        /* funkce testujici validnost mapy */
{
    for (int r = 0; r < map->rows; r++)
    {
        for (int c = 0; c < map->cols; c++)
        {                                                                       /* pro kazdou bunku zkontroluj, */
            if (map->cells[r * map->cols + c] > 7)                              /* zda obsahuje hodnotu 0 az 7 */
                return EMAT;

            if (c < map->cols - 1)
                if (isborder(map, r, c, RIGHT) != isborder(map, r, c + 1, LEFT))    /* zda je jeji prava hranice shodna */
                    return EMAT;                                                       /* s levou hranici bunky vpravo od ni */

            if (r < map->rows - 1 && (r + c) % 2 == 1)                          /* zda je jeji dolni hranice shodna */
                if (isborder(map, r, c, DOWN) != isborder(map, r + 1, c, UP))   /* s horni hranici bunky pod ni */
                    return EMAT;
        }
    }

    if (arguments->operation > 2)
    {
        if (arguments->r_start < 0 || arguments->c_start < 0 ||
            arguments->r_start > map->rows || arguments->c_start > map->cols)
            return EOVER;
    }

    return EOK;
}

bool isborder(Map *map, int r, int c, int border)                               /* funkce, ktera vraci true, kdyz ma bunka */
{                                                                               /* na dane hranici zed */
    switch (border)
    {
        case 0:                                                                 /* leva hnaice */
            if ((map->cells[r * map->cols + c] & 1) == 1)
                return true;
            break;
        case 2:                                                                 /* prava hranice */
            if ((map->cells[r * map->cols + c] & 2) == 2)
                return true;
            break;
        case 1:                                                                 /* horni hranice */
        case 3:                                                                 /* dolni hranice */
            if ((map->cells[r * map->cols + c] & 4) == 4)
                return true;
            break;
        default: break;
    }
    return false;
}

int start_border(Map *map, int x, int y, int leftright)                         /* funkce pro nalezeni hrany, kterou se ma */
{                                                                               /* po vchodu do bludiste zacit */
    if (y == 0)                                                                 /* jestlize se vchazi z leve strany */
    {
        if (x % 2 == 0 && x != 0)                                               /* jestlize trojuhelnik smeruje dolu */
            return (leftright == LEFT) ? UP : RIGHT;
        else
            return (leftright == LEFT) ? RIGHT : DOWN;
    }

    else if (y == map->cols - 1)                                                /* jestlize se vchazi z prave strany */
    {
        if ((x + y) % 2 == 0 && x != map->rows - 1)                             /* jestlize trojuhelnik smeruje dolu */
            return (leftright == LEFT) ? LEFT : UP;
        else
            return (leftright == LEFT) ? DOWN : LEFT;
    }

    else if (x == 0)                                                             /* jestlize se vchazi shora */
        return (leftright == LEFT) ? RIGHT : LEFT;

    else if (x == map->rows - 1)                                                /* jestlize se vchazi zespod */
        return (leftright == LEFT) ? LEFT : RIGHT;

    else                                                                        /* jinak -> chyba - neplatna pozice */
        return EINPUT;
}
