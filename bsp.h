#ifndef _BSP_H_
#define _BSP_H_

#include "geometry.h"
#include "lista.h"
#include "renderer.h"
#include <stdio.h>
#include <stdlib.h>

#define MAXPTS 40

static const double EPSILON = 10e-5;

typedef struct bsp_tree
{
    Plane partition;
    lista polygons;
    struct bsp_tree *front, *back;
} BSP;

typedef GenericObj polygon;

enum
{
    ZAWIERA_SIE = 0,
    Z_TYLU,
    Z_PRZODU,
    PRZECINA
};

int klasyfikuj_polygon(polygon poly, Plane partition);

double klasyfikuj_punkt(Point3D *P, Plane *partition);

polygon nowy_polygon(Point3D *points, int v_count);

void dziel_polygon(polygon poly, Plane partition, polygon *przednia_czesc, polygon *tylna_czesc);

BSP* buduj_BSP(BSP *tree, lista *polygons);

void rysuj_BSP(BSP *tree, Point3D* srodek_rzutni, Plane *rzutnia, Vector basis[3], FILE *plik);

void kasuj_BSP(BSP *tree);

#endif
