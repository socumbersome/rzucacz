#ifndef _RENDER_H_
#define _RENDER_H_

#include "geometry.h"
#include "lista.h"
#include "bsp.h"

void transformuj_uklad(Plane *rzutnia, Vector basis[3]);

void renderuj_wireframe(FILE *plik, Plane *rzutnia, wezel *obiekty);

void rysuj_polygons(lista polygons, Point3D* srodek_rzutni, Plane *rzutnia, Vector basis[3], FILE *plik);

void renderuj_hidden_face_removal(FILE *plik, Plane *rzutnia, wezel *obiekty);

#endif
