	
/* c206.c **********************************************************}
{* Téma: Dvousmìrnì vázaný lineární seznam
**
**                   Návrh a referenèní implementace: Bohuslav Køena, øíjen 2001
**                            Pøepracované do jazyka C: Martin Tuèek, øíjen 2004
**                                            Úpravy: Bohuslav Køena, øíjen 2014
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
	L->First = NULL;
	L->Act = NULL;
	L->Last = NULL;
}

void DLDisposeList (tDLList *L) {
/*
** Zru¹í v¹echny prvky seznamu L a uvede seznam do stavu, v jakém
** se nacházel po inicializaci. Ru¹ené prvky seznamu budou korektnì
** uvolnìny voláním operace free. 
**/
	L->Act = L->First;				/* Nastaví aktivitu zoznamu na prvý prvok */
	
	while(L->Act != NULL)
	{
		L->First = L->Act->rptr;	/* Nastaví nasledujúci prvok ako prvý */
		free(L->Act);				/* Uvoµní aktívny prvok */
		L->Act = L->First;			/* Nastaví ïal¹í (po novom prvý) prvok ako aktuálny */
	}
	
	L->Last = NULL;
}

void DLInsertFirst (tDLList *L, int val) {
/*
** Vlo¾í nový prvek na zaèátek seznamu L.
** V pøípadì, ¾e není dostatek pamìti pro nový prvek pøi operaci malloc,
** volá funkci DLError().
**/
	tDLElemPtr newFirst = NULL;
	
	if ((newFirst = malloc(sizeof(struct tDLElem))) == NULL)
	{ /* Alokuje pamä» pre nový prvok, v prípade zlyhania volá DLError() a vracia riadenie volajúcej funkcii */
		DLError();
		return;
	}
	
	newFirst->data = val;
	newFirst->lptr = NULL;
	newFirst->rptr = L->First;		/* Pointer na nasledujúci prvok nastaví na prvý prvok v zozname */
	if (L->First == NULL)
		L->Last = newFirst;			/* Ak je zoznam prázdny nový prvý prvok bude zároveò posledný */
	else
		L->First->lptr = newFirst;
		
	L->First = newFirst;
}

void DLInsertLast(tDLList *L, int val) {
/*
** Vlo¾í nový prvek na konec seznamu L (symetrická operace k DLInsertFirst).
** V pøípadì, ¾e není dostatek pamìti pro nový prvek pøi operaci malloc,
** volá funkci DLError().
**/ 	
	tDLElemPtr newLast = NULL;
	
	if ((newLast = malloc(sizeof(struct tDLElem))) == NULL)
	{
		DLError();
		return;
	}
	
	newLast->data = val;
	newLast->lptr = L->Last;
	newLast->rptr = NULL;
	if (L->First == NULL)
		L->First = newLast;			/* Ak je zoznam prázdny nový posledný prvok bude zároveò prvý */
	else
		L->Last->rptr = newLast;
	
	L->Last = newLast;
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
	if (L->First == NULL)
	{
		DLError();
		return;
	}
	*val = L->First->data;
}

void DLCopyLast (tDLList *L, int *val) {
/*
** Prostøednictvím parametru val vrátí hodnotu posledního prvku seznamu L.
** Pokud je seznam L prázdný, volá funkci DLError().
**/
	if (L->First == NULL)
	{
		DLError();
		return;
	}
	*val = L->Last->data;
}

void DLDeleteFirst (tDLList *L) {
/*
** Zru¹í první prvek seznamu L. Pokud byl první prvek aktivní, aktivita 
** se ztrácí. Pokud byl seznam L prázdný, nic se nedìje.
**/
	if (L->First != NULL)
	{
		tDLElemPtr temp = L->First;		/* Do pomocnej premennej ukladá adresu prvého prvku zoznamu */
		if (L->Act == L->First)
			L->Act = NULL;				/* Ak je prvý prvok zároveò aktívny, aktivita sa stráca */
		if (L->First == L->Last)		/* Ak má zoznam jediný prvok je potrebné zru¹i» pointer na prvý aj posledný prvok (identické) */
		{
			L->First = NULL;
			L->Last = NULL;
		}
		else
		{
			L->First = L->First->rptr;	/* Prvým prvkom sa stáva nasledujúci prvok */
			L->First->lptr = NULL;
		}	
		free(temp);						/* Pôvodný prvý prvok zaniká, pamä» je vrátená OS */
	}
}	

void DLDeleteLast (tDLList *L) {
/*
** Zru¹í poslední prvek seznamu L. Pokud byl poslední prvek aktivní,
** aktivita seznamu se ztrácí. Pokud byl seznam L prázdný, nic se nedìje.
**/ 
	if (L->First != NULL)
	{
		tDLElemPtr temp = L->Last;		/* Do pomocnej premennej ukladá adresu posledného prvku zoznamu */
		if (L->Act == L->Last)
			L->Act = NULL;				/* Ak je prvý prvok zároveò aktívny, aktivita sa stráca */
		if (L->First == L->Last)		/* Ak má zoznam jediný prvok je potrebné zru¹i» pointer na prvý aj posledný prvok (identické) */
		{
			L->First = NULL;
			L->Last = NULL;
		}
		else
		{
			L->Last = L->Last->lptr;	/* Posledným prvkom sa stáva predchádzajúci prvok */
			L->Last->rptr = NULL;
		}	
		free(temp);						/* Pôvodný prvý prvok zaniká, pamä» je vrátená OS */
	}
}

