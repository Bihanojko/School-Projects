	
/* c206.c **********************************************************}
{* T�ma: Dvousm�rn� v�zan� line�rn� seznam
**
**                   N�vrh a referen�n� implementace: Bohuslav K�ena, ��jen 2001
**                            P�epracovan� do jazyka C: Martin Tu�ek, ��jen 2004
**                                            �pravy: Bohuslav K�ena, ��jen 2015
**
** Implementujte abstraktn� datov� typ dvousm�rn� v�zan� line�rn� seznam.
** U�ite�n�m obsahem prvku seznamu je hodnota typu int.
** Seznam bude jako datov� abstrakce reprezentov�n prom�nnou
** typu tDLList (DL znamen� Double-Linked a slou�� pro odli�en�
** jmen konstant, typ� a funkc� od jmen u jednosm�rn� v�zan�ho line�rn�ho
** seznamu). Definici konstant a typ� naleznete v hlavi�kov�m souboru c206.h.
**
** Va��m �kolem je implementovat n�sleduj�c� operace, kter� spolu
** s v��e uvedenou datovou ��st� abstrakce tvo�� abstraktn� datov� typ
** obousm�rn� v�zan� line�rn� seznam:
**
**      DLInitList ...... inicializace seznamu p�ed prvn�m pou�it�m,
**      DLDisposeList ... zru�en� v�ech prvk� seznamu,
**      DLInsertFirst ... vlo�en� prvku na za��tek seznamu,
**      DLInsertLast .... vlo�en� prvku na konec seznamu, 
**      DLFirst ......... nastaven� aktivity na prvn� prvek,
**      DLLast .......... nastaven� aktivity na posledn� prvek, 
**      DLCopyFirst ..... vrac� hodnotu prvn�ho prvku,
**      DLCopyLast ...... vrac� hodnotu posledn�ho prvku, 
**      DLDeleteFirst ... zru�� prvn� prvek seznamu,
**      DLDeleteLast .... zru�� posledn� prvek seznamu, 
**      DLPostDelete .... ru�� prvek za aktivn�m prvkem,
**      DLPreDelete ..... ru�� prvek p�ed aktivn�m prvkem, 
**      DLPostInsert .... vlo�� nov� prvek za aktivn� prvek seznamu,
**      DLPreInsert ..... vlo�� nov� prvek p�ed aktivn� prvek seznamu,
**      DLCopy .......... vrac� hodnotu aktivn�ho prvku,
**      DLActualize ..... p�ep�e obsah aktivn�ho prvku novou hodnotou,
**      DLSucc .......... posune aktivitu na dal�� prvek seznamu,
**      DLPred .......... posune aktivitu na p�edchoz� prvek seznamu, 
**      DLActive ........ zji��uje aktivitu seznamu.
**
** P�i implementaci jednotliv�ch funkc� nevolejte ��dnou z funkc�
** implementovan�ch v r�mci tohoto p��kladu, nen�-li u funkce
** explicitn� uvedeno n�co jin�ho.
**
** Nemus�te o�et�ovat situaci, kdy m�sto leg�ln�ho ukazatele na seznam 
** p�ed� n�kdo jako parametr hodnotu NULL.
**
** Svou implementaci vhodn� komentujte!
**
** Terminologick� pozn�mka: Jazyk C nepou��v� pojem procedura.
** Proto zde pou��v�me pojem funkce i pro operace, kter� by byly
** v algoritmick�m jazyce Pascalovsk�ho typu implemenov�ny jako
** procedury (v jazyce C procedur�m odpov�daj� funkce vracej�c� typ void).
**/

#include "c206.h"

int solved;
int errflg;

void DLError() {
/*
** Vytiskne upozorn�n� na to, �e do�lo k chyb�.
** Tato funkce bude vol�na z n�kter�ch d�le implementovan�ch operac�.
**/	
    printf ("*ERROR* The program has performed an illegal operation.\n");
    errflg = TRUE;             /* glob�ln� prom�nn� -- p��znak o�et�en� chyby */
    return;
}

void DLInitList (tDLList *L) {
/*
** Provede inicializaci seznamu L p�ed jeho prvn�m pou�it�m (tzn. ��dn�
** z n�sleduj�c�ch funkc� nebude vol�na nad neinicializovan�m seznamem).
** Tato inicializace se nikdy nebude prov�d�t nad ji� inicializovan�m
** seznamem, a proto tuto mo�nost neo�et�ujte. V�dy p�edpokl�dejte,
** �e neinicializovan� prom�nn� maj� nedefinovanou hodnotu.
**/
    L->First = NULL;		// zadne polozky
    L->Act = NULL;			// seznam neaktivni
    L->Last = NULL;

}

