
/* c402.c: ********************************************************************}
{* Téma: Nerekurzivní implementace operací nad BVS 
**                                     Implementace: Petr Přikryl, prosinec 1994
**                                           Úpravy: Petr Přikryl, listopad 1997
**                                                     Petr Přikryl, květen 1998
**			  	                        Převod do jazyka C: Martin Tuček, srpen 2005
**                                         Úpravy: Bohuslav Křena, listopad 2009
**                                         Úpravy: Karel Masařík, říjen 2013
**                                         Úpravy: Radek Hranický, říjen 2014
**                                         Úpravy: Radek Hranický, listopad 2015
**
** S využitím dynamického přidělování paměti, implementujte NEREKURZIVNĚ
** následující operace nad binárním vyhledávacím stromem (předpona BT znamená
** Binary Tree a je u identifikátorů uvedena kvůli možné kolizi s ostatními
** příklady):
**
**     BTInit .......... inicializace stromu
**     BTInsert ........ nerekurzivní vložení nového uzlu do stromu
**     BTPreorder ...... nerekurzivní průchod typu pre-order
**     BTInorder ....... nerekurzivní průchod typu in-order
**     BTPostorder ..... nerekurzivní průchod typu post-order
**     BTHeight ........ výpočet výšky stromu
**     BTDisposeTree ... zruš všechny uzly stromu
**
** U všech funkcí, které využívají některý z průchodů stromem, implementujte
** pomocnou funkci pro nalezení nejlevějšího uzlu v podstromu. Tyto pomocné
** funkce jsou:
**
**     Leftmost_Preorder
**     Leftmost_Inorder
**     Leftmost_Postorder
**
** Ve vaší implementaci si můžete definovat pomocné zásobníky pro uložení
** ukazetelů na uzly stromu (tStackP)
** nebo pro uložení booleovských hodnot TRUE/FALSE (tStackB).
** Pro práci s pomocnými zásobníky můžete použít následující funkce:
**
**     SInitP / SInitB ....... inicializace zásobníku
**     SPushP / SPushB ....... vložení prvku na vrchol zásobníku
**     SPopP / SPopB ......... odstranění prvku z vrcholu zásobníku
**     STopP / STopB ......... získání hodonty prvku na vrcholu zásobníku
**     STopPopP / STopPopB ... kombinace předchozích dvou funkcí 
**     SSizeP / SSizeB ....... zjištění počtu prvků v zásobníku
**     SEmptyP / SEmptyB ..... zjištění, zda je zásobník prázdný 
** 
** Pomocné funkce pro práci ze zásobníky je zakázáno upravovat!
** 
** Přesné definice typů naleznete v souboru c402.h. Uzel stromu je typu tBTNode,
** ukazatel na něj je typu tBTNodePtr. Jeden uzel obsahuje položku int Cont,
** která současně slouží jako užitečný obsah i jako vyhledávací klíč 
** a ukazatele na levý a pravý podstrom (LPtr a RPtr).
**
** Příklad slouží zejména k procvičení nerekurzivních zápisů algoritmů
** nad stromy. Než začnete tento příklad řešit, prostudujte si důkladně
** principy převodu rekurzivních algoritmů na nerekurzivní. Programování
** je především inženýrská disciplína, kde opětné objevování Ameriky nemá
** místo. Pokud se Vám zdá, že by něco šlo zapsat optimálněji, promyslete
** si všechny detaily Vašeho řešení. Povšimněte si typického umístění akcí
** pro různé typy průchodů. Zamyslete se nad modifikací řešených algoritmů
** například pro výpočet počtu uzlů stromu, počtu listů stromunebo pro
** vytvoření zrcadlového obrazu stromu (pouze popřehazování ukazatelů
** bez vytváření nových uzlů a rušení starých).
**
** Při průchodech stromem použijte ke zpracování uzlu funkci BTWorkOut().
** Pro zjednodušení práce máte předem připraveny zásobníky pro hodnoty typu
** bool a tBTNodePtr. Pomocnou funkci BTWorkOut ani funkce pro práci
** s pomocnými zásobníky neupravujte 
** Pozor! Je třeba správně rozlišovat, kdy použít dereferenční operátor *
** (typicky při modifikaci) a kdy budeme pracovat pouze se samotným ukazatelem 
** (např. při vyhledávání). V tomto příkladu vám napoví prototypy funkcí.
** Pokud pracujeme s ukazatelem na ukazatel, použijeme dereferenci.
**/

