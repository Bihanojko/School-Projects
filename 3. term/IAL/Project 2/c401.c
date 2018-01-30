
/* c401.c: **********************************************************}
{* Téma: Rekurzivní implementace operací nad BVS
**                                         Vytvořil: Petr Přikryl, listopad 1994
**                                         Úpravy: Andrea Němcová, prosinec 1995
**                                                      Petr Přikryl, duben 1996
**                                                   Petr Přikryl, listopad 1997
**                                  Převod do jazyka C: Martin Tuček, říjen 2005
**                                         Úpravy: Bohuslav Křena, listopad 2009
**                                         Úpravy: Karel Masařík, říjen 2013
**                                         Úpravy: Radek Hranický, říjen 2014
**                                         Úpravy: Radek Hranický, listopad 2015
**
** Implementujte rekurzivním způsobem operace nad binárním vyhledávacím
** stromem (BVS; v angličtině BST - Binary Search Tree).
**
** Klíčem uzlu stromu je jeden znak (obecně jím může být cokoliv, podle
** čeho se vyhledává). Užitečným (vyhledávaným) obsahem je zde integer.
** Uzly s menším klíčem leží vlevo, uzly s větším klíčem leží ve stromu
** vpravo. Využijte dynamického přidělování paměti.
** Rekurzivním způsobem implementujte následující funkce:
**
**   BSTInit ...... inicializace vyhledávacího stromu
**   BSTSearch .... vyhledávání hodnoty uzlu zadaného klíčem
**   BSTInsert .... vkládání nové hodnoty
**   BSTDelete .... zrušení uzlu se zadaným klíčem
**   BSTHeight .... výpočet výšky stromu
**   BSTDispose ... zrušení celého stromu
**
** ADT BVS je reprezentován kořenovým ukazatelem stromu (typ tBSTNodePtr).
** Uzel stromu (struktura typu tBSTNode) obsahuje klíč (typu char), podle
** kterého se ve stromu vyhledává, vlastní obsah uzlu (pro jednoduchost
** typu int) a ukazatel na levý a pravý podstrom (LPtr a RPtr). Přesnou definici typů 
** naleznete v souboru c401.h.
**
** Pozor! Je třeba správně rozlišovat, kdy použít dereferenční operátor *
** (typicky při modifikaci) a kdy budeme pracovat pouze se samotným ukazatelem 
** (např. při vyhledávání). V tomto příkladu vám napoví prototypy funkcí.
** Pokud pracujeme s ukazatelem na ukazatel, použijeme dereferenci.
**/

#include "c401.h"
int solved;

void BSTInit (tBSTNodePtr *RootPtr) {
/*   -------
** Funkce provede počáteční inicializaci stromu před jeho prvním použitím.
**
** Ověřit, zda byl již strom předaný přes RootPtr inicializován, nelze,
** protože před první inicializací má ukazatel nedefinovanou (tedy libovolnou)
** hodnotu. Programátor využívající ADT BVS tedy musí zajistit, aby inicializace
** byla volána pouze jednou, a to před vlastní prací s BVS. Provedení
** inicializace nad neprázdným stromem by totiž mohlo vést ke ztrátě přístupu
** k dynamicky alokované paměti (tzv. "memory leak").
**	
** Všimněte si, že se v hlavičce objevuje typ ukazatel na ukazatel.	
** Proto je třeba při přiřazení přes RootPtr použít dereferenční operátor *.
** Ten bude použit i ve funkcích BSTDelete, BSTInsert a BSTDispose.
**/
	*RootPtr = NULL;	
}	

int BSTSearch (tBSTNodePtr RootPtr, char K, int *Content)	{
/*  ---------
** Funkce vyhledá uzel v BVS s klíčem K.
**
** Pokud je takový nalezen, vrací funkce hodnotu TRUE a v proměnné Content se
** vrací obsah příslušného uzlu.´Pokud příslušný uzel není nalezen, vrací funkce
** hodnotu FALSE a obsah proměnné Content není definován (nic do ní proto
** nepřiřazujte).
**
** Při vyhledávání v binárním stromu bychom typicky použili cyklus ukončený
** testem dosažení listu nebo nalezení uzlu s klíčem K. V tomto případě ale
** problém řešte rekurzivním volání této funkce, přičemž nedeklarujte žádnou
** pomocnou funkci.
**/
	if (RootPtr != NULL)	// jestlize strom neni prazdny
	{
		if (RootPtr->Key == K)		// jestlize aktualni uzel obsahuje klic K
		{
			*Content = RootPtr->BSTNodeCont;	// ulozime obsah tohoto uzlu
			return TRUE;
		}
		else
			if (RootPtr->Key < K)	// jestlize aktualni uzel obsahuje mensi hodnotu
			{
				if (BSTSearch(RootPtr->RPtr, K, Content))	// pokracujeme doprava
					return TRUE;				// vratime TRUE, kdyz nalezne
			}
			else
			{
				if (RootPtr->Key > K)	// pokracujeme doleva
				{
					if (BSTSearch(RootPtr->LPtr, K, Content))	// vratime TRUE, kdyz nalezne
						return TRUE;
				}
			}	
	}
	return FALSE;
} 


