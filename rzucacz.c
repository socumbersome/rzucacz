/* **********************************************************************************************
 * Patryk Kajdas, 258311

 *                                  Rzucacz
 *
 * Program pozwala na tworzenie obiektów typu: punkt, odcinek, trójk¹t, czworoœcian
 * w kartezjañskim uk³adzie wspó³rzêdnych OXYZ oraz rysowanie rzutu ortogonalnego utworzonych
 * obiektów na p³aszczyznê (rzutniê) podan¹ przez u¿ytkownika (nale¿y zwróciæ uwagê, ¿e
 * p³aszczyzny s¹ skierowane, tj. kierunek, w którym patrzy "obserwator" jest zgodny z kierunkiem
 * wektora normalnego (normalnej) rzutni) do pliku z rozszerzeniem .ps (postscriptowego).
 * Przyjête za³o¿enia:
 * - œrodek kartki formatu A4 zawsze pokrywa siê z rzutem œrodka O uk³. wsp. OXYZ
 *   na p³aszczyznê rzutni.
 * - orientacja osi O'X' i O'Y' na rzutni jest arbitralnie ustalana przez program, gdyz
 *   w przeciwnym przypadku nie jest ona jednoznaczna.
 * - wspó³rzêdne sa podawane w milimetrach, wiêc prawy górny róg kartki ma
 *   wspó³rzêdne (105.0 ; 148.5) w uk³adzie O'X'Y'.
 * Dostêpne s¹ dwie metody rysowania:
   - szkieletu (siatki) obiektów sk³adaj¹cych siê na scenê
   - wype³nionych (losowymi) kolorami œcian z uwzglêdniem ich wzajemnego
     zas³aniania siê oraz przecinania

 * Opis metody rozwi¹zania problemu:
 * - W przypadku rysowania siatki problemu w³aœciwie nie ma - program iteracyjnie rysuje
 *   wszystkie krawêdzie znajduj¹ce siê w scenie.
 * - Zaœ do poprawnego rysowania zamalowanych œcian u¿yto po³¹czenia algorytmu malarza z
 *   drzewami binarnego podzia³u przestrzeni (BSP Tree od binary space partition ).
 *   Dok³adniej, za pomoc¹ drzewa BSP program dzieli obiekty w scenie tak, by dla dowolnych
 *   dwóch œcianek F i G relacja "F znajduje siê w ca³oœci przed G, albo w ca³oœci za G"
 *   by³a dobrze zdefiniowana. Skutkiem ubocznym takiego podzia³u jest czêste dzielenie œcianek
 *   na mniejsze kawa³ki i, w efekcie, widocznoœæ krawêdzi ciêcia na gotowym rysunku
 *   (to jest jeden z powodów, dla którego rysunek jest wtedy kolorowy - w przeciwnym
 *    przypadku mog³oby byæ bardzo trudno stwierdziæ, "który obiekt jest który").
 *   Dalej, dziêki strukturze drzew BSP, mo¿liwe jest wykonanie algorytmu malarza poprzez przejœcie
 *   przez dane drzewo w odpowiednim porz¹dku (zale¿nym od tego, czy kierunek patrzenia
 *   obserwatora jest zgodny z normaln¹ w³aœnie rozpatrywanej p³aszczyzny podzia³u, czy nie).

 * U¿ywane struktury danych:
 * - Wspomniane ju¿ drzewo BSP
 * - Lista jednokierunkowa (w niej program trzyma wprowadzane przez u¿ytkownika obiekty)
 * - Struktura GenericObj reprezentuj¹ca pojedynczy obiekt jako tablicê wierzcho³ków
 *   oraz zmienn¹ okreœlaj¹c¹ rodzaj obiektu

 * Bibliografia:
 * - ftp://ftp.sgi.com/other/bspfaq/faq/bspfaq.html
 * - www.cns.gatech.edu/~predrag/courses/PHYS-4421-10/Lautrup/space.pdf
************************************************************************************************/

#include <gtk/gtk.h>
#include <stdlib.h>
#include "gui.h"