#include "c402.h"
int solved;

void BTWorkOut (tBTNodePtr Ptr)		{
/*   ---------
** Pomocná funkce, kterou budete volat při průchodech stromem pro zpracování
** uzlu určeného ukazatelem Ptr. Tuto funkci neupravujte.
**/
			
	if (Ptr==NULL) 
    printf("Chyba: Funkce BTWorkOut byla volána s NULL argumentem!\n");
  else 
    printf("Výpis hodnoty daného uzlu> %d\n",Ptr->Cont);
}
	
/* -------------------------------------------------------------------------- */
/*
** Funkce pro zásobník hotnot typu tBTNodePtr. Tyto funkce neupravujte.
**/

void SInitP (tStackP *S)  
/*   ------
** Inicializace zásobníku.
**/
{
	S->top = 0;  
}	

void SPushP (tStackP *S, tBTNodePtr ptr)
/*   ------
** Vloží hodnotu na vrchol zásobníku.
**/
{ 
                 /* Při implementaci v poli může dojít k přetečení zásobníku. */
  if (S->top==MAXSTACK) 
    printf("Chyba: Došlo k přetečení zásobníku s ukazateli!\n");
  else {  
		S->top++;  
		S->a[S->top]=ptr;
	}
}	

tBTNodePtr STopPopP (tStackP *S)
/*         --------
** Odstraní prvek z vrcholu zásobníku a současně vrátí jeho hodnotu.
**/
{
                            /* Operace nad prázdným zásobníkem způsobí chybu. */
	if (S->top==0)  {
		printf("Chyba: Došlo k podtečení zásobníku s ukazateli!\n");
		return(NULL);	
	}	
	else {
		return (S->a[S->top--]);
	}	
}

tBTNodePtr STopP (tStackP *S)
/*         --------
** Vrátí hodnotu prvku na vrcholu zásobníku
**/
{
                            /* Operace nad prázdným zásobníkem způsobí chybu. */
	if (S->top==0)  {
		printf("Chyba: Došlo k podtečení zásobníku s ukazateli!\n");
		return(NULL);	
	}	
	else {
		return (S->a[S->top]);
	}	
}

void SPopP (tStackP *S)
/*         --------
** Odstraní prvek z vrcholu zásobníku
**/
{
                            /* Operace nad prázdným zásobníkem způsobí chybu. */
	if (S->top==0)  {
		printf("Chyba: Došlo k podtečení zásobníku s ukazateli!\n");
	}	
	else {
		S->top--;
	}	
}

int SSizeP (tStackP *S) {
/*   -------
** Vrátí počet prvků v zásobníku
**/
  return(S->top);
}

bool SEmptyP (tStackP *S)
/*   -------
** Je-li zásobník prázdný, vrátí hodnotu true.
**/
{
  return(S->top==0);
}	

/* -------------------------------------------------------------------------- */
/*
** Funkce pro zásobník hotnot typu bool. Tyto funkce neupravujte.
*/

void SInitB (tStackB *S) {
/*   ------
** Inicializace zásobníku.
**/

	S->top = 0;  
}	

void SPushB (tStackB *S,bool val) {
/*   ------
** Vloží hodnotu na vrchol zásobníku.
**/
                 /* Při implementaci v poli může dojít k přetečení zásobníku. */
	if (S->top==MAXSTACK) 
		printf("Chyba: Došlo k přetečení zásobníku pro boolean!\n");
	else {
		S->top++;  
		S->a[S->top]=val;
	}	
}

bool STopPopB (tStackB *S) {
/*   --------
** Odstraní prvek z vrcholu zásobníku a současně vrátí jeho hodnotu.
**/
                            /* Operace nad prázdným zásobníkem způsobí chybu. */
	if (S->top==0) {
		printf("Chyba: Došlo k podtečení zásobníku pro boolean!\n");
		return(NULL);	
	}	
	else {  
		return(S->a[S->top--]); 
	}	
}

bool STopB (tStackB *S)
/*         --------
** Vrátí hodnotu prvku na vrcholu zásobníku
**/
{
                            /* Operace nad prázdným zásobníkem způsobí chybu. */
	if (S->top==0)  {
		printf("Chyba: Došlo k podtečení zásobníku s ukazateli!\n");
		return(NULL);	
	}	
	else {
		return (S->a[S->top]);
	}	
}