void DLPostDelete (tDLList *L) {
/*
** Zru¹í prvek seznamu L za aktivním prvkem.
** Pokud je seznam L neaktivní nebo pokud je aktivní prvek
** posledním prvkem seznamu, nic se nedìje.
**/
	if ((L->Act != NULL) && (L->Act != L->Last))
	{
		tDLElemPtr temp = L->Act->rptr;	/* Do pomocnej premennej vlo¾í prvok zoznamu za akt. prvkom */
		L->Act->rptr = temp->rptr;		/* Ako nasledujúci prvok nastaví prvok za tým, kt. bude vymazaný */
		if (temp == L->Last)			/* Ak je vymazávaný prvok zároveò posledný, posledným sa stáva prvok aktívny, teda prvok pred ním */
			L->Last = L->Act;
		else
			temp->rptr->lptr = L->Act;	/* Inak prepojí prvok za vamazávaným s prvkom aktívnym (teda vynechá vymazávaný prvok) */
		
		free(temp);
	}
}

void DLPreDelete (tDLList *L) {
/*
** Zru¹í prvek pøed aktivním prvkem seznamu L .
** Pokud je seznam L neaktivní nebo pokud je aktivní prvek
** prvním prvkem seznamu, nic se nedìje.
**/
	if ((L->Act != NULL) && (L->Act != L->First))
	{
		tDLElemPtr temp = L->Act->lptr;		/* Do pomocnej premennej vlo¾í prvok zoznamu pred akt. prvkom */
		L->Act->lptr = temp->lptr;			/* Ako predcházajúci prvok nastaví prvok pred tým, kt. bude vymazaný */
		if (temp == L->First)				/* Rovnaký princíp ako vo funkcii DLPostDelete */
			L->First = L->Act;
		else
			temp->lptr->rptr = L->Act;
		free(temp);
	}
}

void DLPostInsert (tDLList *L, int val) {
/*
** Vlo¾í prvek za aktivní prvek seznamu L.
** Pokud nebyl seznam L aktivní, nic se nedìje.
** V pøípadì, ¾e není dostatek pamìti pro nový prvek pøi operaci malloc,
** volá funkci DLError().
**/
	if (L->Act == NULL)
		return;
	
	tDLElemPtr newPostInsert = NULL;
	
	if ((newPostInsert = malloc(sizeof(struct tDLElem))) == NULL)
	{
		DLError();
		return;
	}
	
	newPostInsert->data = val;
	newPostInsert->lptr = L->Act;						/* Aktívny prvok sa stáva predchádzajúcim prvku newPostInsert */
	newPostInsert->rptr = L->Act->rptr;					/* Nasledujúci prvok ostáva, teda sa prevezme z L->Act->rptr */
	L->Act->rptr = newPostInsert;
	if (L->Act == L->Last)
		L->Last = newPostInsert;
	else
		newPostInsert->rptr->lptr = newPostInsert;
}

void DLPreInsert (tDLList *L, int val) {
/*
** Vlo¾í prvek pøed aktivní prvek seznamu L.
** Pokud nebyl seznam L aktivní, nic se nedìje.
** V pøípadì, ¾e není dostatek pamìti pro nový prvek pøi operaci malloc,
** volá funkci DLError().
**/
	if (L->Act == NULL)
		return;
	
	tDLElemPtr newPreInsert = NULL;
	
	if ((newPreInsert = malloc(sizeof(struct tDLElem))) == NULL)
	{
		DLError();
		return;
	}
	
	newPreInsert->data = val;
	newPreInsert->rptr = L->Act;
	newPreInsert->lptr = L->Act->lptr;
	L->Act->lptr = newPreInsert;
	if (L->Act == L->First)
		L->First = newPreInsert;
	else
		newPreInsert->lptr->rptr = newPreInsert;
}

void DLCopy (tDLList *L, int *val) {
/*
** Prostøednictvím parametru val vrátí hodnotu aktivního prvku seznamu L.
** Pokud seznam L není aktivní, volá funkci DLError ().
**/
	if (L->Act == NULL)
	{
		DLError();
		return;
	}
	
	*val = L->Act->data;
}

void DLActualize (tDLList *L, int val) {
/*
** Pøepí¹e obsah aktivního prvku seznamu L.
** Pokud seznam L není aktivní, nedìlá nic.
**/
	if (L->Act != NULL)
		L->Act->data = val;
}

void DLSucc (tDLList *L) {
/*
** Posune aktivitu na následující prvek seznamu L.
** Není-li seznam aktivní, nedìlá nic.
** V¹imnìte si, ¾e pøi aktivitì na posledním prvku se seznam stane neaktivním.
**/
	if (L->Act == NULL)
		return;
	
	if (L->Act == L->Last)
		L->Act = NULL;
	else
		L->Act = L->Act->rptr;
}


void DLPred (tDLList *L) {
/*
** Posune aktivitu na pøedchozí prvek seznamu L.
** Není-li seznam aktivní, nedìlá nic.
** V¹imnìte si, ¾e pøi aktivitì na prvním prvku se seznam stane neaktivním.
**/
	if (L->Act == NULL)
		return;
	
	if (L->Act == L->First)
		L->Act = NULL;
	else
		L->Act = L->Act->lptr;
}

int DLActive (tDLList *L) {
/*
** Je-li seznam aktivní, vrací true. V opaèném pøípadì vrací false.
** Funkci implementujte jako jediný pøíkaz.
**/
	return L->Act != NULL;		/* Vracia TRUE (1) ak L->Act ukazuje na prvok, FALSE (0) ak nadobúda hodnotu NULL */
}

/* Konec c206.c*/
