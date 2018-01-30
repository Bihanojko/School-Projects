CFLAGS=-std=c99 -pedantic -Wall -Wextra -O2 -lm	

all: prvocisla-inline prvocisla steg-decode


#sestaveni modulu
prvocisla.o: prvocisla.c error.h bit-array.h eratosthenes.h
	gcc $(CFLAGS) -c prvocisla.c -o prvocisla.o

eratosthenes.o: eratosthenes.c eratosthenes.h bit-array.h error.h
	gcc $(CFLAGS) -c eratosthenes.c -o eratosthenes.o

error.o: error.c error.h
	gcc $(CFLAGS) -c error.c -o error.o

ppm.o: ppm.c ppm.h eratosthenes.h bit-array.h error.h
	gcc $(CFLAGS) -c ppm.c -o ppm.o

steg-decode.o: steg-decode.c bit-array.h error.h eratosthenes.h ppm.h
	gcc $(CFLAGS) -c steg-decode.c -o steg-decode.o

#sestaveni programu
prvocisla: prvocisla.o error.o eratosthenes.o
	gcc $(CFLAGS) prvocisla.o error.o eratosthenes.o -o prvocisla

prvocisla-inline: prvocisla.c error.o eratosthenes.c 
	gcc $(CFLAGS) -DUSE_INLINE prvocisla.c eratosthenes.c error.o -o prvocisla-inline

steg-decode: steg-decode.o ppm.o error.o eratosthenes.o
	gcc $(CFLAGS) steg-decode.o error.o eratosthenes.o ppm.o -o steg-decode
