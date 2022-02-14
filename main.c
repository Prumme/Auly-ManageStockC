#include <stdio.h>
#include <gtk/gtk.h>
#include <curl/curl.h>
#include <curl/easy.h>
#include "window.h"
#include <mysql.h>

int testCurl(char* url);

void selectMysql();
void finish_with_error(MYSQL *con)
{
    fprintf(stderr, "%s\n", mysql_error(con));
    mysql_close(con);
    exit(1);
}

int main(int argc, char *argv[]) {
    /*GtkWidget *fenetre_principale = NULL;
    GtkBuilder *builder = NULL;
    GError *error = NULL;
    gchar *filename = NULL;
    *//* Initialisation de la librairie Gtk. *//*
    gtk_init(&argc, &argv);

    *//* Ouverture du fichier Glade de la fenêtre principale *//*
    builder = gtk_builder_new();

    *//* Création du chemin complet pour accéder au fichier test.glade. *//*
    *//* g_build_filename(); construit le chemin complet en fonction du système *//*
    *//* d'exploitation. ( / pour Linux et \ pour Windows) *//*
    filename = g_build_filename("/home/aurelien/Bureau/project C/Auly-ManageStockC-main/window_main.glade", NULL);

    *//* Chargement du fichier window_main.glade *//*
    gtk_builder_add_from_file(builder, filename, &error);
    g_free(filename);
    if (error) {
        gint code = error->code;
        g_printerr("%s\n", error->message);
        g_error_free(error);
        return code;
    }

    *//* Récupération du pointeur de la fenêtre principale *//*
    fenetre_principale = GTK_WIDGET(gtk_builder_get_object(builder, "MainWindow"));

    *//* Affectation du signal "destroy" à la fonction gtk_main_quit(); pour la *//*
    *//* fermeture de la fenêtre. *//*
    g_signal_connect (G_OBJECT(fenetre_principale), "destroy", (GCallback) gtk_main_quit, NULL);

    *//* Affichage de la fenêtre principale. *//*
    gtk_widget_show_all(fenetre_principale);

    gtk_main();*/

    testCurl("google.fr");

    selectMysql();


    return 0;
}

int testCurl(char *url) {
    CURL *curl;
    CURLcode res;

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url);

        /* example.com is redirected, so we tell libcurl to follow redirection */
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

        /* create an output file and prepare to write the response */
        FILE *output_file = fopen("output_file.html", "w");
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, output_file);

        /* Perform the request, res will get the return code */
        res = curl_easy_perform(curl);

        /* Check for errors */
        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %sn",
                    curl_easy_strerror(res));
        }

        /* always cleanup */
        curl_easy_cleanup(curl);
    }
    return 0;
}


/**
 * Fonction test qui récupère les informations en bdd.
 */
void selectMysql() {
    MYSQL *con = mysql_init(NULL);

    if (con == NULL) {
        fprintf(stderr, "%s\n", mysql_error(con));
        exit(1);
    }

    // Connexion à la BDD. Variable con (init), host, username, mdp, nom de la bdd, port, jsp et jsp mdrrr
    if (mysql_real_connect(con, "localhost", "esgi", "esgi",
                           "test", 0, NULL, 0) == NULL) {
        fprintf(stderr, "%s\n", mysql_error(con));
        mysql_close(con);
        exit(1);
    }

    // Exécution d'une requête SQL. Exit avec erreur sinon.
    if (mysql_query(con, "SELECT * FROM Product"))
    {
        finish_with_error(con);
    }

    MYSQL_RES *result = mysql_store_result(con);

    if (result == NULL)
    {
        finish_with_error(con);
    }

    // Renvoie le nombre de colonnes de la table (pas le nombre de lignes bellec)
    int num_fields = mysql_num_fields(result);

    MYSQL_ROW row;

    printf("\n\nNombre de colonnes : %d\n", num_fields);

    printf("\nID | titre | description | url \n");

    // mysql_fetch_row fonctionne comme en PHP, chaque call de la fonction récupère la ligne suivante dans le résultat (result)
    while ((row = mysql_fetch_row(result)))
    {
        // La ligne existe sous forme de array donc on itère pour récupérer.
        for(int i = 0; i < num_fields; i++)
        {
            printf("%s | ", row[i] ? row[i] : "NULL");
        }

        printf("\n");
    }

    // J'imagine qu'on free la mémoire prise par tous les mallocs des fonctions mysql pour stocker les strings
    mysql_free_result(result);

    printf("\nMySQL client version: %s\n", mysql_get_client_info());


    // On ferme la connexion au serveur
    mysql_close(con);
}