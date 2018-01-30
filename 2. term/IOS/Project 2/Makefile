# Soubor: Makefile
# IOS, Projekt 2
# Datum: 3. 5. 2015
# Autor: Nikola Valesova, xvales02, FIT
# Prelozeno: gcc 4.8


CFLAGS=-std=gnu99 -pedantic -Wall -Wextra -Werror

all: h2o

h2o: h2o.o
	gcc $(CFLAGS) h2o.o -pthread -o h2o

h2o.o: h2o.c h2o.h
	gcc $(CFLAGS) -c h2o.c -o h2o.o

clean:
	rm -f h2o h2o.o
