#include <string.h>
#include "gui.h"
#include "renderer.h"

#define MAX_OPIS 200

static int nr;
lista lista_obj;
GtkWidget *lista_obj_gui;
char *nazw_obj[] = {"Punkt", "Odcinek", "Trojkat", "Czworoscian"};
GtkWidget *wsp_new_obj[4][3];
GtkEntryBuffer *wsp_new_obj_buf[4][3];
GtkWidget *radio1, *radio2;
GtkWidget *radio1metoda, *radio2metoda;
GtkWidget *param[4];
GtkEntryBuffer *param_buf[4];
GtkWidget *pkty[3][3];
GtkEntryBuffer *pkty_buf[3][3];

void kill(GtkButton *button, GtkWidget *window)
{
    gtk_widget_destroy(window);
}

void pokaz_oprogramie(GtkWidget *widget, gpointer data)
{
    GtkWidget *dialog = gtk_about_dialog_new();

    gtk_about_dialog_set_name(GTK_ABOUT_DIALOG(dialog), "Rzucacz");
    gtk_about_dialog_set_version(GTK_ABOUT_DIALOG(dialog), "v0.15");
    gtk_about_dialog_set_copyright(GTK_ABOUT_DIALOG(dialog), "Patryk Kajdas");
    gtk_about_dialog_set_comments(GTK_ABOUT_DIALOG(dialog),
    "Srodek kartki formatu A4 pokrywa sie z rzutem srodka O ukladu wspolrzednych OXYZ na plaszczyzne rzutni.\n" \
    "Orientacja osi O'X' i O'Y' na rzutni jest arbitralnie\n ustalana przez program, gdyz w przeciwnym przypadku\n nie jest ona jednoznaczna.\n" \
    "Wspolrzedne sa podawane w milimetrach.\n Zatem prawy gorny rog kartki ma\n wspolrzedne (105.0 ; 148.5) w ukladzie O'X'Y'.");
    gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER);
    gtk_dialog_run(GTK_DIALOG (dialog));
    gtk_widget_destroy(dialog);
}

void pokaz_error(char *tekst)
{
    GtkWidget *dialog;
    dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_DESTROY_WITH_PARENT,
        GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, (gchar*)tekst);
    gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER);
    gtk_window_set_title(GTK_WINDOW(dialog), "Error");
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

void pokaz_info(char *tekst)
{
    GtkWidget *dialog;
    dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_DESTROY_WITH_PARENT,
        GTK_MESSAGE_INFO, GTK_BUTTONS_OK, (gchar*)tekst);
    gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER);
    gtk_window_set_title(GTK_WINDOW(dialog), "Informacja");
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

void odpowiedz(GtkDialog *dialog, gint arg1, gpointer data)
{
	int *taknie = (int *) data;
	if(arg1 == GTK_RESPONSE_YES)
		*taknie = 1;
    else
        *taknie = 0;
}

void pokaz_pytanie(char *tytul, char *tekst, gpointer odp)
{
    GtkWidget *dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_DESTROY_WITH_PARENT,
        GTK_MESSAGE_QUESTION, GTK_BUTTONS_YES_NO, (gchar*)tekst);
    gtk_window_set_title(GTK_WINDOW(dialog), (gchar*)tytul);
    gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER);
    g_signal_connect(G_OBJECT(dialog), "response", G_CALLBACK(odpowiedz), odp);
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

void napraw_wejscie(char **wart, int indeks)
{
    char *temp = wart[indeks];
    while(*temp != '\0' && *temp != '.') // uniezalezniamy sie od konwencji zapisu , lub . w liczbie niecalkowitej
        temp++;
    if((*temp) == '.')
        *temp = ',';
}

