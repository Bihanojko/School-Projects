	
/* c206.c **********************************************************}
{* Téma: Dvousmìrnì vázaný lineární seznam
**
**                   Návrh a referenèní implementace: Bohuslav Køena, øíjen 2001
**                            Pøepracované do jazyka C: Martin Tuèek, øíjen 2004
**                                            Úpravy: Bohuslav Køena, øíjen 2015
**
** Implementujte abstraktní datový typ dvousmìrnì vázaný lineární seznam.
** U¾iteèným obsahem prvku seznamu je hodnota typu int.
** Seznam bude jako datová abstrakce reprezentován promìnnou
** typu tDLList (DL znamená Double-Linked a slou¾í pro odli¹ení
** jmen konstant, typù a funkcí od jmen u jednosmìrnì vázaného lineárního
** seznamu). Definici konstant a typù naleznete v hlavièkovém souboru c206.h.
**
** Va¹ím úkolem je implementovat následující operace, které spolu
** s vý¹e uvedenou datovou èástí abstrakce tvoøí abstraktní datový typ
** obousmìrnì vázaný lineární seznam:
**
**      DLInitList ...... inicializace seznamu pøed prvním pou¾itím,
**      DLDisposeList ... zru¹ení v¹ech prvkù seznamu,
**      DLInsertFirst ... vlo¾ení prvku na zaèátek seznamu,
**      DLInsertLast .... vlo¾ení prvku na konec seznamu, 
**      DLFirst ......... nastavení aktivity na první prvek,
**      DLLast .......... nastavení aktivity na poslední prvek, 
**      DLCopyFirst ..... vrací hodnotu prvního prvku,
**      DLCopyLast ...... vrací hodnotu posledního prvku, 
**      DLDeleteFirst ... zru¹í první prvek seznamu,
**      DLDeleteLast .... zru¹í poslední prvek seznamu, 
**      DLPostDelete .... ru¹í prvek za aktivním prvkem,
**      DLPreDelete ..... ru¹í prvek pøed aktivním prvkem, 
**      DLPostInsert .... vlo¾í nový prvek za aktivní prvek seznamu,
**      DLPreInsert ..... vlo¾í nový prvek pøed aktivní prvek seznamu,
**      DLCopy .......... vrací hodnotu aktivního prvku,
**      DLActualize ..... pøepí¹e obsah aktivního prvku novou hodnotou,
**      DLSucc .......... posune aktivitu na dal¹í prvek seznamu,
**      DLPred .......... posune aktivitu na pøedchozí prvek seznamu, 
**      DLActive ........ zji¹»uje aktivitu seznamu.
**
** Pøi implementaci jednotlivých funkcí nevolejte ¾ádnou z funkcí
** implementovaných v rámci tohoto pøíkladu, není-li u funkce
** explicitnì uvedeno nìco jiného.
**
** Nemusíte o¹etøovat situaci, kdy místo legálního ukazatele na seznam 
** pøedá nìkdo jako parametr hodnotu NULL.
**
** Svou implementaci vhodnì komentujte!
**
** Terminologická poznámka: Jazyk C nepou¾ívá pojem procedura.
** Proto zde pou¾íváme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako
** procedury (v jazyce C procedurám odpovídají funkce vracející typ void).
**/

#include "c206.h"

int solved;
int errflg;

void DLError() {
/*
** Vytiskne upozornìní na to, ¾e do¹lo k chybì.
** Tato funkce bude volána z nìkterých dále implementovaných operací.
**/	
    printf ("*ERROR* The program has performed an illegal operation.\n");
    errflg = TRUE;             /* globální promìnná -- pøíznak o¹etøení chyby */
    return;
}

void DLInitList (tDLList *L) {
/*
** Provede inicializaci seznamu L pøed jeho prvním pou¾itím (tzn. ¾ádná
** z následujících funkcí nebude volána nad neinicializovaným seznamem).
** Tato inicializace se nikdy nebude provádìt nad ji¾ inicializovaným
** seznamem, a proto tuto mo¾nost neo¹etøujte. V¾dy pøedpokládejte,
** ¾e neinicializované promìnné mají nedefinovanou hodnotu.
**/
    L->First = NULL;		// zadne polozky
    L->Act = NULL;			// seznam neaktivni
    L->Last = NULL;

}

