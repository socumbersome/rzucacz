/*
 * postscript.h - generowanie prostych plików postscriptowych
 *
 * Środek kartezjańskiego układu współrzędnych znajduje się
 * w środku kartki formatu A4. Współrzędne są podawane
 * w milimetrach - zatem prawy górny róg kartki ma współrzędne
 * (105.0,148.5), zaś lewy dolny róg - współrzędne (-105.0,-148.5).
 */

#ifndef _POSTSCRIPT_H_
#define _POSTSCRIPT_H_

#include <stdio.h>
#include <stdlib.h>
#include "geometry.h"

/*
 * definicja koloru w systemie RGB
 */
typedef struct { double r, g, b; } RGB;

/*
 * rozpoczęcie i zakończenie wypisywania pliku postscriptowego
 */
void openPostscript(FILE* plik, double lineWidth);

void closePostscript(FILE* plik);

void newPath(FILE* plik);

void closePath(FILE* plik);

void closePathStroke(FILE* plik);

void PathFillStroke(FILE* plik, const RGB kolor);

void PathFill(FILE* plik, const RGB kolor);

/*
 * przesunięcie żółwia do punktu o podanych współrzędnych
 * (bezwględnych lub względnych)
 */
void moveTo(FILE* plik, const Point2D P);

void relMoveTo(FILE* plik, const Point2D P);

void makeDot(FILE* plik); // kropka w aktualnym miejscu żółwia

/*
 * wykreślenie linii z bieżącej pozycji do punktu o podanych
 * współrzędnych (bezwględnych lub względnych)
 */
void lineTo(FILE* plik, const Point2D P);

void dash(FILE* plik);

void undash(FILE* plik);

void relLineTo(FILE* plik, const Point2D P);

/*
 * podstawowe kolory w systemie RGB
 */
extern const RGB black, red, green, blue, cyan, magenta, yellow, gray;

/*
 * ustalenie koloru pisaka jakim kreślone są linie
 */
void setColor(FILE* plik, const RGB rgb);

#endif