void stworz_obj_et2(GtkWidget *widget, GtkWidget *dialog)
{
    GenericObj nowy_twor;
    nowy_twor.kind = nr;
    double wart_xyz[4][3];
    char **wart = calloc(4*3, sizeof(char *));
    for(int i = 0; i < (nr+1)*3; i++)
    {
        wart[i] = calloc(gtk_entry_buffer_get_length(wsp_new_obj_buf[i/3][i-3*(i/3)])+2, sizeof(char));
        strcpy(wart[i], gtk_entry_buffer_get_text(wsp_new_obj_buf[i/3][i-3*(i/3)]));
        napraw_wejscie(wart, i);
        wart_xyz[i/3][i-3*(i/3)] = strtod(wart[i], NULL);
    }
    for(int i = 0; i <= nr; i++)
        for(int j = 0; j < 3; j++)
        {
            if(wart_xyz[i][j] == 0.0 && wart[3*i+j][0] != '0')
            {
                pokaz_error("Blad danych wejsciowych");
                for(int k = 0; k < (nr+1)*3; k++)
                    free(wart[k]);
                free(wart);
                return;
            }
        }
    // udalo sie, dane poprawne
    for(int i = 0; i <= nr; i++)
        for(int j = 0; j < 3; j++)
            nowy_twor.V[i].wsp[j] = wart_xyz[i][j];

    lista_obj = wstaw_na_pocz(lista_obj, nowy_twor);

    char *str = calloc(MAX_OPIS, sizeof(char));
    if(nr == 0)
        sprintf(str, "%s {(%.1f ; %.1f ; %.1f)}", nazw_obj[nr], wart_xyz[0][0], wart_xyz[0][1], wart_xyz[0][2]);
    else if(nr == 1)
        sprintf(str, "%s {(%.1f ; %.1f ; %.1f) (%.1f ; %.1f ; %.1f)}", nazw_obj[nr], wart_xyz[0][0], wart_xyz[0][1], wart_xyz[0][2],
            wart_xyz[1][0], wart_xyz[1][1], wart_xyz[1][2]);
    else if(nr == 2)
        sprintf(str, "%s {(%.1f ; %.1f ; %.1f) (%.1f ; %.1f ; %.1f) (%.1f ; %.1f ; %.1f)}", nazw_obj[nr], wart_xyz[0][0], wart_xyz[0][1],
            wart_xyz[0][2], wart_xyz[1][0], wart_xyz[1][1], wart_xyz[1][2], wart_xyz[2][0], wart_xyz[2][1], wart_xyz[2][2]);
    else
        sprintf(str, "%s {(%.1f ; %.1f ; %.1f) (%.1f ; %.1f ; %.1f) (%.1f ; %.1f ; %.1f) (%.1f ; %.1f ; %.1f)}", nazw_obj[nr], wart_xyz[0][0],
            wart_xyz[0][1], wart_xyz[0][2], wart_xyz[1][0], wart_xyz[1][1], wart_xyz[1][2], wart_xyz[2][0], wart_xyz[2][1], wart_xyz[2][2],
            wart_xyz[3][0], wart_xyz[3][1], wart_xyz[3][2]);

    for(int i = 0; i < (nr+1)*3; i++)
        free(wart[i]);
    free(wart);

    GtkListStore *store;
    GtkTreeIter  iter;
    store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(lista_obj_gui)));

    wezel *akt_obiekt = lista_obj; // wstawialismy na poczatek, wiec lista_obj pokazuje na nasz nowy twor
    // ogolnie tu jest taki trik, ze oprocz stringa opisujacego stworzony obiekt na liscie w GUI trzymamy
    // tez wskaznik na ten obiekt, zeby potem mozna go bylo w ogole namierzyc i usunac
    gtk_list_store_append(store, &iter);
    gtk_list_store_set(store, &iter, 0, str, 1, akt_obiekt, -1);

    free(str);
    gtk_widget_destroy(dialog);
}

void stworz_obj(GtkWidget *widget, gpointer data)
{
    nr = GPOINTER_TO_INT(data);
    if(nr < 0 || nr > 3) // w zasadzie to sie chyba nigdy nie zdarzy
        return;          // ale przezorny zawsze ubezpieczony :)

    GtkWidget *dialog, *obszar, *label_i;
    dialog = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_widget_set_size_request (dialog, 330, 75+25*nr);
	gtk_window_set_resizable(GTK_WINDOW(dialog), FALSE);
	gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER);
    gtk_window_set_title(GTK_WINDOW(dialog), (gchar*)nazw_obj[nr]);
    obszar = gtk_fixed_new();
    char lit1 = 'A', lit2 = 'x';
    for(int i = 0; i <= nr; i++)
    {
        char temp[4]; temp[0] = lit1 + i; temp[1] = ' '; temp[2] = ':'; temp[3] = '\0';
        label_i = gtk_label_new((gchar*)temp);
        gtk_fixed_put(GTK_FIXED(obszar), label_i, 5, 10+25*i);
        gtk_widget_show(label_i);
        for(int j = 0; j < 3; j++)
        {
            temp[0] = lit2 + j; temp[1] = '\0';
            label_i = gtk_label_new((gchar*)temp);
            gtk_fixed_put(GTK_FIXED(obszar), label_i, 100*j+40, 10+25*i);
            gtk_widget_show(label_i);
            wsp_new_obj_buf[i][j] = gtk_entry_buffer_new("", -1);
            wsp_new_obj[i][j] = gtk_entry_new_with_buffer(wsp_new_obj_buf[i][j]);
            gtk_fixed_put(GTK_FIXED(obszar), wsp_new_obj[i][j], 100*j+55, 10+25*i);
            gtk_widget_set_size_request(wsp_new_obj[i][j], 50, 20);
            gtk_widget_show(wsp_new_obj[i][j]);
        }
    }
    GtkWidget *ok, *anuluj;
    ok = gtk_button_new_with_label("OK");
    gtk_widget_set_size_request(ok, 60, 30);
    g_signal_connect(G_OBJECT(ok), "clicked", G_CALLBACK(stworz_obj_et2), dialog);
    gtk_fixed_put(GTK_FIXED(obszar), ok, 70, 40+25*nr);
    gtk_widget_show(ok);
    anuluj = gtk_button_new_with_label("Anuluj");
    gtk_widget_set_size_request(anuluj, 60, 30);
    g_signal_connect(G_OBJECT(anuluj), "clicked", G_CALLBACK(kill), dialog);
    gtk_fixed_put(GTK_FIXED(obszar), anuluj, 200, 40+25*nr);
    gtk_widget_show(anuluj);
    gtk_container_add(GTK_CONTAINER(dialog), obszar);
	gtk_widget_show(obszar);
	gtk_widget_show(dialog);
}