void DLDisposeList (tDLList *L) {
/*
** Zru¹í v¹echny prvky seznamu L a uvede seznam do stavu, v jakém
** se nacházel po inicializaci. Ru¹ené prvky seznamu budou korektnì
** uvolnìny voláním operace free. 
**/
    while (L->First != NULL)		// dokud neni seznam prazdny
    {
    	L->Act = L->First;			// udela prvni prvek aktivnim 
		L->First = L->First->rptr;	// vyvaze ho ze seznamu
		free(L->Act);				// uvolni pamet
    }
    L->Act = NULL;			// seznam jiz nebude aktivni
    L->Last = NULL;			// prepiseme hodnoty ukazatelu, aby nedoslo pozdeji k pristupu

}

void DLInsertFirst (tDLList *L, int val) {
/*
** Vlo¾í nový prvek na zaèátek seznamu L.
** V pøípadì, ¾e není dostatek pamìti pro nový prvek pøi operaci malloc,
** volá funkci DLError().
**/
    tDLElemPtr tmp;		// pomocny ukazatel
    if ((tmp = malloc(sizeof(struct tDLElem))) == NULL)	// naalokujeme pamet pro novy prvek 
		DLError();		// alokace neuspesna, DLError()
    else
    {
		tmp->data = val;	// nainicializujeme data prvku
		tmp->lptr = NULL;
		tmp->rptr = L->First;	// provazeme se seznamem
		if (L->First == NULL)	// pokud je tento prvek prvnim prvkem
		    L->Last = tmp;	// bude i poslednim
		else
		    L->First->lptr = tmp;	// jinak jej provazeme s nasledujicim prvkem
		L->First = tmp;			// zacatek seznamu ukazuje na novy prvek
    }
}


void DLInsertLast(tDLList *L, int val) {
/*
** Vlo¾í nový prvek na konec seznamu L (symetrická operace k DLInsertFirst).
** V pøípadì, ¾e není dostatek pamìti pro nový prvek pøi operaci malloc,
** volá funkci DLError().
**/ 	
    tDLElemPtr tmp;		// pomocny ukazatel
    if ((tmp = malloc(sizeof(struct tDLElem))) == NULL)	// naalokujeme pamet pro novy prvek
		DLError();		// nezdarena alokace -> DLError()
    else
    {
		tmp->data = val;	// nainicializujeme data noveho prvku
		tmp->lptr = L->Last;	// vlozime na konec seznamu
		tmp->rptr = NULL;
		if (L->First == NULL)	// pokud byl seznam predtim prazdny
		    L->First = tmp;	// bude novy prvek zaroven prvnim
		else
		    L->Last->rptr = tmp; // jinak provazeme s predchozim prvkem
		L->Last = tmp;		// konec seznamu bude ukazovat na novy prvek
    }
}


void DLFirst (tDLList *L) {
/*
** Nastaví aktivitu na první prvek seznamu L.
** Funkci implementujte jako jediný pøíkaz (nepoèítáme-li return),
** ani¾ byste testovali, zda je seznam L prázdný.
**/
    L->Act = L->First;

}

void DLLast (tDLList *L) {
/*
** Nastaví aktivitu na poslední prvek seznamu L.
** Funkci implementujte jako jediný pøíkaz (nepoèítáme-li return),
** ani¾ byste testovali, zda je seznam L prázdný.
**/
    L->Act = L->Last;

}

void DLCopyFirst (tDLList *L, int *val) {
/*
** Prostøednictvím parametru val vrátí hodnotu prvního prvku seznamu L.
** Pokud je seznam L prázdný, volá funkci DLError().
**/
    if (L->First == NULL)	// seznam prazdny -> DLError()
		DLError();
    else
		*val = L->First->data;	// ulozime hodnotu do promenne

}

