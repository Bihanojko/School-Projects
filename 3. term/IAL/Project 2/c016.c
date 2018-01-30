
/* c016.c: **********************************************************}
{* Téma:  Tabulka s Rozptýlenými Položkami
**                      První implementace: Petr Přikryl, prosinec 1994
**                      Do jazyka C prepsal a upravil: Vaclav Topinka, 2005
**                      Úpravy: Karel Masařík, říjen 2014
**                      Úpravy: Radek Hranický, říjen 2014
**                      Úpravy: Radek Hranický, listopad 2015
**
** Vytvořete abstraktní datový typ
** TRP (Tabulka s Rozptýlenými Položkami = Hash table)
** s explicitně řetězenými synonymy. Tabulka je implementována polem
** lineárních seznamů synonym.
**
** Implementujte následující procedury a funkce.
**
**  HTInit ....... inicializuje tabulku před prvním použitím
**  HTInsert ..... vložení prvku
**  HTSearch ..... zjištění přítomnosti prvku v tabulce
**  HTDelete ..... zrušení prvku
**  HTRead ....... přečtení hodnoty prvku
**  HTClearAll ... zrušení obsahu celé tabulky (inicializace tabulky
**                 poté, co již byla použita)
**
** Definici typů naleznete v souboru c016.h.
**
** Tabulka je reprezentována datovou strukturou typu tHTable,
** která se skládá z ukazatelů na položky, jež obsahují složky
** klíče 'key', obsahu 'data' (pro jednoduchost typu float), a
** ukazatele na další synonymum 'ptrnext'. Při implementaci funkcí
** uvažujte maximální rozměr pole HTSIZE.
**
** U všech procedur využívejte rozptylovou funkci hashCode.  Povšimněte si
** způsobu předávání parametrů a zamyslete se nad tím, zda je možné parametry
** předávat jiným způsobem (hodnotou/odkazem) a v případě, že jsou obě
** možnosti funkčně přípustné, jaké jsou výhody či nevýhody toho či onoho
** způsobu.
**
** V příkladech jsou použity položky, kde klíčem je řetězec, ke kterému
** je přidán obsah - reálné číslo.
*/

#include "c016.h"

int HTSIZE = MAX_HTSIZE;
int solved;

/*          -------
** Rozptylovací funkce - jejím úkolem je zpracovat zadaný klíč a přidělit
** mu index v rozmezí 0..HTSize-1.  V ideálním případě by mělo dojít
** k rovnoměrnému rozptýlení těchto klíčů po celé tabulce.  V rámci
** pokusů se můžete zamyslet nad kvalitou této funkce.  (Funkce nebyla
** volena s ohledem na maximální kvalitu výsledku). }
*/

int hashCode ( tKey key ) {
	int retval = 1;
	int keylen = strlen(key);
	for ( int i=0; i<keylen; i++ )
		retval += key[i];
	return ( retval % HTSIZE );
}

/*
** Inicializace tabulky s explicitně zřetězenými synonymy.  Tato procedura
** se volá pouze před prvním použitím tabulky.
*/

void htInit ( tHTable* ptrht ) {
	if (ptrht != NULL)	// pokud tabulka neni prazdna
	{
		for (int i = 0; i < HTSIZE; i++)	// inicializuj vsechny polozky na NULL
			(*ptrht)[i] = NULL;
	}
}

/* TRP s explicitně zřetězenými synonymy.
** Vyhledání prvku v TRP ptrht podle zadaného klíče key.  Pokud je
** daný prvek nalezen, vrací se ukazatel na daný prvek. Pokud prvek nalezen není, 
** vrací se hodnota NULL.
**
*/

tHTItem* htSearch ( tHTable* ptrht, tKey key ) {
	if (ptrht != NULL)	// jestli tabulka neni prazdna
	{
		int i = hashCode(key);		// vypocti umisteni polozky s danym klicem
		tHTItem *tmp = (*ptrht)[i];	// uloz adresu prvni polozky synonym
		while (tmp != NULL)		// dokud nedojdeme na konec synonym
		{
			if (!strcmp(tmp->key, key))	// jesltize se nalezla polozka s danym klicem
				return tmp;		// vrat ukazatel na ni
			else
				tmp = tmp->ptrnext;	// jinak postup na dalsi synonymum
		}
	}
	return NULL;	// nenalezena polozka s klicem key, vracime NULL
}

/* 
** TRP s explicitně zřetězenými synonymy.
** Tato procedura vkládá do tabulky ptrht položku s klíčem key a s daty
** data.  Protože jde o vyhledávací tabulku, nemůže být prvek se stejným
** klíčem uložen v tabulce více než jedenkrát.  Pokud se vkládá prvek,
** jehož klíč se již v tabulce nachází, aktualizujte jeho datovou část.
**
** Využijte dříve vytvořenou funkci htSearch.  Při vkládání nového
** prvku do seznamu synonym použijte co nejefektivnější způsob,
** tedy proveďte.vložení prvku na začátek seznamu.
**/

