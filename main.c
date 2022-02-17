#include <stdio.h>
#include <gtk/gtk.h>
#include <curl/curl.h>
#include <curl/easy.h>
#include "window.h"
#include <mysql.h>

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

void go_to_form(int arc, char **argv);

void confirm(GtkWidget *pEntry, gpointer data);

int main(int argc, char *argv[]) {
    char **row;

    //testCurl("google.fr");
    //row = selectMysql();
    selectMysql();


    main_page(argc, argv);


    return EXIT_SUCCESS;


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
    GtkLabel *label[5][3];
    gchar *sUtf8;
    GtkWidget *pVBox;
    GtkWidget *pHBoxProduct[5];
    GtkWidget *pHBox;
    GtkWidget *btn;
    GtkWidget *entry;
    GtkWidget *scrollbar;





    //TRES IMPORTANT (Permet de faire fonctionner les fonction de gtk)
    gtk_init(&argc, &argv);


    //DEFINITION DE VARIABLE LABEL (gtk label new permet de cr�er un label avec un string)
    label1 = (GtkLabel *) gtk_label_new(NULL);
    label[0][0] = gtk_label_new("Product 1");
    label[0][1] = gtk_label_new("Description 1");
    label[0][2] = gtk_label_new("Price 1");

    label[1][0] = gtk_label_new("Product 2");
    label[1][1] = gtk_label_new("Description 2");
    label[1][2] = gtk_label_new("Price 2");

    label[2][0] = gtk_label_new("Product 3");
    label[2][1] = gtk_label_new("Description 3");
    label[2][2] = gtk_label_new("Price 3");

    label[3][0] = gtk_label_new("Product 4");
    label[3][1] = gtk_label_new("Description 4");
    label[3][2] = gtk_label_new("Price 4");

    label[4][0] = gtk_label_new("Product 5");
    label[4][1] = gtk_label_new("Description 5");
    label[4][2] = gtk_label_new("Price 5");


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

    pHBoxProduct[0] = gtk_hbox_new(TRUE, 0);
    pHBoxProduct[1] = gtk_hbox_new(TRUE, 0);
    pHBoxProduct[2] = gtk_hbox_new(TRUE, 0);
    pHBoxProduct[3] = gtk_hbox_new(TRUE, 0);
    pHBoxProduct[4] = gtk_hbox_new(TRUE, 0);

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
    g_signal_connect(G_OBJECT(btn), "clicked", G_CALLBACK(go_to_form), NULL);


    //Insertion d'une box horizontal puis du label/input/btn dans la fenetre
    // RAPPEL (WINDOW>SCROLLABLE>VERTICALE BOX > MULTIPLE HORIZONTALE BOX> INFOS)
    gtk_box_pack_start(GTK_BOX(pVBox), pHBox, FALSE, FALSE, 0);

    gtk_box_pack_start(GTK_BOX(pHBox), label1, FALSE, FALSE, 0);

    gtk_box_pack_start(GTK_BOX(pHBox), entry, FALSE, FALSE, 0);

    gtk_box_pack_start(GTK_BOX(pHBox), btn, FALSE, FALSE, 0);


    //Boucle pour rentrer les infos
    for (int i = 0; i < 5; i++) {
        gtk_box_pack_start(GTK_BOX(pVBox), pHBoxProduct[i], FALSE, FALSE, 0);
        gtk_box_pack_start(GTK_BOX(pHBoxProduct[i]), label[i][0], FALSE, FALSE, 0);
        gtk_box_pack_start(GTK_BOX(pHBoxProduct[i]), label[i][1], FALSE, FALSE, 0);
        gtk_box_pack_start(GTK_BOX(pHBoxProduct[i]), label[i][2], FALSE, FALSE, 0);
    }

    //Affiche la window et tous les widget � l'interieur
    gtk_widget_show_all(window);
    //d�finit la window main
    gtk_main();

}

void go_to_form(int argc, char **argv) {
    //apelle la fonction form
    form(argc, argv);
}

void form(int argc, char **argv) {
    //Definintion des widget de gtk
    GtkWidget *window;

    GtkWidget *entryName;
    GtkWidget *entryUrl;
    GtkWidget *entryPrice;
    GtkWidget *entryDescription;

    GtkWidget *labelName;
    GtkWidget *labelUrl;
    GtkWidget *labelPrice;
    GtkWidget *labelDescription;

    GtkWidget *pVBox1;
    GtkWidget *pHBox1;
    GtkWidget *pHBox2;

    GtkWidget *confirmBtn;


    gtk_init(&argc, &argv);


    //Definition de la window plus parametre
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW (window), "Auly ManageStockC - Formulaire");
    gtk_window_set_default_size(GTK_WINDOW (window), 700, 700);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_window_set_resizable(GTK_WINDOW(window), TRUE);


    pVBox1 = gtk_vbox_new(TRUE, 0);
    pHBox1 = gtk_hbox_new(TRUE, 0);
    pHBox2 = gtk_hbox_new(TRUE, 0);
    gtk_container_add(GTK_CONTAINER(window), pVBox1);

    entryName = gtk_entry_new();
    entryUrl = gtk_entry_new();
    entryPrice = gtk_entry_new();
    entryDescription = gtk_entry_new();

    labelDescription = gtk_label_new("Description");
    labelName = gtk_label_new("Nom");
    labelPrice = gtk_label_new("Prix");
    labelUrl = gtk_label_new("Url");

    confirmBtn = gtk_button_new_with_label("Confirmer");

    gtk_box_pack_start(GTK_BOX(pVBox1), pHBox1, TRUE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(pVBox1), pHBox2, TRUE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(pVBox1), confirmBtn, TRUE, FALSE, 0);

    gtk_box_pack_start(GTK_BOX(pHBox1), labelName, TRUE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(pHBox1), entryName, TRUE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(pHBox1), labelPrice, TRUE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(pHBox1), entryPrice, TRUE, FALSE, 0);

    gtk_box_pack_start(GTK_BOX(pHBox2), labelDescription, TRUE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(pHBox2), entryDescription, TRUE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(pHBox2), labelUrl, TRUE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(pHBox2), entryUrl, TRUE, FALSE, 0);


    g_signal_connect(G_OBJECT(confirmBtn), "clicked", G_CALLBACK(confirm), pVBox1);

    gtk_widget_show_all(window);
    gtk_main();

}

void confirm(GtkWidget *pEntry, gpointer data) {
    GtkWidget *tempEntryName;
    GtkWidget *tempLabelName;
    GList *list;
    const gchar *text;

    list = gtk_container_get_children(GTK_CONTAINER((GtkWidget *) data));
    tempEntryName = GTK_WIDGET(list->data);
    g_print("%p\n", data);
}

void OnDestroy(GtkWidget *pWidget, gpointer pData) {
    gtk_main_quit;
