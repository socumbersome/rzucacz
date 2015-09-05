/* geometry.h - podstawowe obliczenia geometryczne */
#ifndef _GEOMETRY_H_
#define _GEOMETRY_H_

#include <stdio.h>

#define MAXVERTICES 40

double wart_bezwzgl(double x);

/* współrzędne punktu na płaszczyźnie */
typedef struct { double wsp[2]; } Point2D;

/* współrzędne punktu w przestrzeni */
typedef struct { double wsp[3]; } Point3D;

/* wektor swobodny [a,b,c] w 3D
 * skladowa x = skl[0], skl. y = skl[1], skl. z = skl[2] */
typedef struct { double skl[3]; } Vector;

Vector makeVector(Point3D A, Point3D B);

/* zwraca wektor przeciwny do wektora V */
Vector neqVector(Vector V);

Vector mnozVector(Vector V, double k);

Vector rob_prostopadly(Vector V);

/* przesunięcie punktu P o wektor V */
void translate_V (Point3D *P, const Vector V);

/* zwraca sumę wektorów V1 i V2 */
Vector addVectors(Vector V1, Vector V2);

/* iloczyn skalarny wektorów V1 i V2 */
double scalar_product(Vector V1, Vector V2);

/* iloczyn wektorowy wektorów V1 i V2 */
Vector cross_product(Vector V1, Vector V2);

double VectorLength(Vector V);

double CosBetweenVectors(Vector V1, Vector V2);

void VectorNormalize(Vector *V);

Vector normal(Point3D A, Point3D B, Point3D C);

int PointsIdentical(Point3D P, Point3D Q);

int PointsCollinear(Point3D P, Point3D Q, Point3D R);

/* płaszczyzna - jednocześnie trzymamy dwie reprezentacje, tj.
 * przez zbiór punktów spełniających Ax + By + Cz + D = 0, gdzie (A,B,C)!=(0,0,0)
 * oraz prm[0] = A, prm[1] = B, prm[2] = C, prm[3] = D
 * oraz przez wybór 3 niewspółliniowych punktów leżących w tej płaszczyźnie */
typedef struct {
    double prm[4];
    Point3D P[3];
} Plane;

/* liczy pierwszą reprezentację płaszczyzny */
void ABCD_Plane(Plane *fi);

/* zeby nie bawic sie w 4 implementacje kontenerow, zrobimy generyczny typ
 * kind=0 - punkt, =1-odcinek, =2-trojkat, =3-czworoscian */
typedef struct {
    int kind;
    Point3D V[MAXVERTICES];
} GenericObj;

int GenericObjIdentical(GenericObj X, GenericObj Y);

Plane prowadz_plaszczyzne(GenericObj *T);

/* /////////////////////////////////////////////////////////// */

/* Wyznacza współrzędne rzutu prostokątnego (ortogonalnego) punktu P na płaszczyznę fi. */
Point2D projectOrtho (Point3D P, Plane* fi);
Point3D projectOrtho3D (Point3D P, Plane* fi);

#endif
