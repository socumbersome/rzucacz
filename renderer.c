#include "renderer.h"
#include "postscript.h"
#include <locale.h>

void transformuj_uklad(Plane *rzutnia, Vector basis[3])
{
    for(int i = 0; i < 3; i++)
        basis[2].skl[i] = rzutnia->prm[i];
    VectorNormalize(&basis[2]);

    Vector global_normal;
    /* po ludzku - OZ przechodzi na wektor normalny rzutni, a OX na iloczyn wektorowy
     * iloczyn wektorowy OZ i normalnej rzutni */
    global_normal.skl[0] = global_normal.skl[1] = 0.0; global_normal.skl[2] = 1.0;
    basis[0] = cross_product(global_normal, basis[2]);
    if(basis[0].skl[0] == 0 && basis[0].skl[1] == 0 && basis[0].skl[2] == 0)
        basis[0].skl[0] = 1.0;
    basis[1] = cross_product(basis[2], basis[0]);
    VectorNormalize(&basis[0]); VectorNormalize(&basis[1]);
}

void renderuj_wireframe(FILE *plik, Plane *rzutnia, wezel *obiekty)
{
    setlocale(LC_ALL, "C"); // bo printf wypisuje standardowo przecinki zamiast kropek,
                            // co nie podoba sie postscriptowi...
    openPostscript(plik, 0.5);
    if(obiekty == NULL)
    {
        closePostscript(plik);
        return;
    }

    Vector basis[3]; // wektory bazowe dla nowego ukl. odniesienia O'X', O'Y', O'Z'
    transformuj_uklad(rzutnia, basis);

    Vector offset;
    Point3D srodek; srodek.wsp[0] = srodek.wsp[1] = srodek.wsp[2] = 0.0;
    Point3D srodek_prim = projectOrtho3D(srodek, rzutnia);
    Point2D linia[2]; Point3D linia3D[2];
    int kind;
    do
    {
        kind = obiekty->wartosc.kind;
        if(kind == 0)
        {
            linia3D[0] = projectOrtho3D(obiekty->wartosc.V[0], rzutnia);
            for(int i = 0; i < 3; i++)
                offset.skl[i] = linia3D[0].wsp[i] - srodek_prim.wsp[i];
            for(int i = 0; i < 2; i++)
                linia[0].wsp[i] = scalar_product(basis[i], offset);
            newPath(plik); moveTo(plik, linia[0]); makeDot(plik);
        }
        if(kind >=1)
        {
            for(int i = 0; i < 2; i++)
                linia3D[i] = projectOrtho3D(obiekty->wartosc.V[i], rzutnia);
            for(int i = 0; i < 2; i++)
            {
                for(int j = 0; j < 3; j++)
                    offset.skl[j] = linia3D[i].wsp[j] - srodek_prim.wsp[j];
                for(int k = 0; k < 2; k++)
                    linia[i].wsp[k] = scalar_product(basis[k], offset);
            }
            newPath(plik); moveTo(plik, linia[0]); lineTo(plik, linia[1]);
            closePathStroke(plik);
        }
        if(kind >=2)
        {
            linia3D[0] = projectOrtho3D(obiekty->wartosc.V[2], rzutnia);
            for(int i = 0; i < 2; i++)
            {
                linia3D[1] = projectOrtho3D(obiekty->wartosc.V[i], rzutnia);
                for(int z = 0; z < 2; z++)
                {
                    for(int j = 0; j < 3; j++)
                        offset.skl[j] = linia3D[z].wsp[j] - srodek_prim.wsp[j];
                    for(int k = 0; k < 2; k++)
                        linia[z].wsp[k] = scalar_product(basis[k], offset);
                }
                newPath(plik); moveTo(plik, linia[0]); lineTo(plik, linia[1]);
                closePathStroke(plik);
            }
        }
        if(kind >= 3)
        {
            linia3D[0] = projectOrtho3D(obiekty->wartosc.V[3], rzutnia);
            for(int i = 0; i < 3; i++)
            {
                linia3D[1] = projectOrtho3D(obiekty->wartosc.V[i], rzutnia);
                for(int z = 0; z < 2; z++)
                {
                    for(int j = 0; j < 3; j++)
                        offset.skl[j] = linia3D[z].wsp[j] - srodek_prim.wsp[j];
                    for(int k = 0; k < 2; k++)
                        linia[z].wsp[k] = scalar_product(basis[k], offset);
                }
                newPath(plik); moveTo(plik, linia[0]); lineTo(plik, linia[1]);
                closePathStroke(plik);
            }
        }
    } while((obiekty = obiekty->nast) != NULL);
    closePostscript(plik);
}


