/* postscript.c - generowanie prostych plików postscriptowych */

#include "postscript.h"
#include "geometry.h"

static double width = 1;

void openPostscript(FILE* plik, double lineWidth)
{
    if (lineWidth<=0) lineWidth = 1;
    if (lineWidth>15) lineWidth = 15;
    width = lineWidth;
    fprintf(plik,
        "%%!PS\n" \
        "%%%%Orientation: Portrait\n" \
        "%%%%BoundingBox: 0 0 595 842\n" \
        "%%%%DocumentPaperSizes: a4\n" \
        "0.0 0.0 moveto\n" \
        "%f setlinewidth\n", width);
}

void newPath(FILE* plik)
{
    fprintf(plik, "newpath\n");
}

void closePath(FILE* plik)
{
    fprintf(plik, "closepath\n");
}

void closePathStroke(FILE* plik)
{
    fprintf(plik, "closepath\n" \
        "stroke\n");
}

void PathFillStroke(FILE* plik, const RGB kolor)
{
    fprintf(plik,
        "gsave\n %.2f %.2f %.2f setrgbcolor\n" \
        "fill\ngrestore\n"
        "stroke\n", kolor.r, kolor.g, kolor.b);
}

void PathFill(FILE* plik, const RGB kolor)
{
    fprintf(plik,
        "gsave\n %.2f %.2f %.2f setrgbcolor\n" \
        "fill\ngrestore\n", kolor.r, kolor.g, kolor.b);
}

void closePostscript(FILE* plik)
{
    fprintf(plik,
        "showpage\n" \
        "%%%%Trailer\n" \
        "%%EOF\n");
}

static const double mm2pt = 72.0/25.4;
static const double centerX = 105.0;
static const double centerY = 148.5;

void moveTo(FILE* plik, const Point2D P)
{
    fprintf(plik, "%.2f %.2f moveto\n", mm2pt*(P.wsp[0]+centerX), mm2pt*(P.wsp[1]+centerY));
}

void relMoveTo(FILE* plik, const Point2D P)
{
    fprintf(plik, "%.2f %.2f rmoveto\n", mm2pt*P.wsp[0], mm2pt*P.wsp[1]);
}
void makeDot(FILE* plik)
{
    fprintf(plik, "1 setlinecap\n" \
        "3 setlinewidth\n" \
        "0 0 rlineto\n");
    closePathStroke(plik);
    fprintf(plik, "0 setlinecap\n" \
        "1 setlinewidth\n");
}

void lineTo(FILE* plik, const Point2D P)
{
    fprintf(plik, "%.2f %.2f lineto\n", mm2pt*(P.wsp[0]+centerX), mm2pt*(P.wsp[1]+centerY));
}

void dash(FILE* plik)
{
    fprintf(plik, "[10] 4 setdash\n");
}

void undash(FILE* plik)
{
    fprintf(plik, "[] 0 setdash\n");
}

void relLineTo(FILE* plik, const Point2D P)
{
    fprintf(plik, "%.2f %.2f rlineto\n", mm2pt*P.wsp[0], mm2pt*P.wsp[1]);
}

const RGB
    black   = {.r=0.0, .g=0.0, .b=0.0},
    red     = {.r=1.0, .g=0.0, .b=0.0},
    green   = {.r=0.0, .g=1.0, .b=0.0},
    blue    = {.r=0.0, .g=0.0, .b=1.0},
    cyan    = {.r=0.0, .g=1.0, .b=1.0},
    magenta = {.r=1.0, .g=0.0, .b=1.0},
    yellow  = {.r=1.0, .g=1.0, .b=0.0},
    gray    = {.r=0.5, .g=0.5, .b=0.5};

void setColor(FILE* plik, const RGB rgb)
{
    if (rgb.r<0||rgb.r>1||rgb.g<0||rgb.g>1||rgb.b<0||rgb.b>1) return;
    fprintf(plik,
        "currentpoint\n" \
        "stroke\n" \
        "newpath\n" \
        "moveto\n" \
        "%d setlinewidth\n" \
        "%.2f %.2f %.2f setrgbcolor\n", width, rgb.r, rgb.g, rgb.b);
}