void DLDisposeList (tDLList *L) {
/*
** Zru�� v�echny prvky seznamu L a uvede seznam do stavu, v jak�m
** se nach�zel po inicializaci. Ru�en� prvky seznamu budou korektn�
** uvoln�ny vol�n�m operace free. 
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
** Vlo�� nov� prvek na za��tek seznamu L.
** V p��pad�, �e nen� dostatek pam�ti pro nov� prvek p�i operaci malloc,
** vol� funkci DLError().
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
** Vlo�� nov� prvek na konec seznamu L (symetrick� operace k DLInsertFirst).
** V p��pad�, �e nen� dostatek pam�ti pro nov� prvek p�i operaci malloc,
** vol� funkci DLError().
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
** Nastav� aktivitu na prvn� prvek seznamu L.
** Funkci implementujte jako jedin� p��kaz (nepo��t�me-li return),
** ani� byste testovali, zda je seznam L pr�zdn�.
**/
    L->Act = L->First;

}

void DLLast (tDLList *L) {
/*
** Nastav� aktivitu na posledn� prvek seznamu L.
** Funkci implementujte jako jedin� p��kaz (nepo��t�me-li return),
** ani� byste testovali, zda je seznam L pr�zdn�.
**/
    L->Act = L->Last;

}

void DLCopyFirst (tDLList *L, int *val) {
/*
** Prost�ednictv�m parametru val vr�t� hodnotu prvn�ho prvku seznamu L.
** Pokud je seznam L pr�zdn�, vol� funkci DLError().
**/
    if (L->First == NULL)	// seznam prazdny -> DLError()
		DLError();
    else
		*val = L->First->data;	// ulozime hodnotu do promenne

}

void DLCopyLast (tDLList *L, int *val) {
/*
** Prost�ednictv�m parametru val vr�t� hodnotu posledn�ho prvku seznamu L.
** Pokud je seznam L pr�zdn�, vol� funkci DLError().
**/
    if (L->First == NULL)	// seznam prazdny -> DLError()
		DLError();
    else
		*val = L->Last->data;	// ulozi hodnotu do promenne

}

void DLDeleteFirst (tDLList *L) {
/*
** Zru�� prvn� prvek seznamu L. Pokud byl prvn� prvek aktivn�, aktivita 
** se ztr�c�. Pokud byl seznam L pr�zdn�, nic se ned�je.
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
** Zru�� posledn� prvek seznamu L. Pokud byl posledn� prvek aktivn�,
** aktivita seznamu se ztr�c�. Pokud byl seznam L pr�zdn�, nic se ned�je.
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
** Zru�� prvek seznamu L za aktivn�m prvkem.
** Pokud je seznam L neaktivn� nebo pokud je aktivn� prvek
** posledn�m prvkem seznamu, nic se ned�je.
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
** Zru�� prvek p�ed aktivn�m prvkem seznamu L .
** Pokud je seznam L neaktivn� nebo pokud je aktivn� prvek
** prvn�m prvkem seznamu, nic se ned�je.
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
** Vlo�� prvek za aktivn� prvek seznamu L.
** Pokud nebyl seznam L aktivn�, nic se ned�je.
** V p��pad�, �e nen� dostatek pam�ti pro nov� prvek p�i operaci malloc,
** vol� funkci DLError().
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
** Vlo�� prvek p�ed aktivn� prvek seznamu L.
** Pokud nebyl seznam L aktivn�, nic se ned�je.
** V p��pad�, �e nen� dostatek pam�ti pro nov� prvek p�i operaci malloc,
** vol� funkci DLError().
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
** Prost�ednictv�m parametru val vr�t� hodnotu aktivn�ho prvku seznamu L.
** Pokud seznam L nen� aktivn�, vol� funkci DLError ().
**/
    if (L->Act == NULL)		// seznam neni aktivni -> DLError()
		DLError();
    else
		*val = L->Act->data;	// ulozi hodnotu do promenne
}


void DLActualize (tDLList *L, int val) {
/*
** P�ep�e obsah aktivn�ho prvku seznamu L.
** Pokud seznam L nen� aktivn�, ned�l� nic.
**/
    if (L->Act != NULL)		// pokud seznam je aktivni
		L->Act->data = val;	// prepise obsah
}


void DLSucc (tDLList *L) {
/*
** Posune aktivitu na n�sleduj�c� prvek seznamu L.
** Nen�-li seznam aktivn�, ned�l� nic.
** V�imn�te si, �e p�i aktivit� na posledn�m prvku se seznam stane neaktivn�m.
**/
    if (L->Act != NULL)		// pokud seznam je aktivni
		L->Act = L->Act->rptr;	// posune aktivitu

}


void DLPred (tDLList *L) {
/*
** Posune aktivitu na p�edchoz� prvek seznamu L.
** Nen�-li seznam aktivn�, ned�l� nic.
** V�imn�te si, �e p�i aktivit� na prvn�m prvku se seznam stane neaktivn�m.
**/
    if (L->Act != NULL)		// pokud seznam je aktivni
		L->Act = L->Act->lptr;	// posune aktivitu

}

int DLActive (tDLList *L) {
/*
** Je-li seznam L aktivn�, vrac� nenulovou hodnotu, jinak vrac� 0.
** Funkci je vhodn� implementovat jedn�m p��kazem return.
**/
    return (L->Act != NULL ? 1 : 0);
}

/* Konec c206.c*/