void BSTInsert (tBSTNodePtr* RootPtr, char K, int Content)	{	
/*   ---------
** Vloží do stromu RootPtr hodnotu Content s klíčem K.
**
** Pokud již uzel se zadaným klíčem ve stromu existuje, bude obsah uzlu
** s klíčem K nahrazen novou hodnotou. Pokud bude do stromu vložen nový
** uzel, bude vložen vždy jako list stromu.
**
** Funkci implementujte rekurzivně. Nedeklarujte žádnou pomocnou funkci.
**
** Rekurzivní implementace je méně efektivní, protože se při každém
** rekurzivním zanoření ukládá na zásobník obsah uzlu (zde integer).
** Nerekurzivní varianta by v tomto případě byla efektivnější jak z hlediska
** rychlosti, tak z hlediska paměťových nároků. Zde jde ale o školní
** příklad, na kterém si chceme ukázat eleganci rekurzivního zápisu.
**/
	if (*RootPtr != NULL)	// jestlize je (pod)strom neprazdny
	{
		if ((*RootPtr)->Key == K)	// nalezena polozka s klicem K
		{
			(*RootPtr)->BSTNodeCont = Content;	// data aktualizujeme
			return;
		}
		else if ((*RootPtr)->Key < K)	// jestlize je hodnota uzlu mensi nez hledana
		{
			BSTInsert(&(*RootPtr)->RPtr, K, Content);	// pokracujeme doprava
			return;
		}
		else if ((*RootPtr)->Key > K)	// jestlize je hodnota uzlu vetsi nez hledana
		{
			BSTInsert(&(*RootPtr)->LPtr, K, Content);	// pokracujeme doleva
			return;
		}
	}
	else	// uzel s klicem K neexistuje
	{
		tBSTNodePtr tmp;	// pomocna promenna
		if ((tmp = malloc(sizeof(struct tBSTNode))) == NULL)	// naalokujeme pamet a overime uspesnost alokace
			return;
		tmp->BSTNodeCont = Content;	// ulozime data
		tmp->Key = K;			// ulozime klic
		tmp->LPtr = NULL;		// ukazatele na dalsi uzly inicializujeme na NULL
		tmp->RPtr = NULL;
		*RootPtr = tmp;			// navazeme do stromu
	}
}

void ReplaceByRightmost (tBSTNodePtr PtrReplaced, tBSTNodePtr *RootPtr) {
/*   ------------------
** Pomocná funkce pro vyhledání, přesun a uvolnění nejpravějšího uzlu.
**
** Ukazatel PtrReplaced ukazuje na uzel, do kterého bude přesunuta hodnota
** nejpravějšího uzlu v podstromu, který je určen ukazatelem RootPtr.
** Předpokládá se, že hodnota ukazatele RootPtr nebude NULL (zajistěte to
** testováním před volání této funkce). Tuto funkci implementujte rekurzivně. 
**
** Tato pomocná funkce bude použita dále. Než ji začnete implementovat,
** přečtěte si komentář k funkci BSTDelete(). 
**/
	if (PtrReplaced != NULL)	// je kam presouvat
	{
		if ((*RootPtr)->RPtr != NULL)	// aktualni uzel neni nejpravejsi
			ReplaceByRightmost(PtrReplaced, &((*RootPtr)->RPtr));	// posuneme se doprava
		else	// jsme v nejpravejsim uzlu
		{
			tBSTNodePtr tmp;	// pomocna promenna 
			tmp = *RootPtr;		// ulozime ukazatel na nejpravejsi uzel
			PtrReplaced->BSTNodeCont = tmp->BSTNodeCont;	// presuneme hodnotu obsahu uzlu
			PtrReplaced->Key = tmp->Key;	// presuneme hodnotu klice
			*RootPtr = tmp->LPtr;	// levy podstrom presuneme k novemu uzlu
			free(tmp);		// uvolnime uzel
		}
	}
}