void usun_obj(GtkWidget *widget, gpointer selection)
{
    GtkListStore *store;
    GtkTreeModel *model;
    GtkTreeIter  iter;

    store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW (lista_obj_gui)));
    model = gtk_tree_view_get_model(GTK_TREE_VIEW(lista_obj_gui));

    if(gtk_tree_model_get_iter_first(model, &iter) == FALSE)
      return;
    if(gtk_tree_selection_get_selected(GTK_TREE_SELECTION(selection), &model, &iter))
    {
        wezel *red;
        gtk_tree_model_get(model, &iter, 1, &red, -1);
        int poz = szukaj_wartosci(lista_obj, red->wartosc);
        if(poz != -1)
            lista_obj = usun_z_pozycji(lista_obj, poz);
        else
        {
            pokaz_error("Uwaga! Wyciek pamieci spowodowany nieusunieciem obiektu (chociaz uzytkownikowi zniknal on z oczu)!");
            // teoretycznie nigdy program nie powinien tutaj dojsc...
        }
        gtk_list_store_remove(store, &iter);
    }
}

void tworz_liste_obj(GtkWidget *list)
{
  GtkCellRenderer *renderer;
  GtkTreeViewColumn *column;
  GtkListStore *store;

  renderer = gtk_cell_renderer_text_new();
  column = gtk_tree_view_column_new_with_attributes("List Item", renderer, "text", 0, NULL);
  gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);
  store = gtk_list_store_new(2, G_TYPE_STRING, G_TYPE_POINTER);
  gtk_tree_view_set_model(GTK_TREE_VIEW(list), GTK_TREE_MODEL(store));
  g_object_unref(store);
}

void czysc_pamiec_radio(int radio1act, char **wart)
{
    if(radio1act)
        for(int j = 0; j < 4; j++)
            free(wart[j]);
    else
        for(int j = 0; j < 3*3; j++)
            free(wart[j]);
    free(wart);
}

