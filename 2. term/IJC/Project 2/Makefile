# Soubor: Makefile
# Reseni IJC-DU2, priklad 1, 2
# Datum: 24. 4. 2015
# Autor: Nikola Valesova, xvales02, FIT
# Prelozeno: gcc 4.8

CFLAGS = -std=c99 -pedantic -Wall -Wextra

CCFLAGS = -std=c++11 -pedantic -Wall -Wextra
PROGRAMS = tail tail2 wordcount wordcount-dynamic
LIBFILES = htab_clear.o htab_init.o htab_lookup.o htab_free.o htab_foreach.o htab_remove.o htab_statistics.o hash_function.o

all: $(PROGRAMS)

tail: tail.c
	gcc $(CFLAGS) -o tail tail.c

tail2: tail2.cc
	g++ $(CCFLAGS) -o tail2 tail2.cc

wordcount: wordcount.o io.o libhtable.a
	gcc $(CFLAGS) -o wordcount -static io.o wordcount.o libhtable.a

wordcount-dynamic: wordcount.o io.o libhtable.so
	gcc $(CFLAGS) -o wordcount-dynamic wordcount.o io.o libhtable.so

libhtable.a: $(LIBFILES)
	ar crs libhtable.a $(LIBFILES)

libhtable.so: $(LIBFILES)
	gcc $(CFLAGS) -shared -fPIC -o libhtable.so $(LIBFILES)

wordcount.o: wordcount.c htable.h
	gcc $(FLAGS) -c wordcount.c -o wordcount.o

io.o:
	gcc $(CFLAGS) -c io.c -o io.o

hash_function.o: hash_function.c hash_function.h
	gcc $(CFLAGS) -fPIC -c hash_function.c

htab_clear.o: htab_clear.c htable.h
	gcc $(CFLAGS) -fPIC -c htab_clear.c -o htab_clear.o

htab_init.o: htab_init.c htable.h
	gcc $(CFLAGS) -fPIC -c htab_init.c -o htab_init.o

htab_lookup.o: htab_lookup.c htable.h
	gcc $(CFLAGS) -fPIC -c htab_lookup.c -o htab_lookup.o

htab_free.o: htab_free.c htable.h
	gcc $(CFLAGS) -fPIC -c htab_free.c -o htab_free.o

htab_foreach.o: htab_foreach.c htable.h
	gcc $(CFLAGS) -fPIC -c htab_foreach.c -o htab_foreach.o

htab_remove.o: htab_remove.c htable.h
	gcc $(CFLAGS) -fPIC -c htab_remove.c -o htab_remove.o

htab_statistics.o: htab_statistics.c htable.h
	gcc $(CFLAGS) -fPIC -c htab_statistics.c -o htab_statistics.o

