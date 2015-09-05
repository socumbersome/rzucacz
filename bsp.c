#include "bsp.h"

int klasyfikuj_polygon(polygon poly, Plane partition)
{
    double wyn; int zprzodu, ztylu, na;
    zprzodu = ztylu = na = 0;
    for(int i = 0; i <= poly.kind; i++)
    {
        wyn = partition.prm[0]*poly.V[i].wsp[0] + partition.prm[1]*poly.V[i].wsp[1] + partition.prm[2]*poly.V[i].wsp[2] + partition.prm[3];

        if(wart_bezwzgl(wyn) < EPSILON)
            na++;
        else if(wyn > 0)
            zprzodu++;
        else
            ztylu++;
    }
    if(na == poly.kind+1)
        return ZAWIERA_SIE;
    if(zprzodu + na == poly.kind+1)
        return Z_PRZODU;
    if(ztylu + na == poly.kind+1)
        return Z_TYLU;
    return PRZECINA;
}

double klasyfikuj_punkt(Point3D* P, Plane* partition)
{
    double wyn;
    wyn = (partition->prm[0])*(P->wsp[0]) + (partition->prm[1])*(P->wsp[1]) + (partition->prm[2])*(P->wsp[2]) + partition->prm[3];

    if(wart_bezwzgl(wyn) < EPSILON)
        return 0.0;
    return wyn;
}

polygon nowy_polygon(Point3D points[], int v_count)
{
    polygon res;
    res.kind = v_count;
    for(int i = 0; i <= v_count; i++)
        res.V[i] = points[i];
    return res;
}

void dziel_polygon(polygon poly, Plane partition, polygon *przednia_czesc, polygon *tylna_czesc)
{
    int count = poly.kind;
    int out_c = 0, in_c = 0;
    Point3D ptA, ptB, outpts[MAXPTS], inpts[MAXPTS];
    double sideA, sideB;
    ptA = poly.V[count];
    sideA = klasyfikuj_punkt(&ptA, &partition);
    for(int i = 0; i <= count; i++)
    {
        ptB = poly.V[i];
        sideB = klasyfikuj_punkt(&ptB, &partition);
        if(sideB > EPSILON)
        {
            if(sideA < -EPSILON)
            {
                Vector V = makeVector(ptA, ptB);
                Vector partition_normal;
                for(int j = 0; j < 3; j++)
                    partition_normal.skl[j] = partition.prm[j];

                double sect = - klasyfikuj_punkt(&ptA, &partition) / scalar_product(partition_normal, V);
                Point3D temp = ptA;
                V = mnozVector(V, sect);
                translate_V(&temp, V);
                outpts[out_c++] = inpts[in_c++] = temp;
            }
            outpts[out_c++] = ptB;
        }
        else if(sideB < -EPSILON)
        {
            if(sideA > EPSILON)
            {
                Vector V = makeVector(ptA, ptB);
                Vector partition_normal;
                for(int j = 0; j < 3; j++)
                    partition_normal.skl[j] = partition.prm[j];

                double sect = - klasyfikuj_punkt(&ptA, &partition) / scalar_product(partition_normal, V);
                Point3D temp = ptA;
                V = mnozVector(V, sect);
                translate_V(&temp, V);
                outpts[out_c++] = inpts[in_c++] = temp;
            }
            inpts[in_c++] = ptB;
        }
        else
            outpts[out_c++] = inpts[in_c++] = ptB;
        ptA = ptB;
        sideA = sideB;
    }
    *przednia_czesc = nowy_polygon(outpts, out_c-1); // zeby zachowac konwencje, ze jak kind=n, to ma n+1 wierzch
    *tylna_czesc = nowy_polygon(inpts, in_c-1);
}

BSP* buduj_BSP(BSP *tree, lista *polygons)
{
    tree = malloc(sizeof(struct bsp_tree));
    tree->front = tree->back = NULL;
    tree->polygons = NULL;
    polygon *root = &((*polygons)->wartosc);
    tree->partition = prowadz_plaszczyzne(root);
    tree->polygons = wstaw_na_pocz(tree->polygons, *root);

    lista front_list = NULL; lista back_list = NULL;
    polygon poly;
    while((*polygons) != NULL)
    {
        poly = (*polygons)->wartosc;
        *polygons = usun_nowe(*polygons, *polygons);
        int zaleznosc = klasyfikuj_polygon(poly, tree->partition);
        switch(zaleznosc)
        {
            case ZAWIERA_SIE:
            {
                tree->polygons = wstaw_na_pocz(tree->polygons, poly);
                break;
            }
            case Z_TYLU:
            {
                back_list = wstaw_na_pocz(back_list, poly);
                break;
            }
            case Z_PRZODU:
            {
                front_list = wstaw_na_pocz(front_list, poly);
                break;
            }
            case PRZECINA:
            {
                polygon przednia_czesc; polygon tylna_czesc;
                dziel_polygon(poly, tree->partition, &przednia_czesc, &tylna_czesc);
                back_list = wstaw_na_pocz(back_list, tylna_czesc);
                front_list = wstaw_na_pocz(front_list, przednia_czesc);
                break;
            }
        }
    }
    if(front_list != NULL)
        tree->front = buduj_BSP(tree->front, &front_list);

    if(back_list != NULL)
        tree->back = buduj_BSP(tree->back, &back_list);

    return tree;
}

void rysuj_BSP(BSP *tree, Point3D *srodek_rzutni, Plane *rzutnia, Vector basis[3], FILE *plik)
{
    if(tree == NULL)
        return;

    Plane *podzialka = &(tree->partition);
    Vector kier_rzutni, kier_scianki;
    for(int i = 0; i < 3; i++)
    {
        kier_rzutni.skl[i] = rzutnia->prm[i];
        kier_scianki.skl[i] = podzialka->prm[i];
    }

    double side = scalar_product(kier_rzutni, kier_scianki);

    if(side < 0)
    {
        rysuj_BSP(tree->back, srodek_rzutni, rzutnia, basis, plik);
        rysuj_polygons(tree->polygons, srodek_rzutni, rzutnia, basis, plik);
        rysuj_BSP(tree->front, srodek_rzutni, rzutnia, basis, plik);
    }
    else
    {
        rysuj_BSP(tree->front, srodek_rzutni, rzutnia, basis, plik);
        rysuj_polygons(tree->polygons, srodek_rzutni, rzutnia, basis, plik);
        rysuj_BSP(tree->back, srodek_rzutni, rzutnia, basis, plik);
    }
}

void kasuj_BSP(BSP *tree)
{
    if(tree == NULL)
        return;
    niszcz_liste(tree->polygons);
    if(tree->back != NULL)
        kasuj_BSP(tree->back);
    if(tree->front != NULL)
        kasuj_BSP(tree->front);
    free(tree);
}
