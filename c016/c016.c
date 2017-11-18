
/* c016.c: **********************************************************}
{* Téma:  Tabulka s Rozptýlenými Položkami
**                      První implementace: Petr Přikryl, prosinec 1994
**                      Do jazyka C prepsal a upravil: Vaclav Topinka, 2005
**                      Úpravy: Karel Masařík, říjen 2014
**                              Radek Hranický, říjen 2014
**                              Radek Hranický, listopad 2015
**                              Radek Hranický, říjen 2016
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

    if (ptrht) { // pokud je ukazatel na tabulku validni
        for (int i = 0; i < HTSIZE; i++) {
            (*ptrht)[i] = NULL;
        }
    }

}

/* TRP s explicitně zřetězenými synonymy.
** Vyhledání prvku v TRP ptrht podle zadaného klíče key.  Pokud je
** daný prvek nalezen, vrací se ukazatel na daný prvek. Pokud prvek nalezen není, 
** vrací se hodnota NULL.
**
*/

tHTItem* htSearch ( tHTable* ptrht, tKey key ) {

    if (ptrht) { // pokud je ukazatel na tabulku validni
        for (int i = 0; i < HTSIZE; i++) {
            if ( (*ptrht)[i] != NULL ) { // pokud je na danem indexu ukazatel na synonymum
                tHTItem *item = (*ptrht)[i];
                while (item) {
                    if (item->key == key) {
                        return item; // pokud je nalezen prvek s klicem
                    }
                    else {
                        item = item->ptrnext;
                    }
                }
            }
        }
    }
    return NULL; // pokud prvek s klicem neni nalezen

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

    if (ptrht) { // pokud je ukazatel na tabulku validni
        // zjisteni, zda prvku s klicem uz existuje
        tHTItem *item = htSearch(ptrht, key);
        if (item) { // aktualizace dat prvku
            item->data = data;
        }
        else { // vlozeni noveho prvku
            // alokovani pameti pro novy prvek
            tHTItem *newitem; // novy prvek
            if ( (newitem = (tHTItem*)malloc(sizeof(tHTItem))) == NULL ) {
                return; // chyba pri vytvareni noveho prvku
            }
            // inicializace noveho prvku
            newitem->key = key;
            newitem->data = data;
            // vlozeni noveho prvku
            tHTItem *itemToJoin = (*ptrht)[hashCode(key)];
            newitem->ptrnext = itemToJoin;
            (*ptrht)[hashCode(key)] = newitem;
        }
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

    if (ptrht) { // pokud je ukazatel na tabulku validni
        tHTItem *searchedItem = htSearch(ptrht, key);
        if (searchedItem) { // pokud byla nalezena polozka s danym klicem
            return &(searchedItem->data);
        }
    }
    return NULL; // pokud polozka nebyla nalezena

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

    if (ptrht) { // pokud ukazatel na tabulku je validni
        tHTItem* item = (*ptrht)[hashCode(key)];
        tHTItem* prevItem;
        if(item) { // pokud existuje prvek s danym klicem
            if (item->key == key) {
                (*ptrht)[hashCode(key)] = item->ptrnext;
                free(item);
                return;
            }

            prevItem = item; // pamatovani predchoziho prvku
            item = item->ptrnext; // posunuti na dalsi prvek
            while (item) { // vyhledani prvku ke smazani
                if (item->key == key) {
                    prevItem->ptrnext = item->ptrnext;
                    free(item);
                    return;
                }
                prevItem = item;
                item = item->ptrnext;
            }
        }
    }

}

/* TRP s explicitně zřetězenými synonymy.
** Tato procedura zruší všechny položky tabulky, korektně uvolní prostor,
** který tyto položky zabíraly, a uvede tabulku do počátečního stavu.
*/

void htClearAll ( tHTable* ptrht ) {

    if (ptrht) { // pokud je ukazatel na tabulku validni
        for (int i = 0; i < HTSIZE; i++) {
            while ( (*ptrht)[i] != NULL ) { // dokud dany index tabulky neni prazdny
                tHTItem* itemToDelete = (*ptrht)[i];
                (*ptrht)[i] = itemToDelete->ptrnext;
                free(itemToDelete);
            }
        }
    }

}