void DLCopyLast (tDLList *L, int *val) {
/*
** Prostøednictvím parametru val vrátí hodnotu posledního prvku seznamu L.
** Pokud je seznam L prázdný, volá funkci DLError().
**/
    if (L->First == NULL)	// seznam prazdny -> DLError()
		DLError();
    else
		*val = L->Last->data;	// ulozi hodnotu do promenne

}

void DLDeleteFirst (tDLList *L) {
/*
** Zru¹í první prvek seznamu L. Pokud byl první prvek aktivní, aktivita 
** se ztrácí. Pokud byl seznam L prázdný, nic se nedìje.
**/
    if (L->First != NULL)		// pokud je seznam neprazdny
    {
		tDLElemPtr tmp;			// pomocny ukazatel
		tmp = L->First;			// ulozime adresu prvniho prvku
		if (L->First == L->Act)		// pokud je prvni prvek aktivni
		    L->Act = NULL;		// aktivita se ztrati
		if (L->First == L->Last) 	// pokud je prvni prvek jediny (zaroven posledni)
		    L->Last = NULL;		// prepiseme adresu, aby nedoslo pozdeji k pristupu
		L->First = L->First->rptr;	// prvnim prvkem se stava prvek nasledujici
		tmp->rptr->lptr = NULL;		// vyvazeme odstranovany prvek ze seznamu
		free(tmp);			// uvolnime pamet
    }
}


void DLDeleteLast (tDLList *L) {
/*
** Zru¹í poslední prvek seznamu L. Pokud byl poslední prvek aktivní,
** aktivita seznamu se ztrácí. Pokud byl seznam L prázdný, nic se nedìje.
**/ 
    if (L->First != NULL)		// seznam neprazdny
    {
		tDLElemPtr tmp;			// pomocny ukazatel
		tmp = L->Last;			// ulozime adresu posledniho prvku
		if (L->Last == L->First)	// pokud byl prvek jediny
		    L->First = NULL;		// prepiseme adresu, aby nedoslo pozdeji k pristupu
		if (L->Last == L->Act)		// pokud byl prvek aktivni
		    L->Act = NULL;		// zrusime aktivitu
		L->Last = L->Last->lptr;	// vyvazeme prvek ze seznamu
		L->Last->rptr = NULL;
		free(tmp);			// uvolnime pamet
    }
}


void DLPostDelete (tDLList *L) {
/*
** Zru¹í prvek seznamu L za aktivním prvkem.
** Pokud je seznam L neaktivní nebo pokud je aktivní prvek
** posledním prvkem seznamu, nic se nedìje.
**/
    if ((L->Act != NULL) && (L->Act != L->Last))	// seznam aktivni a aktivni prvek neni posledni
    {
		tDLElemPtr tmp;			// pomocny ukazatel 
		tmp = L->Act->rptr;		// ulozime adresu ruseneho prvku
		L->Act->rptr = tmp->rptr;	// vyvazeme z predchoziho prvku
		if (tmp == L->Last)		// pokud je ruseny prvek posledni
		    L->Last = L->Act;		// novym poslednim bude aktivni prvek
		else
		    tmp->rptr->lptr = L->Act;	// jinak vyvazeme z nasledujiciho prvku
		free(tmp);			// uvolnime pamet
    }
}


void DLPreDelete (tDLList *L) {
/*
** Zru¹í prvek pøed aktivním prvkem seznamu L .
** Pokud je seznam L neaktivní nebo pokud je aktivní prvek
** prvním prvkem seznamu, nic se nedìje.
**/
    if ((L->Act != NULL) && (L->Act != L->First))	// seznam je aktivni a aktivni neni prvni prvek
    {
		tDLElemPtr tmp;			// pomocny ukazatel
		tmp = L->Act->lptr;		// ulozime adresu ruseneho prvku
		L->Act->lptr = tmp->lptr;	// vyvazeme z nasledujiciho prvku
		if (tmp == L->First)		// pokud je ruseny prvek prvni
		    L->First = L->Act;		// nove bude prvni nasledujici prvek
		else
		    tmp->lptr->rptr = L->Act;	// jinak vyvazeme z predchoziho prvku
		free(tmp);			// uvolnime pamet
    }
}