void htInsert ( tHTable* ptrht, tKey key, tData data ) {
	if (ptrht != NULL)	// jestli je tabulka neprazdna
	{
		tHTItem *tmp;			// promenna na uchovani adresy polozky
		tmp = htSearch(ptrht, key);	// vyhledej polozku s klicem key
		if (tmp == NULL)		// jestlize nenalena (zatim neexistuje)
		{
			tHTItem *new;					// vytvor novou polozku
			if ((new = malloc(sizeof(tHTItem))) == NULL)	// naalokuj pametovy prostor a over uspesnost
				return;
			new->data = data;				// do nove polozky ulozime data 
			new->key = key;					// ulozime klic key
			int i = hashCode(key);				// zjistime umisteni v tabulce
			new->ptrnext = (*ptrht)[i];			// ulozime ukazatel na nasledujici polozku
			(*ptrht)[i] = new;				// vlozime novou polozku jako prvni synonymum
		}
		else				// jestlize se polozka nalezla
			tmp->data = data;	// aktualizujeme datovou cast
	}
}

/*
** TRP s explicitně zřetězenými synonymy.
** Tato funkce zjišťuje hodnotu datové části položky zadané klíčem.
** Pokud je položka nalezena, vrací funkce ukazatel na položku
** Pokud položka nalezena nebyla, vrací se funkční hodnota NULL
**
** Využijte dříve vytvořenou funkci HTSearch.
*/

tData* htRead ( tHTable* ptrht, tKey key ) {
	if (ptrht != NULL)			// jestlize tabulka neni prazdna
	{
		tHTItem *tmp;			// pomocna prommena
		tmp = htSearch(ptrht, key);	// vyhledame polozku s klicem key
		if (tmp != NULL)		// pokud byla nalezena
			return &(tmp->data);	// vratime adresu datove casti
	}
	return NULL;				// polozka nenalezena, vracime NULL
}

/*
** TRP s explicitně zřetězenými synonymy.
** Tato procedura vyjme položku s klíčem key z tabulky
** ptrht.  Uvolněnou položku korektně zrušte.  Pokud položka s uvedeným
** klíčem neexistuje, dělejte, jako kdyby se nic nestalo (tj. nedělejte
** nic).
**
** V tomto případě NEVYUŽÍVEJTE dříve vytvořenou funkci HTSearch.
*/

void htDelete ( tHTable* ptrht, tKey key ) {
	if (ptrht != NULL)	// jestlize tabulka neni prazdna
	{
		int i = hashCode(key);
		tHTItem *tmp;			// pomocny ukazatel na soucasnou polozku
		tHTItem *previous = NULL;	// pomocny ukazatel na predchazejici polozku
		tmp = (*ptrht)[i];		// ulozime ukazatel na prvni synonymum
		if (tmp != NULL)		// pokud neni seznam synonym prazdny
		{
			if (!strcmp(tmp->key, key))		// jestlize prvni synonymum obsauje hledany klic key
			{
				(*ptrht)[i] = tmp->ptrnext;	// vyvazeme prvni synonymum ze seznamu
				free(tmp);			// uvolnime polozku
				return;
			}
			while (tmp != NULL)			// dokud nedojdeme na konec seznamu synonym
			{
				previous = tmp;			// aktualizujeme ukazatel na predchozi polozku
				tmp = tmp->ptrnext;		// posuneme se na dalsi
				if (!strcmp(key, tmp->key))	// pokud je dana polozka tou hledanou
					break;
			}
			if (tmp != NULL)	// jestlize byla polozka nalezena
			{
				previous->ptrnext = tmp->ptrnext;	// vyvazeme ji ze seznamu synonym
				free(tmp);				// uvolnime pamet
			}
		}
	}
}

/* TRP s explicitně zřetězenými synonymy.
** Tato procedura zruší všechny položky tabulky, korektně uvolní prostor,
** který tyto položky zabíraly, a uvede tabulku do počátečního stavu.
*/

void htClearAll ( tHTable* ptrht ) {
	if (ptrht != NULL)				// jestlize tabulka neni prazdna
	{
		tHTItem *tmp;				// pomocna promenna
		for (int i = 0; i < HTSIZE; i++)	// pro vsechny polozky tabulky
		{
			tmp = (*ptrht)[i];		// zacneme na prvnim synonymu
			while (tmp != NULL)		// dokud "radek" obsahuje dalsi synonyma
			{
				(*ptrht)[i] = (*ptrht)[i]->ptrnext;	// ulozime ukazatel na dalsi
				free(tmp);				// uvolnime predchazejici
				tmp = (*ptrht)[i];			// posuneme se na dalsi
			}
		}
	}
}