void BSTDelete (tBSTNodePtr *RootPtr, char K) 		{
/*   ---------
** Zruší uzel stromu, který obsahuje klíč K.
**
** Pokud uzel se zadaným klíčem neexistuje, nedělá funkce nic. 
** Pokud má rušený uzel jen jeden podstrom, pak jej zdědí otec rušeného uzlu.
** Pokud má rušený uzel oba podstromy, pak je rušený uzel nahrazen nejpravějším
** uzlem levého podstromu. Pozor! Nejpravější uzel nemusí být listem.
**
** Tuto funkci implementujte rekurzivně s využitím dříve deklarované
** pomocné funkce ReplaceByRightmost.
**/
	if (*RootPtr != NULL)	// (pod)strom neni prazdny
	{
		if ((*RootPtr)->Key == K)	// pokud jsme nalezli uzel s klicem K
		{
			if (((*RootPtr)->RPtr != NULL) && ((*RootPtr)->LPtr != NULL))	// ruseny uzel ma oba podstromy
				ReplaceByRightmost(*RootPtr, &((*RootPtr)->LPtr));	// ruseny uzel nahradime nejravejsim
			else
			{
				tBSTNodePtr tmp;
				tmp = (*RootPtr);
				if ((*RootPtr)->RPtr == NULL)		// ruseny uzel ma pouze levy podstrom
				{
					*RootPtr = tmp->LPtr;		// priradime ho otci
					free(tmp);			// uvolnime pamet
				}
				else if ((*RootPtr)->LPtr == NULL)	// ruseny uzel ma pouze pravy podstrom
				{
					*RootPtr = tmp->RPtr;		// priradme ho otci
					free(tmp);			// uvolnime pamet
				}
			}
		}	
		else if ((*RootPtr)->Key < K)	// aktualni uzel obsahuje mensi klic
			BSTDelete(&((*RootPtr)->RPtr), K);	// pokracujeme doprava
		else if ((*RootPtr)->Key > K)	// aktualni uzel obsahuje vetsi klic
			BSTDelete(&((*RootPtr)->LPtr), K);	// pokracujeme doleva
	}
} 

int BSTHeight (tBSTNodePtr NodePtr, bool IsRoot) {	
/*   ----------
** Vypočítá výšku BVS. Výška stromu délka (počet hran) nejdelší cesty
** od kořene k listu. Vzhledem k rekurzivní implementaci je třeba rozlišit,
** zda funkci voláme pro samotný kořen stromu nebo rekurzivně pro některý
** z ostatních uzlů.
** Výpočet výšky stromu se tedy bude provádět voláním:
**   BSTHeight(ukazatel_na_kořen, TRUE)
**
** Návratová hodnota je výška stromu. Výška prázdného stromu však není definována.
** V případě prázdného stromu bude funkce vracet hodnotu -1. 
** 
** Tuto funkci implementujte bez deklarování pomocné funkce.
**/
	if ((NodePtr == NULL) && (IsRoot == TRUE))	// vyska prazdneho stromu je -1
		return -1;

	if (NodePtr == NULL)	// prazdny podstrom
		return 0;

	int left_height = BSTHeight(NodePtr->LPtr, FALSE);	// vyska leveho podstromu
	int right_height = BSTHeight(NodePtr->RPtr, FALSE);	// vyska praveho podstromu

	if (left_height > right_height)		// vrati vesti ze dvou hodnot zvetsenou o jedna/nula

		return (left_height + !(IsRoot));
	else
		return (right_height + !(IsRoot));
}

void BSTDispose (tBSTNodePtr *RootPtr) {	
/*   ----------
** Zruší celý binární vyhledávací strom a korektně uvolní paměť.
**
** Po zrušení se bude BVS nacházet ve stejném stavu, jako se nacházel po
** inicializaci. Tuto funkci implementujte rekurzivně bez deklarování pomocné
** funkce.
**/
	if (*RootPtr != NULL)	// jestlize strom neni prazdny
	{
		BSTDispose(&((*RootPtr)->LPtr));	// zrus levy podstrom
		BSTDispose(&((*RootPtr)->RPtr));	// zrus pravy podstrom
		free(*RootPtr);				// uvolni aktualni uzel
		*RootPtr = NULL;			// zahod ukazatel
	}
}

/* konec c401.c */

