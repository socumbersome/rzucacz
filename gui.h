#ifndef _GUI_H_
#define _GUI_H_

#include <gtk/gtk.h>
#include <stdlib.h>

extern GtkWidget *lista_obj_gui;
extern char *nazw_obj[];
extern GtkWidget *radio1; extern GtkWidget *radio2;
extern GtkWidget *radio1metoda; extern GtkWidget *radio2metoda;
extern GtkWidget *param[];
extern GtkEntryBuffer *param_buf[];
extern GtkWidget *pkty[3][3];
extern GtkEntryBuffer *pkty_buf[3][3];

void kill(GtkButton *button, GtkWidget *window);

void pokaz_oprogramie(GtkWidget *widget, gpointer data);

void pokaz_error(char *tekst);

void pokaz_info(char *tekst);

void odpowiedz(GtkDialog *dialog, gint arg1, gpointer data);

void pokaz_pytanie(char *tytul, char *tekst, gpointer odp);

void napraw_wejscie(char **wart, int indeks);

void stworz_obj_et2(GtkWidget *widget, GtkWidget *dialog);

void stworz_obj(GtkWidget *widget, gpointer data);

void usun_obj(GtkWidget *widget, gpointer data);

void tworz_liste_obj(GtkWidget *list);

void gen_rzut(GtkWidget *widget, gpointer data);

#endif
