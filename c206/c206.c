	
/* c206.c **********************************************************}
{* T�ma: Dvousm�rn� v�zan� line�rn� seznam
**
**                   N�vrh a referen�n� implementace: Bohuslav K�ena, ��jen 2001
**                            P�epracovan� do jazyka C: Martin Tu�ek, ��jen 2004
**                                            �pravy: Bohuslav K�ena, ��jen 2014
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
	L->First = NULL;
	L->Act = NULL;
	L->Last = NULL;
}

void DLDisposeList (tDLList *L) {
/*
** Zru�� v�echny prvky seznamu L a uvede seznam do stavu, v jak�m
** se nach�zel po inicializaci. Ru�en� prvky seznamu budou korektn�
** uvoln�ny vol�n�m operace free. 
**/
	L->Act = L->First;				/* Nastav� aktivitu zoznamu na prv� prvok */
	
	while(L->Act != NULL)
	{
		L->First = L->Act->rptr;	/* Nastav� nasleduj�ci prvok ako prv� */
		free(L->Act);				/* Uvo�n� akt�vny prvok */
		L->Act = L->First;			/* Nastav� �al�� (po novom prv�) prvok ako aktu�lny */
	}
	
	L->Last = NULL;
}

void DLInsertFirst (tDLList *L, int val) {
/*
** Vlo�� nov� prvek na za��tek seznamu L.
** V p��pad�, �e nen� dostatek pam�ti pro nov� prvek p�i operaci malloc,
** vol� funkci DLError().
**/
	tDLElemPtr newFirst = NULL;
	
	if ((newFirst = malloc(sizeof(struct tDLElem))) == NULL)
	{ /* Alokuje pam� pre nov� prvok, v pr�pade zlyhania vol� DLError() a vracia riadenie volaj�cej funkcii */
		DLError();
		return;
	}
	
	newFirst->data = val;
	newFirst->lptr = NULL;
	newFirst->rptr = L->First;		/* Pointer na nasleduj�ci prvok nastav� na prv� prvok v zozname */
	if (L->First == NULL)
		L->Last = newFirst;			/* Ak je zoznam pr�zdny nov� prv� prvok bude z�rove� posledn� */
	else
		L->First->lptr = newFirst;
		
	L->First = newFirst;
}

void DLInsertLast(tDLList *L, int val) {
/*
** Vlo�� nov� prvek na konec seznamu L (symetrick� operace k DLInsertFirst).
** V p��pad�, �e nen� dostatek pam�ti pro nov� prvek p�i operaci malloc,
** vol� funkci DLError().
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
		L->First = newLast;			/* Ak je zoznam pr�zdny nov� posledn� prvok bude z�rove� prv� */
	else
		L->Last->rptr = newLast;
	
	L->Last = newLast;
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
	if (L->First == NULL)
	{
		DLError();
		return;
	}
	*val = L->First->data;
}

void DLCopyLast (tDLList *L, int *val) {
/*
** Prost�ednictv�m parametru val vr�t� hodnotu posledn�ho prvku seznamu L.
** Pokud je seznam L pr�zdn�, vol� funkci DLError().
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
** Zru�� prvn� prvek seznamu L. Pokud byl prvn� prvek aktivn�, aktivita 
** se ztr�c�. Pokud byl seznam L pr�zdn�, nic se ned�je.
**/
	if (L->First != NULL)
	{
		tDLElemPtr temp = L->First;		/* Do pomocnej premennej uklad� adresu prv�ho prvku zoznamu */
		if (L->Act == L->First)
			L->Act = NULL;				/* Ak je prv� prvok z�rove� akt�vny, aktivita sa str�ca */
		if (L->First == L->Last)		/* Ak m� zoznam jedin� prvok je potrebn� zru�i� pointer na prv� aj posledn� prvok (identick�) */
		{
			L->First = NULL;
			L->Last = NULL;
		}
		else
		{
			L->First = L->First->rptr;	/* Prv�m prvkom sa st�va nasleduj�ci prvok */
			L->First->lptr = NULL;
		}	
		free(temp);						/* P�vodn� prv� prvok zanik�, pam� je vr�ten� OS */
	}
}	

void DLDeleteLast (tDLList *L) {
/*
** Zru�� posledn� prvek seznamu L. Pokud byl posledn� prvek aktivn�,
** aktivita seznamu se ztr�c�. Pokud byl seznam L pr�zdn�, nic se ned�je.
**/ 
	if (L->First != NULL)
	{
		tDLElemPtr temp = L->Last;		/* Do pomocnej premennej uklad� adresu posledn�ho prvku zoznamu */
		if (L->Act == L->Last)
			L->Act = NULL;				/* Ak je prv� prvok z�rove� akt�vny, aktivita sa str�ca */
		if (L->First == L->Last)		/* Ak m� zoznam jedin� prvok je potrebn� zru�i� pointer na prv� aj posledn� prvok (identick�) */
		{
			L->First = NULL;
			L->Last = NULL;
		}
		else
		{
			L->Last = L->Last->lptr;	/* Posledn�m prvkom sa st�va predch�dzaj�ci prvok */
			L->Last->rptr = NULL;
		}	
		free(temp);						/* P�vodn� prv� prvok zanik�, pam� je vr�ten� OS */
	}
}

