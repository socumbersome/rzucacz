#ifndef _LISTA_H_
#define _LISTA_H_

#include "geometry.h"

#define TYP GenericObj
#define CZYT(x) scanf("%lf",&x)
#define PISZ(x) printf("%.3f",x)

typedef struct element {
    TYP wartosc;
    struct element *nast;
} wezel;

typedef wezel* lista;

wezel* stworz(TYP w);

void usun(wezel* p, wezel* red);

wezel* usun_nowe(wezel* p, wezel* red);

wezel* wstaw_na_pocz(wezel *p, TYP x);

wezel* wstaw_na_kon(wezel *p, TYP x);

int ile(wezel *p);

void wypisz_liste(wezel *p);

wezel* usun_z_pozycji(wezel* p, int nr);

int szukaj_wartosci(wezel *p, TYP x);

void sprawdz_pozycje(wezel *p, int nr);

void niszcz_liste(wezel *p);

#endif