void gen_rzut(GtkWidget *widget, gpointer nazwap)
{
    Plane *rzutnia = malloc(sizeof(Plane));
    char *nazwa_pliku = calloc(gtk_entry_buffer_get_length(nazwap)+4, sizeof(char));
    strcpy(nazwa_pliku, gtk_entry_buffer_get_text(nazwap));
    char *temp = nazwa_pliku;
    if(temp[0] == ' ' || temp[0] == '\0')
    {
        pokaz_error("Zla nazwa pliku");
        free(nazwa_pliku); free(rzutnia);
        return;
    }
    while((*temp) != '\0')
    {
        if((*temp) == '\\' || (*temp) == '/' || (*temp) == ':' || (*temp) == '*' ||
           (*temp) == '?' || (*temp) == '"' || (*temp) == '<' || (*temp) == '>' || (*temp) == '|')
        {
            pokaz_error("Zla nazwa pliku");
            free(nazwa_pliku); free(rzutnia);
            return;
        }
        temp++;
    }
    strcat(nazwa_pliku, ".ps");

    int radio1act = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(radio1));
    char **wart = NULL;
    if(radio1act)
    {
        double wart_abcd[4];
        wart = calloc(4, sizeof(char *));
        for(int i = 0; i < 4; i++)
        {
            wart[i] = calloc(gtk_entry_buffer_get_length(param_buf[i])+2, sizeof(char));
            strcpy(wart[i], gtk_entry_buffer_get_text(param_buf[i]));
            napraw_wejscie(wart, i);
            wart_abcd[i] = strtod(wart[i], NULL);
        }
        for(int i = 0; i < 4; i++)
            if(wart_abcd[i] == 0.0 && wart[i][0] != '0')
            {
                pokaz_error("Blad danych wejsciowych");
                for(int j = 0; j < 4; j++)
                    free(wart[j]);
                free(wart); free(nazwa_pliku); free(rzutnia);
                return;
            }

        if(wart_abcd[0] == 0.0 && wart_abcd[1] == 0.0 && wart_abcd[2] == 0.0)
        {
            pokaz_error("Parametry A, B, C nie moga byc jednoczesnie rowne 0");
            for(int j = 0; j < 4; j++)
                free(wart[j]);
            free(wart); free(nazwa_pliku); free(rzutnia);
            return;
        }
        for(int i = 0; i < 4; i++)
            rzutnia->prm[i] = wart_abcd[i];
    }
    else // radio2 aktywne
    {
        double wart_xyz[3][3];
        wart = calloc(3*3, sizeof(char *));
        for(int i = 0; i < 3*3; i++)
        {
            wart[i] = calloc(gtk_entry_buffer_get_length(pkty_buf[i/3][i-3*(i/3)])+2, sizeof(char));
            strcpy(wart[i], gtk_entry_buffer_get_text(pkty_buf[i/3][i-3*(i/3)]));
            napraw_wejscie(wart, i);
            wart_xyz[i/3][i-3*(i/3)] = strtod(wart[i], NULL);
        }
        for(int i = 0; i < 3; i++)
            for(int j = 0; j < 3; j++)
                if(wart_xyz[i][j] == 0.0 && wart[3*i+j][0] != '0')
                {
                    pokaz_error("Blad danych wejsciowych");
                    for(int k = 0; k < 3*3; k++)
                        free(wart[k]);
                    free(wart); free(nazwa_pliku); free(rzutnia);
                    return;
                }

        for(int i = 0; i < 3; i++)
            for(int j = 0; j < 3; j++)
                rzutnia->P[i].wsp[j] = wart_xyz[i][j];

        if(PointsIdentical(rzutnia->P[0], rzutnia->P[1]) || PointsIdentical(rzutnia->P[0], rzutnia->P[2]) ||
           PointsIdentical(rzutnia->P[1], rzutnia->P[2]) || PointsCollinear(rzutnia->P[0], rzutnia->P[1], rzutnia->P[2]))
        {
            pokaz_error("Przez podane punkty nie da sie jednoznacznie poprowadzic plaszczyzny rzutni");
            for(int k = 0; k < 3*3; k++)
                free(wart[k]);
            free(wart); free(nazwa_pliku); free(rzutnia);
            return;
        }

        ABCD_Plane(rzutnia); // liczymy reprezentacje normalna plaszczyzny rzutni
    }
    FILE *plik = NULL;
    plik = fopen(nazwa_pliku, "r");
    if(plik != NULL)
    {
        int odp = 0;
        pokaz_pytanie("Nadpisac?", "Plik o tej nazwie juz istnieje. Czy chcesz go nadpisac?", (gpointer) &odp);
        if(odp)
        {
            fclose(plik);
            plik = fopen(nazwa_pliku, "w");
            if(plik == NULL)
            {
                pokaz_error("Nie udalo sie stworzyc pliku");
                czysc_pamiec_radio(radio1act, wart);
                free(nazwa_pliku); free(rzutnia);
                return;
            }
        }
        else
        {
            czysc_pamiec_radio(radio1act, wart);
            free(nazwa_pliku); free(rzutnia);
            return;
        }
    }
    else
    {
        plik = fopen(nazwa_pliku, "w");
        if(plik == NULL)
        {
            pokaz_error("Nie udalo sie stworzyc pliku");
            czysc_pamiec_radio(radio1act, wart);
            free(nazwa_pliku); free(rzutnia);
            return;
        }
    }

    if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(radio1metoda)))
        renderuj_wireframe(plik, rzutnia, lista_obj);
    else
        renderuj_hidden_face_removal(plik, rzutnia, lista_obj);

    fclose(plik);
    pokaz_info("Rysowanie rzutu zakonczone powodzeniem");
    czysc_pamiec_radio(radio1act, wart);
    free(nazwa_pliku); free(rzutnia);
}