void SPopB (tStackB *S)
/*         --------
** Odstraní prvek z vrcholu zásobníku
**/
{
                            /* Operace nad prázdným zásobníkem způsobí chybu. */
	if (S->top==0)  {
		printf("Chyba: Došlo k podtečení zásobníku s ukazateli!\n");
	}	
	else {
		S->top--;
	}	
}

int SSizeB (tStackB *S) {
/*   -------
** Vrátí počet prvků v zásobníku
**/
  return(S->top);
}

bool SEmptyB (tStackB *S) {
/*   -------
** Je-li zásobník prázdný, vrátí hodnotu true.
**/
  return(S->top==0);
}

/* -------------------------------------------------------------------------- */
/*
** Následuje jádro domácí úlohy - funkce, které máte implementovat. 
*/

void BTInit (tBTNodePtr *RootPtr)	{
/*   ------
** Provede inicializaci binárního vyhledávacího stromu.
**
** Inicializaci smí programátor volat pouze před prvním použitím binárního
** stromu, protože neuvolňuje uzly neprázdného stromu (a ani to dělat nemůže,
** protože před inicializací jsou hodnoty nedefinované, tedy libovolné).
** Ke zrušení binárního stromu slouží procedura BTDisposeTree.
**	
** Všimněte si, že zde se poprvé v hlavičce objevuje typ ukazatel na ukazatel,	
** proto je třeba při práci s RootPtr použít dereferenční operátor *.
**/
	*RootPtr = NULL;
}

void BTInsert (tBTNodePtr *RootPtr, int Content) {
/*   --------
** Vloží do stromu nový uzel s hodnotou Content.
**
** Z pohledu vkládání chápejte vytvářený strom jako binární vyhledávací strom,
** kde uzly s hodnotou menší než má otec leží v levém podstromu a uzly větší
** leží vpravo. Pokud vkládaný uzel již existuje, neprovádí se nic (daná hodnota
** se ve stromu může vyskytnout nejvýše jednou). Pokud se vytváří nový uzel,
** vzniká vždy jako list stromu. Funkci implementujte nerekurzivně.
**/
	tBTNodePtr tmp = *RootPtr;
	tBTNodePtr prev = NULL;

	if (*RootPtr != NULL)	// vkladame do neprazdneho stromu
	{
		while (tmp != NULL)	// dokud nejsme v koncovem uzlu
		{
			prev = tmp;
			if (tmp->Cont == Content)	// uzel s hodnotou uz existuje
				return;
			else if (tmp->Cont < Content)	// musime prejit do praveho podstromu
				tmp = tmp->RPtr;
			else if (tmp->Cont > Content)	// musime prejit do leveho podstromu
				tmp = tmp->LPtr;
		}
		
		tBTNodePtr new = malloc(sizeof(struct tBTNode));	// alokujeme pamet
		if (new == NULL)	// overime uspesnost
			return;
		new->Cont = Content;	// ulozime data
		new->RPtr = NULL;	// inicializujeme ukazatele
		new->LPtr = NULL;

		if (prev->Cont < Content)	// navazeme do stromu
			prev->RPtr = new;
		else
			prev->LPtr = new;
	}
	else	// vkladame do prazdneho stomu
	{
		tBTNodePtr new = malloc(sizeof(struct tBTNode));	// alokujeme pamet
		if (new == NULL)					// overime uspesnost
			return;
		new->Cont = Content;	// ulozime do nove polozky data
		new->RPtr = NULL;	// inicializujeme ukazatele
		new->LPtr = NULL;
		*RootPtr = new;		// navazeme do stromu
	}
}

/*                                  PREORDER                                  */

void Leftmost_Preorder (tBTNodePtr ptr, tStackP *Stack)	{
/*   -----------------
** Jde po levě větvi podstromu, dokud nenarazí na jeho nejlevější uzel.
**
** Při průchodu Preorder navštívené uzly zpracujeme voláním funkce BTWorkOut()
** a ukazatele na ně is uložíme do zásobníku.
**/
	for ( ; ptr != NULL; ptr = ptr->LPtr)	// dokud nejsme na nejlevejsim, pokracujeme doleva
	{
		BTWorkOut(ptr);		// zpracuj aktualni uzel
		SPushP(Stack, ptr);	// ulozime ukazatel do zasobniku
	}
}

