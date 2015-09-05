/* geometry.c - podstawowe obliczenia geometryczne */

#include "geometry.h"
#include <math.h>

static const double deg2rad = 3.14159265358979323846/180.0;

double wart_bezwzgl(double x)
{
    if(x >= 0)
        return x;
    return -x;
}

Vector makeVector(Point3D A, Point3D B)
{
    Vector res;
    for(int i = 0; i < 3; i++)
        res.skl[i] = B.wsp[i] - A.wsp[i];
    return res;
}

Vector neqVector(Vector V)
{
    for(int i = 0; i < 3; i++)
        V.skl[i] = -V.skl[i];
    return V;
}

Vector mnozVector(Vector V, double k)
{
    for(int i = 0; i < 3; i++)
        V.skl[i] *= k;
    return V;
}

Vector rob_prostopadly(Vector V)
{
    Vector W;
    W.skl[0] = V.skl[2] - V.skl[1];
    W.skl[1] = V.skl[0];
    W.skl[2] = -W.skl[1];
    if(W.skl[0] == 0 && W.skl[1] == 0 && W.skl[2] == 0)
        W.skl[0] = 1.0;
    return W;
}

/* przesuniêcie punktu P o wektor V */
void translate_V(Point3D *P, const Vector V)
{
    for(int i = 0; i < 3; i++)
        P->wsp[i] += V.skl[i];
}

Vector addVectors(Vector V1, Vector V2)
{
    Vector V;
    for(int i = 0; i < 3; i++)
        V.skl[i] = V1.skl[i] + V2.skl[i];
    return V;
}

double scalar_product(Vector V1, Vector V2)
{
    return (V1.skl[0]*V2.skl[0] + V1.skl[1]*V2.skl[1] + V1.skl[2]*V2.skl[2]);
}

Vector cross_product(Vector V1, Vector V2)
{
    Vector res;
    res.skl[0] = V1.skl[1]*V2.skl[2] - V1.skl[2]*V2.skl[1];
    res.skl[1] = V1.skl[2]*V2.skl[0] - V1.skl[0]*V2.skl[2];
    res.skl[2] = V1.skl[0]*V2.skl[1] - V1.skl[1]*V2.skl[0];
    return res;
}

double VectorLength(Vector V)
{
  //  double res = V.skl[0]*V.skl[0] + V.skl[1]*V.skl[1] + V.skl[2]*V.skl[2];
//    return sqrt(res);
    return sqrt(V.skl[0]*V.skl[0] + V.skl[1]*V.skl[1] + V.skl[2]*V.skl[2]);
}

double CosBetweenVectors(Vector V1, Vector V2)
{
    return (scalar_product(V1, V2) / (VectorLength(V1) * VectorLength(V2)));
}

void VectorNormalize(Vector *V)
{
    double length = VectorLength(*V);
    for(int i = 0; i < 3; i++)
        V->skl[i] = V->skl[i] / length;
}

Vector normal(Point3D A, Point3D B, Point3D C)
{
    Vector t1, t2;
    for(int i = 0; i < 3; i++)
    {
        t1.skl[i] = C.wsp[i] - A.wsp[i];
        t2.skl[i] = B.wsp[i] - A.wsp[i];
    }
    return cross_product(t1, t2);
}

int PointsIdentical(Point3D P, Point3D Q)
{
    return (P.wsp[0] == Q.wsp[0] && P.wsp[1] == Q.wsp[1] && P.wsp[2] == Q.wsp[2]);
}

int PointsCollinear(Point3D P, Point3D Q, Point3D R)
{
    if(P.wsp[0]*(Q.wsp[1]-R.wsp[1]) + Q.wsp[0]*(R.wsp[1]-P.wsp[1]) + R.wsp[0]*(P.wsp[1]-Q.wsp[1]) == 0 &&
       P.wsp[0]*(Q.wsp[2]-R.wsp[2]) + Q.wsp[0]*(R.wsp[2]-P.wsp[2]) + R.wsp[0]*(P.wsp[2]-Q.wsp[2]) == 0 &&
       P.wsp[1]*(Q.wsp[2]-R.wsp[2]) + Q.wsp[1]*(R.wsp[2]-P.wsp[2]) + R.wsp[1]*(P.wsp[2]-Q.wsp[2]) == 0)
        return 1;
    return 0;
}

void ABCD_Plane(Plane *fi)
{
    Vector PP[3];
    for(int i = 0; i < 3; i++)
        for(int j = 0; j < 3; j++)
            PP[i].skl[j] = fi->P[i].wsp[j];

    Vector ABC = cross_product(addVectors(PP[1], neqVector(PP[0])), addVectors(PP[2], neqVector(PP[0])));

    for(int i = 0; i < 3; i++)
        fi->prm[i] = ABC.skl[i];

    fi->prm[3] = -(ABC.skl[0] * fi->P[0].wsp[0] + ABC.skl[1] * fi->P[0].wsp[1] + ABC.skl[2] * fi->P[0].wsp[2]);
}

int GenericObjIdentical(GenericObj X, GenericObj Y)
{
    if(X.kind != Y.kind)
        return 0;
    int odp = 1;
    for(int i = 0; i <= X.kind; i++)
        odp &= PointsIdentical(X.V[i], Y.V[i]);
    return odp;
}

Plane prowadz_plaszczyzne(GenericObj *T)
{
    Plane temp;
    if(T->kind >= 2)
    {
        for(int i = 0; i < 3; i++)
            temp.P[i] = T->V[i];
        ABCD_Plane(&temp);
        return temp;
    }
    if(T->kind == 1)
    {
        for(int i = 0; i < 2; i++)
            temp.P[i] = T->V[i];

        Vector V = makeVector(temp.P[0], temp.P[1]);
        V = rob_prostopadly(V);
        temp.P[2] = temp.P[0];
        translate_V(&temp.P[2], V);
        ABCD_Plane(&temp);
        return temp;
    }
   /* if(T->kind == 0) */
    {
        temp.P[0] = T->V[0];
        temp.P[1] = T->V[0]; temp.P[1].wsp[0] += 1;
        Vector V = makeVector(temp.P[0], temp.P[1]);
        V = rob_prostopadly(V);
        temp.P[2] = temp.P[0];
        translate_V(&temp.P[2], V);
        ABCD_Plane(&temp);
        return temp;
    }
}

Point2D projectOrtho(Point3D P, Plane* fi)
{
    double r = (fi->prm[0]*P.wsp[0] + fi->prm[1]*P.wsp[1] + fi->prm[2]*P.wsp[2] + fi->prm[3]);
    r = r / (fi->prm[0]*fi->prm[0] + fi->prm[1]*fi->prm[1] + fi->prm[2]*fi->prm[2]);
    Point2D res;
    for(int i = 0; i < 2; i++)
        res.wsp[i] = P.wsp[i] - r * fi->prm[i];
    return res;
}

Point3D projectOrtho3D(Point3D P, Plane* fi)
{
    double r = (fi->prm[0]*P.wsp[0] + fi->prm[1]*P.wsp[1] + fi->prm[2]*P.wsp[2] + fi->prm[3]);
    r = r / (fi->prm[0]*fi->prm[0] + fi->prm[1]*fi->prm[1] + fi->prm[2]*fi->prm[2]);
    Point3D res;
    for(int i = 0; i < 3; i++)
        res.wsp[i] = P.wsp[i] - r * fi->prm[i];
    return res;
}