void DLPostInsert (tDLList *L, int val) {
/*
** Vlo¾í prvek za aktivní prvek seznamu L.
** Pokud nebyl seznam L aktivní, nic se nedìje.
** V pøípadì, ¾e není dostatek pamìti pro nový prvek pøi operaci malloc,
** volá funkci DLError().
**/
    if (L->Act != NULL)		// seznam aktivni
    {
		tDLElemPtr tmp;		// pomocna promenna
		if ((tmp = malloc(sizeof(struct tDLElem))) == NULL)	// alokace pameti pro novy prvek
		    DLError();		// selhala -> DLError()
		else
		{
		    tmp->data = val;		// inicializace dat noveho prvku
		    tmp->lptr = L->Act;		// provazani se seznamem
		    tmp->rptr = L->Act->rptr;
		    L->Act->rptr = tmp;
		    if (L->Act == L->Last)	// jestlize je aktivni posledni prvek
			L->Last = tmp;		// nove bude posledni nove vytvoreny prvek
		    else
			tmp->rptr->lptr = tmp;	// jinak provazeme s nasledujici polozkou
		}
    }
}


void DLPreInsert (tDLList *L, int val) {
/*
** Vlo¾í prvek pøed aktivní prvek seznamu L.
** Pokud nebyl seznam L aktivní, nic se nedìje.
** V pøípadì, ¾e není dostatek pamìti pro nový prvek pøi operaci malloc,
** volá funkci DLError().
**/
    if (L->Act != NULL)		// seznam aktivni
    {
		tDLElemPtr tmp;		// pomocna promenna
		if ((tmp = malloc(sizeof(struct tDLElem))) == NULL)	// alokace pameti pro novy prvek
		    DLError();		// selhala -> DLError()
		else
		{
		    tmp->data = val;		// inicializace dat noveho prvku
		    tmp->lptr = L->Act->lptr;	// provazani se seznamem
		    tmp->rptr = L->Act;
		    L->Act->lptr = tmp;
		    if (L->Act == L->First)	// jestlize je aktivni prvni prvek
			L->First = tmp;		// nove bude prvni nove vytvoreny prvek
		    else
			tmp->lptr->rptr = tmp;	// jinak provazeme s predchazejici polozkou
		}
    }
}


void DLCopy (tDLList *L, int *val) {
/*
** Prostøednictvím parametru val vrátí hodnotu aktivního prvku seznamu L.
** Pokud seznam L není aktivní, volá funkci DLError ().
**/
    if (L->Act == NULL)		// seznam neni aktivni -> DLError()
		DLError();
    else
		*val = L->Act->data;	// ulozi hodnotu do promenne
}


void DLActualize (tDLList *L, int val) {
/*
** Pøepí¹e obsah aktivního prvku seznamu L.
** Pokud seznam L není aktivní, nedìlá nic.
**/
    if (L->Act != NULL)		// pokud seznam je aktivni
		L->Act->data = val;	// prepise obsah
}


void DLSucc (tDLList *L) {
/*
** Posune aktivitu na následující prvek seznamu L.
** Není-li seznam aktivní, nedìlá nic.
** V¹imnìte si, ¾e pøi aktivitì na posledním prvku se seznam stane neaktivním.
**/
    if (L->Act != NULL)		// pokud seznam je aktivni
		L->Act = L->Act->rptr;	// posune aktivitu

}


void DLPred (tDLList *L) {
/*
** Posune aktivitu na pøedchozí prvek seznamu L.
** Není-li seznam aktivní, nedìlá nic.
** V¹imnìte si, ¾e pøi aktivitì na prvním prvku se seznam stane neaktivním.
**/
    if (L->Act != NULL)		// pokud seznam je aktivni
		L->Act = L->Act->lptr;	// posune aktivitu

}

int DLActive (tDLList *L) {
/*
** Je-li seznam L aktivní, vrací nenulovou hodnotu, jinak vrací 0.
** Funkci je vhodné implementovat jedním pøíkazem return.
**/
    return (L->Act != NULL ? 1 : 0);
}

/* Konec c206.c*/