void BTPreorder (tBTNodePtr RootPtr)	{
/*   ----------
** Průchod stromem typu preorder implementovaný nerekurzivně s využitím funkce
** Leftmost_Preorder a zásobníku ukazatelů. Zpracování jednoho uzlu stromu
** realizujte jako volání funkce BTWorkOut(). 
**/
	tStackP StackP1;	// zasobnik ukazatelu na prochazene uzly
	SInitP(&StackP1);	// inicializace
	Leftmost_Preorder(RootPtr, &StackP1);	// dojdeme do nejlevejsiho uzlu

	while (SEmptyP(&StackP1) == FALSE)	// dokud jsou nejake nezpracovane uzly
	{
		RootPtr = STopPopP(&StackP1);			// vyjmeme posedni vlozeny uzel
		Leftmost_Preorder(RootPtr->RPtr, &StackP1);	// jdeme do praveho podstromu
	}
}


/*                                  INORDER                                   */ 

void Leftmost_Inorder(tBTNodePtr ptr, tStackP *Stack)		{
/*   ----------------
** Jde po levě větvi podstromu, dokud nenarazí na jeho nejlevější uzel.
**
** Při průchodu Inorder ukládáme ukazatele na všechny navštívené uzly do
** zásobníku. 
**/
	for ( ; ptr != NULL; ptr = ptr->LPtr)	// dokud nenarazime na nejlevejsi, pokracujeme doleva
		SPushP(Stack, ptr);		// ukladame uzly do zasobniku
}

void BTInorder (tBTNodePtr RootPtr)	{
/*   ---------
** Průchod stromem typu inorder implementovaný nerekurzivně s využitím funkce
** Leftmost_Inorder a zásobníku ukazatelů. Zpracování jednoho uzlu stromu
** realizujte jako volání funkce BTWorkOut(). 
**/
	tStackP StackP1;	// zasobnik ukazatelu na prochazene uzly
	SInitP(&StackP1);	// inicializace
	Leftmost_Inorder(RootPtr, &StackP1);	// dojdeme do nejlevejsiho uzlu

	while (SEmptyP(&StackP1) == FALSE)	// dokud jsou nejake nezpracovane uzly
	{
		RootPtr = STopPopP(&StackP1);	// vyjmeme posedni vlozeny uzel
		BTWorkOut(RootPtr);		// zpracujeme uzel
		Leftmost_Inorder(RootPtr->RPtr, &StackP1);	// jdeme do praveho podstromu
	}
}

/*                                 POSTORDER                                  */ 

void Leftmost_Postorder (tBTNodePtr ptr, tStackP *StackP, tStackB *StackB) {
/*           --------
** Jde po levě větvi podstromu, dokud nenarazí na jeho nejlevější uzel.
**
** Při průchodu Postorder ukládáme ukazatele na navštívené uzly do zásobníku
** a současně do zásobníku bool hodnot ukládáme informaci, zda byl uzel
** navštíven poprvé a že se tedy ještě nemá zpracovávat. 
**/
	for ( ; ptr != NULL; ptr = ptr->LPtr)	// dokud nejsme na nejlevejsim uzlu, pokracujeme doleva
	{
		SPushP(StackP, ptr);	// ukladame ukazatele na aktualni uzel
		SPushB(StackB, TRUE);	// ukladame, ze uzel je navstiven poprve
	}
}

void BTPostorder (tBTNodePtr RootPtr)	{
/*           -----------
** Průchod stromem typu postorder implementovaný nerekurzivně s využitím funkce
** Leftmost_Postorder, zásobníku ukazatelů a zásobníku hotdnot typu bool.
** Zpracování jednoho uzlu stromu realizujte jako volání funkce BTWorkOut(). 
**/
	tStackP StackP1;	// zasobnik ukazatelu na prochazene uzly
	SInitP(&StackP1);	// inicializace

	tStackB StackB1;	// zasobnik bool hodnot
	SInitB(&StackB1);	// inicializace

	Leftmost_Postorder(RootPtr, &StackP1, &StackB1);	// dojdeme do nejlevejsiho uzlu

	while (SEmptyP(&StackP1) == FALSE)	// dokud jsou nejake nezpracovane uzly
	{
		RootPtr = STopP(&StackP1);	// ulozime posedni vlozeny uzel
		
		if (STopPopB(&StackB1) == FALSE)	// jestlize navstevujeme uzel potreti
		{
			SPopP(&StackP1);	// vyjmeme posledni vlozeny uzel
			BTWorkOut(RootPtr);	// zpracujeme jej
		}
		else	// navstevujeme uzel podruhe
		{
			SPushB(&StackB1, FALSE);	// ulozime false na zasobnik
			Leftmost_Postorder(RootPtr->RPtr, &StackP1, &StackB1);	// prejdeme do praveho podstromu
		}
	}
}


