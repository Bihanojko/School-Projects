/** Soubor: bit-array.h
* Reseni IJC-DU1, priklad a), b)
* Datum: 26. 3. 2015
* Autor: Nikola Valesova, xvales02, FIT
* Prelozeno: gcc 4.8
* Definice maker a inline funkcí pro práci s bitovými poli
**/


#ifndef __BITARRAYH__
    #define __BITARRAYH__

#include <limits.h>
#include <ctype.h>

#include "error.h"


typedef unsigned long BitArray_t[];
typedef unsigned long T;


/* vytvoreni pole a inicializace vsech prvku na 0 */
#define BA_create(jmeno_pole,velikost) T jmeno_pole[(velikost/(sizeof(T) \
        * CHAR_BIT)) + ((velikost%(sizeof(T) * CHAR_BIT)) ? 2 : 1)];       \
        jmeno_pole[0] = velikost; \
        for (T i = 1; i < (velikost/(sizeof(T) \
        * CHAR_BIT)) + ((velikost%(sizeof(T) * CHAR_BIT)) ? 2 : 1); i++) jmeno_pole[i] = 0;

#define DU1_GET_BIT_(p,i) \
    (p[(i / (sizeof(p[0]) * CHAR_BIT)) + 1] & \
    ((__typeof__(p[0])) 1) << (i%(sizeof(p[0]) * CHAR_BIT))) != 0

#define DU1_SET_BIT_(p,i,b) \
    b ? (p[(i / (sizeof(p[0]) * CHAR_BIT)) + 1] |= \
    ((__typeof__(p[0])) 1) << (i%(sizeof(p[0]) * CHAR_BIT))) : \
    (p[(i / (sizeof(p[0]) * CHAR_BIT)) + 1] &= \
    ~(((__typeof__(p[0])) 1) << (i%(sizeof(p[0]) * CHAR_BIT))))


#ifndef USE_INLINE

/* vraci velikost pole */
#define BA_size(jmeno_pole) (jmeno_pole[0])

/* kontroluje meze a vraci hodnotu na danem indexu */
#define BA_get_bit(jmeno_pole,index) ((index > jmeno_pole[0]) ? \
        (FatalError("Index %ld mimo rozsah 0..%ld", \
        (long)index, (long)jmeno_pole[0]), 0) : \
        DU1_GET_BIT_(jmeno_pole,index))

/* kontroluje meze a nastavi prvek dany indexem na 0, nebo 1 */
#define BA_set_bit(jmeno_pole,index,vyraz) \
        if (index > jmeno_pole[0]) FatalError("Index %ld mimo rozsah 0..%ld", (long)index, \
        (long)jmeno_pole[0]);   \
        else DU1_SET_BIT_(jmeno_pole,index,vyraz)

#else


/* vraci velikost pole */
inline T BA_size(BitArray_t jmeno_pole)
{
    return jmeno_pole[0];
}

/* kontroluje meze a vraci hodnotu na danem indexu */
inline int BA_get_bit(BitArray_t jmeno_pole, T index)
{
    if (index > jmeno_pole[0])
        FatalError("Index %ld mimo rozsah 0..%ld", (long)index, (long)jmeno_pole[0]);
    return (DU1_GET_BIT_(jmeno_pole,index));
}

/* kontroluje meze a nastavi prvek dany indexem na 0, nebo 1 */
inline void BA_set_bit(BitArray_t jmeno_pole, T index, int vyraz)
{
    if (index > jmeno_pole[0])
	FatalError("Index %ld mimo rozsah 0..%ld", (long)index, (long)jmeno_pole[0]);
    else
        DU1_SET_BIT_(jmeno_pole,index,vyraz);
}

#endif 	//pro USE_INLINE
#endif
