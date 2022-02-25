#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <curl/curl.h>
#include <curl/easy.h>
#include "window.h"
#include <mysql.h>
#include <string.h>


//Entrée de text pour le formulaire
GtkEntry *entry_url;

GtkEntry *entry_description;
GtkEntry *entry_name;
int id;


int testCurl(char *url);

//char ** selectMysql();
void selectMysql();

void finish_with_error(MYSQL *con) {
    fprintf(stderr, "%s\n", mysql_error(con));
    mysql_close(con);
    exit(1);
}

/* Partie GTK */
void main_page(int argc, char **argv);

void OnDestroy(GtkWidget *pWidget, gpointer pData);

void form(int arc, char **argv);

void more(GtkWidget *pWidget, gpointer pData);

void delete(GtkWidget *pWidget, gpointer data);

void test(GtkWidget *pWidget, gpointer data);


void confirm(GtkWidget *pEntry, gpointer data);

typedef struct _identifier {
    int id;

} identifier;

int main(int argc, char *argv[]) {
    char **row;



    //testCurl("google.fr");
    //row = selectMysql();
    //selectMysql();

    main_page(argc, argv);


    return 0;

    return EXIT_SUCCESS;

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
//char** selectMysql() {
void selectMysql() {
    MYSQL *con = mysql_init(NULL);
    char *ptr = (char *) malloc(255 * 10 * 4 * sizeof(char));

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
    if (mysql_query(con, "SELECT * FROM Product")) {
        finish_with_error(con);
    }

    MYSQL_RES *result = mysql_store_result(con);

    if (result == NULL) {
        finish_with_error(con);
    }

    // Renvoie le nombre de colonnes de la table (pas le nombre de lignes bellec)
    int num_fields = mysql_num_fields(result);

    MYSQL_ROW row;

    printf("\n\nNombre de colonnes : %d\n", num_fields);

    printf("\nID | titre | description | url \n");

    // mysql_fetch_row fonctionne comme en PHP, chaque call de la fonction récupère la ligne suivante dans le résultat (result)
    while ((row = mysql_fetch_row(result))) {
        // La ligne existe sous forme de array donc on itère pour récupérer.
        for (int i = 0; i < num_fields; i++) {
            printf("%s | ", row[i] ? row[i] : "NULL");
            //    *(ptr+i) = row[i] ? row[i] : "NULL";
        }


        printf("\n");
    }



    // J'imagine qu'on free la mémoire prise par tous les mallocs des fonctions mysql pour stocker les strings
    mysql_free_result(result);

    printf("\nMySQL client version: %s\n", mysql_get_client_info());


    // On ferme la connexion au serveur
    mysql_close(con);
    //  return ptr;


}


/* Partie GTK */
void main_page(int argc, char **argv) {
    GtkWidget *window;
    GtkLabel *label1;

    gchar *sUtf8;
    GtkWidget *pVBox;

    GtkWidget *pHBox;
    GtkWidget *btn;
    GtkWidget *entry;
    GtkWidget *scrollbar;

    char *buffer[255];
    FILE *data = NULL;
    int c, nbRow = 0, i = 0, j = 0;
    int c2 = '\0';
    int tmp;
    char *token;

    unsigned int counter = 0;


    data = fopen("../data.txt", "r");

    //TRES IMPORTANT (Permet de faire fonctionner les fonction de gtk)
    gtk_init(&argc, &argv);


    //DEFINITION DE VARIABLE LABEL (gtk label new permet de cr�er un label avec un string)
    label1 = (GtkLabel *) gtk_label_new(NULL);


    if (data != NULL) {

        while ((c = fgetc(data)) != EOF) {
            if (c == '\n')
                nbRow++;
            c2 = c;
        }

        if (c2 != '\n')
            nbRow++;
        fclose(data);
    }


    GtkLabel *label[nbRow][3];
    GtkWidget *pHBoxProduct[nbRow];
    GtkWidget *btnMore[nbRow];

    identifier id[nbRow * 2];


    //DEFINITION d'un label avec des classe de style
    sUtf8 = g_locale_to_utf8(
            "<span font_desc=\"Times New Roman 16\" foreground=\"#880000\"><u>Liste des produits</u></span>", -1, NULL,
            NULL, NULL);
    gtk_label_set_markup(GTK_LABEL(label1), sUtf8);

    // ON a mis les info de stUtf8 dasn le label1 on peut donc l'effacer
    g_free(sUtf8);

    //On place au centre le label 1
    gtk_label_set_justify(GTK_LABEL(label1), GTK_JUSTIFY_CENTER);


    //Definition d'un boite scrollable
    scrollbar = gtk_scrolled_window_new(NULL, NULL);


    //Definition de la fenetre + Parametre titre/taille/position/redefinition de la taille
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW (window), "Auly ManageStockC - Liste");
    gtk_window_set_default_size(GTK_WINDOW (window), 700, 700);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_window_set_resizable(GTK_WINDOW(window), TRUE);

    //On insere la box scrollable dans la window
    gtk_container_add(GTK_CONTAINER(window), scrollbar);


    //Cr�ation de box/container verticale / horizontales
    pVBox = gtk_vbox_new(FALSE, 25);


    pHBox = gtk_hbox_new(TRUE, 0);

    // ON insert la box verticale dans la box scrollable
    gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(scrollbar), pVBox);

    //Definintion d'un input text
    entry = gtk_entry_new();


    // Appelle de la fonction OnDestroy sur le bouton pour fem� la fenetre
    g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(OnDestroy), NULL);

    //Creation d'un bouton avec label
    btn = gtk_button_new_with_label("Ajouter un produit");
    //Appelle de la fonction go to form
    g_signal_connect(G_OBJECT(btn), "clicked", G_CALLBACK(form), NULL);


    //Insertion d'une box horizontal puis du label/input/btn dans la fenetre
    // RAPPEL (WINDOW>SCROLLABLE>VERTICALE BOX > MULTIPLE HORIZONTALE BOX> INFOS)
    gtk_box_pack_start(GTK_BOX(pVBox), pHBox, FALSE, FALSE, 0);

    gtk_box_pack_start(GTK_BOX(pHBox), label1, FALSE, FALSE, 0);

    gtk_box_pack_start(GTK_BOX(pHBox), entry, FALSE, FALSE, 0);

    gtk_box_pack_start(GTK_BOX(pHBox), btn, FALSE, FALSE, 0);


    //Boucle pour rentrer les infos

    data = fopen("../data.txt", "r");

    if (data != NULL) {

        i = 0;


        while (fgets(buffer, 255, data)) {
            token = strtok(buffer, ";");


            j = 0;

            pHBoxProduct[i] = gtk_hbox_new(TRUE, 0);
            gtk_box_pack_start(GTK_BOX(pVBox), pHBoxProduct[i], FALSE, FALSE, 0);
            while (token != NULL) {

                if (j != 0) {
                    label[i][j] = gtk_label_new(token);

                    gtk_box_pack_start(GTK_BOX(pHBoxProduct[i]), label[i][j], FALSE, FALSE, 0);
                    token = strtok(NULL, ";");

                } else {
                    tmp = atoi(token);
                    id[counter].id = tmp;
                }
                j++;
            }
            btnMore[i] = gtk_button_new_with_label("...");
            gtk_box_pack_start(GTK_BOX((pHBoxProduct[i])), btnMore[i], FALSE, FALSE, 0);
            g_signal_connect(G_OBJECT(btnMore[i]), "pressed", G_CALLBACK(more), &id[counter]);
            counter++;
            i++;
        }
        fclose(data);
    }
    //Affiche la window et tous les widget a l'interieur
    gtk_widget_show_all(window);
    //definit la window main
    gtk_main();


}