int BTHeight (tBTNodePtr RootPtr) {	
/*   ----------
** Vypočítá výšku BVS bez použití rekurze
**
** Návratová hodnota je výška stromu. Funkci implementujte nerekurzivně
** bez deklarování jakékoli další pomocné funkce, která není v zadání.
** Využijte pomocných zásobníků. Je doporučeno použít jeden ze zásobníků
** pro průběžné ukládání cesty od kořene stromu. Počet uzlů na takovéto
** cestě můžete zjistit použitím funkce SSizeP. Výška stromu je rovna
** délce (počtu hran) nejdelší cesty  od kořene k lisu.
**
** Výška prázdného stromu však není definována. V případě prázdného stromu
** bude funkce vracet hodnotu -1.  
**/
	if (RootPtr == NULL)	// vyska prazdneho stromu
		return -1;

	tStackP StackP1;	// zasobnik ukazatelu na prochazene uzly
	SInitP(&StackP1);	// inicializace

	tStackB StackB1;	// zasobnik bool hodnot, zda se ma uzel zpracovat
	SInitB(&StackB1);	// inicializace

	for (tBTNodePtr ptr = RootPtr; ptr != NULL; ptr = ptr->LPtr)	// metodou postorder projdeme do nejlevejsiho uzlu
	{
		SPushP(&StackP1, ptr);		// kazdy uzel zpracujeme ulozenim jeho adresy
		SPushB(&StackB1, TRUE);		// a ulozenim hodnoty true
	}

	int max_height = SSizeP(&StackP1);	// pocatecni delka bude delka zpracovaneho podstromu

	tBTNodePtr tmp;
	while (SEmptyP(&StackP1) == FALSE)	// dokud jsme nezpracovali vsechny uzly
	{
		tmp = STopP(&StackP1);		// prejdu do predchoziho uzlu

		if ((tmp->RPtr == NULL) && (tmp->LPtr == NULL))		// jestlize uzel nema zadne podstromy
		{
			if (SSizeP(&StackP1) > max_height)		// overime, zda delka aktualni cesty neni delsi
				max_height = SSizeP(&StackP1);
		}

		if (STopPopB(&StackB1) == FALSE)	// jestlize jsem v uzlu potreti
			SPopP(&StackP1);		// vyjmu ze zasobniku

		else	// v uzlu podruhe
		{
			SPushB(&StackB1, FALSE);
			for (tBTNodePtr ptr = tmp->RPtr; ptr != NULL; ptr = ptr->LPtr)	// jdeme do praveho podstromu
			{
				SPushP(&StackP1, ptr);		// opet ho zpracujeme
				SPushB(&StackB1, TRUE);
			}
		}
	}
	return (max_height - 1); 
}


void BTDisposeTree (tBTNodePtr *RootPtr)	{
/*   -------------
** Zruší všechny uzly stromu a korektně uvolní jimi zabranou paměť.
**
** Funkci implementujte nerekurzivně s využitím zásobníku ukazatelů.
**/
	tStackP StackP1;
	SInitP(&StackP1);

	if (*RootPtr == NULL)	// v prazdnem stromu neni co rusit
		return;
	
	tBTNodePtr tmp;

	while (1)
	{
		tmp = *RootPtr;
		if ((tmp == NULL) && (SEmptyP(&StackP1) == TRUE))	// pokud nezbyvaji zadne uzly na ruseni
			break;						// ukoncime cyklus
		if (tmp != NULL)					// nejsme na konci vetve
		{
			if (tmp->RPtr != NULL)			// uzel ma pravy podstrom
				SPushP(&StackP1, tmp->RPtr);	// ulozim adresu praveho podstromu
			*RootPtr = tmp->LPtr;			// dale jdu do leveho
			free(tmp);				// uvolnim aktualni
		}
		else
			if (SEmptyP(&StackP1) == FALSE)		// jestlize jsou stale nezpracovane uzly
				tmp = STopPopP(&StackP1);	// prejdeme do posledniho z nich
	}
	*RootPtr = NULL;
}

/* konec c402.c */

