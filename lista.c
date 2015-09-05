#include <stdio.h>
#include <stdlib.h>
#include "lista.h"

wezel* stworz(TYP w)
{
    wezel* t = (wezel*) malloc(sizeof(wezel));
    if(t == NULL)
    {
        printf("Nieudany malloc!\n");
        return NULL;
    //    exit(EXIT_FAILURE);
    }
    t -> wartosc = w;
    t -> nast = NULL;
    return t;
}

void usun(wezel* p, wezel* red)
{
    if(p == red)
        free(red);
    else
    {
        while((p -> nast) != red)
            p = p -> nast;

        p -> nast = red -> nast;
        free(red);
    }
}

wezel* usun_nowe(wezel* p, wezel* red)
{
    wezel* temp = p;
    if(p == red)
    {
        if(p -> nast == NULL)
        {
            free(red);
            return NULL;
        }
        else
        {
            temp = p -> nast;
            free(red);
            return temp;
        }
    }
    else
    {
        while((p -> nast) != red)
            p = p -> nast;

        p -> nast = red -> nast;
        free(red);
        return temp;
    }
}

wezel* wstaw_na_pocz(wezel* p, TYP w) // zwraca poczatek listy
{
    wezel* nowy = stworz(w);
    nowy -> nast = p;
    return nowy;
}

wezel* wstaw_na_kon(wezel* p, TYP w)
{
    if(!p) // p == NULL
        return stworz(w);
    p -> nast = wstaw_na_kon(p -> nast, w);
    return p;
}

int ile(wezel *p)
{
    if(!p)
        return 0;
    return (1 + ile(p -> nast));
}

void wypisz_liste(wezel* p)
{
    if(!p)
        return;
    PISZ(p -> wartosc.V[0].wsp[0]);
    printf(" ");
    wypisz_liste(p -> nast);
}

wezel* usun_z_pozycji(wezel* p, int nr)
{
    if(nr >= 0 && nr <= ile(p) - 1)
    {
        wezel* smiec = p;
        wezel* poprz = NULL;
        while(nr > 0)
        {
            poprz = smiec;
            smiec = smiec -> nast;
            nr--;
        }
        if(poprz == NULL)
            if(p -> nast == NULL)
            {
                usun(p, smiec);
                return NULL;
            }
            else
            {
                wezel* n_pocz = p -> nast;
                usun(p, smiec);
                return n_pocz;
            }
        else
        {
            usun(p, smiec);
            return p;
        }
    }
    return p;
}

int szukaj_wartosci(wezel* p, TYP w) // zwraca nr el. w liscie
{
    int wyn = 0;

    while(!GenericObjIdentical(p -> wartosc, w) && (p -> nast != NULL))
    {
        p = p -> nast;
        wyn++;
    }
    if(GenericObjIdentical(p -> wartosc, w))
        return wyn;
    return -1; // nie ma takiego el. w liscie
}

void sprawdz_pozycje(wezel *p, int nr)
{
    if(nr >= 0 && nr <= ile(p) - 1)
    {
        while(nr > 0)
        {
            p = p -> nast;
            nr--;
        }
        PISZ(p -> wartosc);
    }
    else
        printf("Zly numer pozycji");
}

void niszcz_liste(wezel *p)
{
    if(p == NULL)
        return;

    wezel *red = p;
    p = p->nast;
    free(red);
    if(p != NULL)
        niszcz_liste(p);
}