void form(int argc, char **argv) {
    //Definintion des widget de gtk
    GtkWidget *window;

    GtkWidget *labelName;
    GtkWidget *labelUrl;

    GtkWidget *labelDescription;

    GtkWidget *pVBox1;
    GtkWidget *pHBox1;
    GtkWidget *pHBox2;
    GtkWidget *pHBox3;

    GtkWidget *VNameBox;
    GtkWidget *VDescriptionBox;
    GtkWidget *VUrlBox;

    GtkWidget *confirmBtn;


    gtk_init(&argc, &argv);


    //Definition de la window plus parametre
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW (window), "Auly ManageStockC - Formulaire");
    gtk_window_set_default_size(GTK_WINDOW (window), 750, 400);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_window_set_resizable(GTK_WINDOW(window), TRUE);


    pVBox1 = gtk_vbox_new(TRUE, 0);
    pHBox1 = gtk_hbox_new(TRUE, 0);
    pHBox2 = gtk_hbox_new(TRUE, 0);
    pHBox3 = gtk_hbox_new(TRUE, 0);

    VNameBox = gtk_vbox_new(TRUE, 2);
    VUrlBox = gtk_vbox_new(TRUE, 2);
    VDescriptionBox = gtk_vbox_new(TRUE, 2);

    gtk_container_add(GTK_CONTAINER(window), pVBox1);

    entry_name = gtk_entry_new();
    entry_url = gtk_entry_new();
    entry_description = gtk_entry_new();

    labelDescription = gtk_label_new("Description");
    labelName = gtk_label_new("Nom");
    labelUrl = gtk_label_new("Url");

    confirmBtn = gtk_button_new_with_label("Confirmer");

    gtk_box_pack_start(GTK_BOX(pVBox1), pHBox1, TRUE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(pVBox1), pHBox2, TRUE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(pVBox1), pHBox3, TRUE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(pHBox3), confirmBtn, FALSE, FALSE, 50);

    gtk_box_pack_start(GTK_BOX(pHBox1), VNameBox, TRUE, TRUE, 25);
    gtk_box_pack_start(GTK_BOX(VNameBox), labelName, TRUE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(VNameBox), entry_name, TRUE, TRUE, 0);

    gtk_box_pack_start(GTK_BOX(pHBox1), VUrlBox, TRUE, TRUE, 25);
    gtk_box_pack_start(GTK_BOX(VUrlBox), labelUrl, TRUE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(VUrlBox), entry_url, TRUE, TRUE, 0);

    gtk_box_pack_start(GTK_BOX(pHBox2), VDescriptionBox, TRUE, TRUE, 25);
    gtk_box_pack_start(GTK_BOX(VDescriptionBox), labelDescription, TRUE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(VDescriptionBox), entry_description, TRUE, TRUE, 0);


    g_signal_connect(G_OBJECT(confirmBtn), "clicked", G_CALLBACK(confirm), window);

    gtk_widget_show_all(window);
    gtk_main();

}

