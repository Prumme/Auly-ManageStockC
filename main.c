#include <stdio.h>
#include <gtk-3.0/gtk/gtk.h>
#include <gtk/gtk.h>
#include <curl/curl.h>
#include "window.h"

#include "mysql.h"

    void selectMysql();
    void finish_with_error(MYSQL *con)
    {
        fprintf(stderr, "%s\n", mysql_error(con));
        mysql_close(con);
        exit(1);
    }

    int
    main(int argc, char *argv [])
    {
        printf("\n\nDébut du programme, ayoooooo !\n\n");
        selectMysql();
        GtkWidget *fenetre_principale = NULL;
        GtkBuilder *builder = NULL;
        GError *error = NULL;
        gchar *filename = NULL;
        /* Initialisation de la librairie Gtk. */
        gtk_init(&argc, &argv);

        /* Ouverture du fichier Glade de la fenêtre principale */
        builder = gtk_builder_new();

        /* Création du chemin complet pour accéder au fichier test.glade. */
        /* g_build_filename(); construit le chemin complet en fonction du système */
        /* d'exploitation. ( / pour Linux et \ pour Windows) */
        filename =  g_build_filename ("/home/aurelien/Bureau/project C/Auly-ManageStockC-main/window_main.glade", NULL);

        /* Chargement du fichier window_main.glade */
        gtk_builder_add_from_file (builder, filename, &error);
        g_free (filename);
        if (error)
        {
            gint code = error->code;
            g_printerr("%s\n", error->message);
            g_error_free (error);
            return code;
        }

        /* Récupération du pointeur de la fenêtre principale */
        fenetre_principale = GTK_WIDGET(gtk_builder_get_object (builder, "MainWindow"));

        /* Affectation du signal "destroy" à la fonction gtk_main_quit(); pour la */
        /* fermeture de la fenêtre. */
        g_signal_connect (G_OBJECT (fenetre_principale), "destroy", (GCallback)gtk_main_quit, NULL);

        /* Affichage de la fenêtre principale. */
        gtk_widget_show_all (fenetre_principale);

        gtk_main();

        printf("\n\nFin du programme ;) Byeeeeeee \n\n");
        return 0;
    }

void selectMysql() {
    MYSQL *con = mysql_init(NULL);

    if (con == NULL) {
        fprintf(stderr, "%s\n", mysql_error(con));
        exit(1);
    }

    if (mysql_real_connect(con, "localhost", "esgi", "esgi",
                           "test", 0, NULL, 0) == NULL) {
        fprintf(stderr, "%s\n", mysql_error(con));
        mysql_close(con);
        exit(1);
    }

    /*if (mysql_query(con, "CREATE DATABASE testdb")) {
        fprintf(stderr, "%s\n", mysql_error(con));
        mysql_close(con);
        exit(1);
    }*/

    if (mysql_query(con, "SELECT * FROM Product"))
    {
        finish_with_error(con);
    }

    MYSQL_RES *result = mysql_store_result(con);

    if (result == NULL)
    {
        finish_with_error(con);
    }

    int num_fields = mysql_num_fields(result);

    MYSQL_ROW row;

    while ((row = mysql_fetch_row(result)))
    {
        for(int i = 0; i < num_fields; i++)
        {
            printf("%s ", row[i] ? row[i] : "NULL");
        }

        printf("\n");
    }

    mysql_free_result(result);

    printf("MySQL client version: %s\n", mysql_get_client_info());


    mysql_close(con);
}