void rysuj_polygons(lista polygons, Point3D* srodek_rzutni, Plane *rzutnia, Vector basis[3], FILE *plik)
{
    int kind;
    Vector offset;
    Point2D linia[2]; Point3D linia3D[2];
    do
    {
        kind = polygons->wartosc.kind;
        if(kind == 0)
        {
            linia3D[0] = projectOrtho3D(polygons->wartosc.V[0], rzutnia);
            for(int i = 0; i < 3; i++)
                offset.skl[i] = linia3D[0].wsp[i] - srodek_rzutni->wsp[i];
            for(int i = 0; i < 2; i++)
                linia[0].wsp[i] = scalar_product(basis[i], offset);
            newPath(plik); moveTo(plik, linia[0]); makeDot(plik);
        }
        else
        {
            linia3D[0] = projectOrtho3D(polygons->wartosc.V[0], rzutnia);
            for(int i = 0; i < 3; i++)
                offset.skl[i] = linia3D[0].wsp[i] - srodek_rzutni->wsp[i];
            for(int i = 0; i < 2; i++)
                linia[0].wsp[i] = scalar_product(basis[i], offset);

            newPath(plik); moveTo(plik, linia[0]);

            for(int i = 1; i <= kind; i++)
            {
                linia3D[1] = projectOrtho3D(polygons->wartosc.V[i], rzutnia);
                for(int j = 0; j < 3; j++)
                    offset.skl[j] = linia3D[1].wsp[j] - srodek_rzutni->wsp[j];
                for(int j = 0; j < 2; j++)
                    linia[1].wsp[j] = scalar_product(basis[j], offset);

                lineTo(plik, linia[1]);

                linia3D[0] = linia3D[1]; linia[0] = linia[1];
            }
            RGB kolor;
            kolor.r = (rand() % 33) / 33.0; kolor.g = (rand() % 33) / 33.0; kolor.b = (rand() % 33) / 33.0;
            kolor.r = 0.15 + kolor.r * 0.85; kolor.g = 0.15 + kolor.g * 0.85; kolor.b = 0.15 + kolor.b * 0.85;

            closePath(plik); PathFillStroke(plik, kolor);
        }
    } while((polygons = polygons->nast) != NULL);
}

void renderuj_hidden_face_removal(FILE *plik, Plane *rzutnia, wezel *obiekty)
{
    setlocale(LC_ALL, "C"); // bo printf wypisuje standardowo przecinki zamiast kropek,
                            // co nie podoba sie postscriptowi...
    openPostscript(plik, 0.5);
    if(obiekty == NULL)
    {
        closePostscript(plik);
        return;
    }

    Vector basis[3]; // wektory bazowe dla nowego ukl. odniesienia O'X', O'Y', O'Z'
    transformuj_uklad(rzutnia, basis);

    Point3D srodek; srodek.wsp[0] = srodek.wsp[1] = srodek.wsp[2] = 0.0;
    Point3D srodek_prim = projectOrtho3D(srodek, rzutnia);
    polygon face; lista faces = NULL; lista obiekty_copy = obiekty;
    int kind;
    do
    {
        kind = obiekty_copy->wartosc.kind;
        if(kind <= 2)
        {
            face = obiekty_copy->wartosc;
            faces = wstaw_na_pocz(faces, face);
        }
        else
        {
            face.V[0] = obiekty_copy->wartosc.V[3];
            for(int i = 0; i < 3; i++)
            {
                face.V[1] = obiekty_copy->wartosc.V[i];
                face.V[2] = obiekty_copy->wartosc.V[(i+1) % 3];
                face.kind = 2;
                faces = wstaw_na_pocz(faces, face);
            }
            for(int i = 0; i < 3; i++)
            {
                face.V[i] = obiekty_copy->wartosc.V[i];
            }
            face.kind = 2;
            faces = wstaw_na_pocz(faces, face);
        }
    } while((obiekty_copy = obiekty_copy->nast) != NULL);

    BSP *tree = NULL;
    tree = buduj_BSP(tree, &faces);
    Point3D *srodek_rzutni = &srodek_prim;
    rysuj_BSP(tree, srodek_rzutni, rzutnia, basis, plik);
    kasuj_BSP(tree);
    closePostscript(plik);
}