int main (int argc, char *argv[])
{
    GtkWidget *window, *obszar, *label_i;

	gtk_init(&argc, &argv);
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_widget_set_size_request (window, 650, 480);
	gtk_window_set_resizable(GTK_WINDOW(window), FALSE);
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_window_set_title(GTK_WINDOW(window), "Rzucacz");
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);

    obszar = gtk_fixed_new();
    label_i = gtk_label_new("Dodaj obiekt:");
    gtk_fixed_put(GTK_FIXED(obszar), label_i, 0, 0);
    gtk_widget_show(label_i);
    extern char *nazw_obj[];
    GtkWidget *obj[4];
    for(int i = 0; i < 4; i++)
    {
        obj[i] = gtk_button_new_with_label((gchar*)nazw_obj[i]);
        gtk_widget_set_size_request(obj[i], 80, 30);
        g_signal_connect(G_OBJECT(obj[i]), "clicked", G_CALLBACK(stworz_obj), (gpointer) i);
        gtk_fixed_put(GTK_FIXED(obszar), obj[i], 90*i, 20);
        gtk_widget_show(obj[i]);
    }
    label_i = gtk_label_new("Lista utworzonych obiektow:");
    gtk_fixed_put(GTK_FIXED(obszar), label_i, 0, 60);
    gtk_widget_show(label_i);

    GtkWidget *oprogramie = gtk_button_new_with_label("O programie");
    gtk_widget_set_size_request(oprogramie, 120, 30);
    g_signal_connect(G_OBJECT(oprogramie), "clicked", G_CALLBACK(pokaz_oprogramie), NULL);
    gtk_fixed_put(GTK_FIXED(obszar), oprogramie, 410, 20);
    gtk_widget_show(oprogramie);

    GtkWidget *sw = gtk_scrolled_window_new(NULL, NULL);
    extern GtkWidget *lista_obj_gui;
    lista_obj_gui = gtk_tree_view_new();
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW(sw), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW(sw), GTK_SHADOW_ETCHED_IN);
    gtk_tree_view_set_headers_visible(GTK_TREE_VIEW (lista_obj_gui), FALSE);
    gtk_widget_set_size_request(sw, 450, 120);
    gtk_container_add(GTK_CONTAINER(sw), lista_obj_gui);
    gtk_fixed_put(GTK_FIXED(obszar), sw, 0, 80);
    gtk_widget_show(lista_obj_gui);
    gtk_widget_show(sw);

    tworz_liste_obj(lista_obj_gui);
    GtkTreeSelection *selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(lista_obj_gui));

    GtkWidget *usun_zazn = gtk_button_new_with_label("     Usun\nzaznaczony");
    gtk_widget_set_size_request(usun_zazn, 70, 50);
    g_signal_connect(G_OBJECT(usun_zazn), "clicked", G_CALLBACK(usun_obj), selection);
    gtk_fixed_put(GTK_FIXED(obszar), usun_zazn, 460, 115);
    gtk_widget_show(usun_zazn);

    label_i = gtk_label_new("Wybierz metode ustalenia rzutni:");
    gtk_fixed_put(GTK_FIXED(obszar), label_i, 0, 210);
    gtk_widget_show(label_i);

    extern GtkWidget *radio1; extern GtkWidget *radio2;
    radio1 = gtk_radio_button_new_with_label(NULL, "podanie parametrow A, B, C, D dla rownania Ax + By + Cz + D = 0");
    radio2 = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(radio1), "podanie 3 niewspolliniowych punktow" \
                                                         " P, Q, R przez ktore przechodzi plaszczyzna rzutni");
    gtk_fixed_put(GTK_FIXED(obszar), radio1, 0, 230);
    gtk_widget_show(radio1);
    extern GtkWidget *param[];
    extern GtkEntryBuffer *param_buf[];
    char lit = 'A';
    for(int i = 0; i < 4; i++)
    {
        char temp[2];
        temp[0] = lit + i; temp[1] = '\0';
        label_i = gtk_label_new((gchar*)temp);
        gtk_fixed_put(GTK_FIXED(obszar), label_i, 90*i, 260);
        gtk_widget_show(label_i);
        param_buf[i] = gtk_entry_buffer_new(NULL, -1);
        param[i] = gtk_entry_new_with_buffer(param_buf[i]);
        gtk_fixed_put(GTK_FIXED(obszar), param[i], 90*i+20, 260);
        gtk_widget_set_size_request(param[i], 50, 20);
        gtk_widget_show(param[i]);
    }
    gtk_fixed_put(GTK_FIXED(obszar), radio2, 0, 290);
    gtk_widget_show(radio2);

    extern GtkWidget *pkty[3][3];
    extern GtkEntryBuffer *pkty_buf[3][3];
    lit = 'P'; char lit2 = 'x';
    for(int i = 0; i < 3; i++)
    {
        char temp[4]; temp[0] = lit + i; temp[1] = ' '; temp[2] = ':'; temp[3] = '\0';
        label_i = gtk_label_new((gchar*)temp);
        gtk_fixed_put(GTK_FIXED(obszar), label_i, 5, 320+25*i);
        gtk_widget_show(label_i);
        for(int j = 0; j < 3; j++)
        {
            temp[0] = lit2 + j; temp[1] = '\0';
            label_i = gtk_label_new((gchar*)temp);
            gtk_fixed_put(GTK_FIXED(obszar), label_i, 100*j+40, 320+25*i);
            gtk_widget_show(label_i);
            pkty_buf[i][j] = gtk_entry_buffer_new("", -1);
            pkty[i][j] = gtk_entry_new_with_buffer(pkty_buf[i][j]);
            gtk_fixed_put(GTK_FIXED(obszar), pkty[i][j], 100*j+55, 320+25*i);
            gtk_widget_set_size_request(pkty[i][j], 50, 20);
            gtk_widget_show(pkty[i][j]);
        }
    }

    label_i = gtk_label_new("Wybierz metode rysowania rzutu:");
    gtk_fixed_put(GTK_FIXED(obszar), label_i, 340, 320);
    gtk_widget_show(label_i);
    extern GtkWidget *radio1metoda; extern GtkWidget *radio2metoda;
    radio1metoda = gtk_radio_button_new_with_label(NULL, "Szkielet (siatka)");
    radio2metoda = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(radio1metoda), "Zakrywanie tylnych scianek");
    gtk_fixed_put(GTK_FIXED(obszar), radio1metoda, 340, 340);
    gtk_widget_show(radio1metoda);
    gtk_fixed_put(GTK_FIXED(obszar), radio2metoda, 340, 360);
    gtk_widget_show(radio2metoda);

    label_i = gtk_label_new("Wpisz nazwe pliku, do ktorego zostanie zapisany wynikowy rzut w formacie .ps");
    gtk_fixed_put(GTK_FIXED(obszar), label_i, 0, 410);
    gtk_widget_show(label_i);

    GtkEntryBuffer *nazwa_plik_buf = gtk_entry_buffer_new("", -1);
    GtkWidget *nazwa_plik = gtk_entry_new_with_buffer(nazwa_plik_buf);
    gtk_fixed_put(GTK_FIXED(obszar), nazwa_plik, 0, 430);
    gtk_widget_set_size_request(nazwa_plik, 350, 20);
    gtk_widget_show(nazwa_plik);

    GtkWidget *gen_button = gtk_button_new_with_label("Generuj rzut");
    gtk_widget_set_size_request(gen_button, 100, 50);
    g_signal_connect(G_OBJECT(gen_button), "clicked", G_CALLBACK(gen_rzut), (gpointer) nazwa_plik_buf);
    gtk_fixed_put(GTK_FIXED(obszar), gen_button, 430, 400);
    gtk_widget_show(gen_button);

	g_signal_connect(G_OBJECT(window),"delete_event",G_CALLBACK(gtk_main_quit),NULL);
	g_signal_connect(G_OBJECT(window), "destroy",G_CALLBACK(gtk_main_quit), NULL);

    gtk_container_add(GTK_CONTAINER(window), obszar);
	gtk_widget_show(obszar);
	gtk_widget_show(window);

    gtk_main();

	return 0;
}
