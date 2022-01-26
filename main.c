#include <stdio.h>
#include <gtk/gtk.h>
#include <curl/curl.h>
#include <curl/easy.h>
#include "window.h"

int testCurl(char* url);

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