void DLPostDelete (tDLList *L) {
/*
** Zru�� prvek seznamu L za aktivn�m prvkem.
** Pokud je seznam L neaktivn� nebo pokud je aktivn� prvek
** posledn�m prvkem seznamu, nic se ned�je.
**/
	if ((L->Act != NULL) && (L->Act != L->Last))
	{
		tDLElemPtr temp = L->Act->rptr;	/* Do pomocnej premennej vlo�� prvok zoznamu za akt. prvkom */
		L->Act->rptr = temp->rptr;		/* Ako nasleduj�ci prvok nastav� prvok za t�m, kt. bude vymazan� */
		if (temp == L->Last)			/* Ak je vymaz�van� prvok z�rove� posledn�, posledn�m sa st�va prvok akt�vny, teda prvok pred n�m */
			L->Last = L->Act;
		else
			temp->rptr->lptr = L->Act;	/* Inak prepoj� prvok za vamaz�van�m s prvkom akt�vnym (teda vynech� vymaz�van� prvok) */
		
		free(temp);
	}
}

void DLPreDelete (tDLList *L) {
/*
** Zru�� prvek p�ed aktivn�m prvkem seznamu L .
** Pokud je seznam L neaktivn� nebo pokud je aktivn� prvek
** prvn�m prvkem seznamu, nic se ned�je.
**/
	if ((L->Act != NULL) && (L->Act != L->First))
	{
		tDLElemPtr temp = L->Act->lptr;		/* Do pomocnej premennej vlo�� prvok zoznamu pred akt. prvkom */
		L->Act->lptr = temp->lptr;			/* Ako predch�zaj�ci prvok nastav� prvok pred t�m, kt. bude vymazan� */
		if (temp == L->First)				/* Rovnak� princ�p ako vo funkcii DLPostDelete */
			L->First = L->Act;
		else
			temp->lptr->rptr = L->Act;
		free(temp);
	}
}

void DLPostInsert (tDLList *L, int val) {
/*
** Vlo�� prvek za aktivn� prvek seznamu L.
** Pokud nebyl seznam L aktivn�, nic se ned�je.
** V p��pad�, �e nen� dostatek pam�ti pro nov� prvek p�i operaci malloc,
** vol� funkci DLError().
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
	newPostInsert->lptr = L->Act;						/* Akt�vny prvok sa st�va predch�dzaj�cim prvku newPostInsert */
	newPostInsert->rptr = L->Act->rptr;					/* Nasleduj�ci prvok ost�va, teda sa prevezme z L->Act->rptr */
	L->Act->rptr = newPostInsert;
	if (L->Act == L->Last)
		L->Last = newPostInsert;
	else
		newPostInsert->rptr->lptr = newPostInsert;
}

void DLPreInsert (tDLList *L, int val) {
/*
** Vlo�� prvek p�ed aktivn� prvek seznamu L.
** Pokud nebyl seznam L aktivn�, nic se ned�je.
** V p��pad�, �e nen� dostatek pam�ti pro nov� prvek p�i operaci malloc,
** vol� funkci DLError().
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
** Prost�ednictv�m parametru val vr�t� hodnotu aktivn�ho prvku seznamu L.
** Pokud seznam L nen� aktivn�, vol� funkci DLError ().
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
** P�ep�e obsah aktivn�ho prvku seznamu L.
** Pokud seznam L nen� aktivn�, ned�l� nic.
**/
	if (L->Act != NULL)
		L->Act->data = val;
}

void DLSucc (tDLList *L) {
/*
** Posune aktivitu na n�sleduj�c� prvek seznamu L.
** Nen�-li seznam aktivn�, ned�l� nic.
** V�imn�te si, �e p�i aktivit� na posledn�m prvku se seznam stane neaktivn�m.
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
** Posune aktivitu na p�edchoz� prvek seznamu L.
** Nen�-li seznam aktivn�, ned�l� nic.
** V�imn�te si, �e p�i aktivit� na prvn�m prvku se seznam stane neaktivn�m.
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
** Je-li seznam aktivn�, vrac� true. V opa�n�m p��pad� vrac� false.
** Funkci implementujte jako jedin� p��kaz.
**/
	return L->Act != NULL;		/* Vracia TRUE (1) ak L->Act ukazuje na prvok, FALSE (0) ak nadob�da hodnotu NULL */
}

/* Konec c206.c*/