void confirm(GtkWidget *pEntry, gpointer data) {
    gtk_window_close(data);

    g_print("Nom: %s\n", gtk_entry_get_text(GTK_ENTRY(entry_name)));
    g_print("Url : %s\n", gtk_entry_get_text(GTK_ENTRY(entry_url)));
    g_print("Description: %s\n", gtk_entry_get_text(GTK_ENTRY(entry_description)));


    MYSQL *conn = mysql_init(NULL);
    if (mysql_real_connect(conn, "localhost", "esgi", "esgi",
                           "test", 0, NULL, 0)) {
        char q[1000];
        sprintf(&q, "INSERT INTO Product(titre, description, url) VALUES ('%s', '%s', '%s')",
                gtk_entry_get_text(GTK_ENTRY(entry_name)),
                gtk_entry_get_text(GTK_ENTRY(entry_description)),
                gtk_entry_get_text(GTK_ENTRY(entry_url)));

        if (mysql_query(conn, q)) {
            g_print("Error: %d", mysql_errno(conn));
        }
    } else {
        g_print("Error: %d", mysql_errno(conn));
    }

}

void more(GtkWidget *button, gpointer data) {
    (void) button;
    identifier *id = data;


    GtkWidget *window;
    GtkWidget *pVBox;
    GtkWidget *footer;
    GtkWidget *modif_div;
    GtkWidget *title;
    GtkWidget *deleteButton;
    GtkWidget *closeButton;
    GtkWidget *labelTitle;
    GtkWidget *labelDescription;

    entry_description = gtk_entry_new();
    gtk_entry_set_text(entry_description, "voiture pas chere");
    entry_name = gtk_entry_new();
    gtk_entry_set_text(entry_name, "voiture");

    pVBox = gtk_vbox_new(FALSE, 25);
    footer = gtk_hbox_new(TRUE, 25);
    modif_div = gtk_hbox_new(TRUE, 25);

    deleteButton = gtk_button_new_with_label("Supprimer");
    closeButton = gtk_button_new_with_label("Fermer");

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW (window), "Auly ManageStockC - Voir plus");
    gtk_window_set_default_size(GTK_WINDOW (window), 750, 400);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_window_set_resizable(GTK_WINDOW(window), TRUE);

    title = gtk_label_new(id);

    gtk_container_add(GTK_CONTAINER(window), pVBox);
    gtk_box_pack_start(GTK_BOX(pVBox), title, NULL, NULL, 0);

    gtk_box_pack_start(GTK_BOX(pVBox), modif_div, NULL, NULL, 0);
    gtk_box_pack_start(GTK_BOX(modif_div), entry_name, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(modif_div), entry_description, FALSE, FALSE, 0);

    gtk_box_pack_end(GTK_BOX(pVBox), footer, FALSE, TRUE, 15);


    gtk_box_pack_start(GTK_BOX(footer), deleteButton, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(footer), closeButton, FALSE, FALSE, 0);
    g_signal_connect(G_OBJECT(deleteButton), "clicked", G_CALLBACK(delete), window);
    g_signal_connect(G_OBJECT(closeButton), "clicked", G_CALLBACK(delete), window);
    g_signal_connect(G_OBJECT(deleteButton), "clicked", G_CALLBACK(test), "test");
    gtk_widget_show_all(window);


}

void delete(GtkWidget *pWidget, gpointer data) {
    gtk_window_close(data);
}

void test(GtkWidget *pWidget, gpointer data) {
    g_print("%s", data);
}


void OnDestroy(GtkWidget *pWidget, gpointer data) {
    gtk_main_quit;

}
