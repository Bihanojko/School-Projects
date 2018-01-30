/** Soubor: io.h
* Reseni IJC-DU2, priklad 2
* Datum: 24. 4. 2015
* Autor: Nikola Valesova, xvales02, FIT
* Prelozeno: gcc 4.8
* Deklarace funkce na nacteni jednoho slova ze souboru
**/


#ifndef IO_H_INCLUDED
#define IO_H_INCLUDED


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>


int fgetw(char *s, int max, FILE *f);


#endif  //IO_H_INCLUDED
