#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <curl/curl.h>
#include <curl/easy.h>
#include "window.h"
#include <mysql.h>
#include <string.h>
#include "bddMethods.h"
#include "curlBdd.h"

//Variables globales pour le prix
int language = 0;

int switchLanguage(int *lang);

// D"claration de la fonction commandline
int commandLine();

int showProductsCL();

//Entrée de text pour le formulaire
GtkEntry *entry_url;

GtkEntry *entry_description;
GtkEntry *entry_name;
int id;

/* Partie GTK */
void main_page();

void OnDestroy(GtkWidget *pWidget, gpointer pData);

void form(int arc, char **argv);

void refresh_page(GtkWidget *pWidget, gpointer pData);

void more(GtkWidget *pWidget, gpointer pData);

void delete(GtkWidget *pWidget, gpointer data);

void modify(GtkWidget *pWidget, gpointer data);

void close_window(GtkWidget *pWidget, gpointer data);

void go_on(GtkWidget *pWidget, gpointer data);


void confirm(GtkWidget *pEntry, gpointer data);

typedef struct _identifier {
    int id;

} identifier;

int main(int argc, char *argv[]) {
    switchLanguage(&language);

    bool commandLineExit = 0;
    for (int i = 0; i < argc; ++i) {
        if (strstr(argv[i], "commandLine")) {
            commandLineExit = 1;
        }

    }

    if (commandLineExit) {
        commandLine();
        exit(EXIT_SUCCESS);
    }

    //TRES IMPORTANT (Permet de faire fonctionner les fonction de gtk)
    gtk_init(&argc, &argv);

    main_page();
    showProductsCL();


    return EXIT_SUCCESS;

}

/* Partie GTK */
void main_page() {
    GtkWidget *window;
    GtkLabel *label1;

    gchar *sUtf8;
    GtkWidget *pVBox;

    GtkWidget *pHBox;
    GtkWidget *btn;
    GtkWidget *refresh;
    GtkWidget *scrollbar;

    char *buffer[255];
    FILE *data = NULL;

    int tmp;
    char *token;

    unsigned int counter = 0;

    char ***productList;
    productList = malloc(1);
    unsigned long rowCount = 0;


    data = fopen("../data.txt", "r");

    //DEFINITION DE VARIABLE LABEL (gtk label new permet de cr�er un label avec un string)
    label1 = (GtkLabel *) gtk_label_new(NULL);

    //DEFINITION d'un label avec des classe de style
    switch (language) {
        case 0:
            sUtf8 = g_locale_to_utf8(
                    "<span font_desc=\"Times New Roman 16\" foreground=\"#880000\"><u>Liste des produits</u></span>",
                    -1, NULL,
                    NULL, NULL);
            gtk_label_set_markup(GTK_LABEL(label1), sUtf8);
            break;
        case 1:
            sUtf8 = g_locale_to_utf8(
                    "<span font_desc=\"Times New Roman 16\" foreground=\"#880000\"><u>Product List</u></span>", -1,
                    NULL,
                    NULL, NULL);
            gtk_label_set_markup(GTK_LABEL(label1), sUtf8);
            break;
    }


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



    // Appelle de la fonction OnDestroy sur le bouton pour fem� la fenetre
    g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(OnDestroy), NULL);

    //Creation d'un bouton avec label
    switch (language) {
        case 0:
            btn = gtk_button_new_with_label("Ajouter un produit");
            break;
        case 1:
            btn = gtk_button_new_with_label("Add a product");
            break;
        default:
            btn = gtk_button_new_with_label("Ajouter un produit");
            break;
    }
    //Appelle de la fonction go to form
    g_signal_connect(G_OBJECT(btn), "clicked", G_CALLBACK(form), NULL);

    switch (language) {
        case 0:
            refresh = gtk_button_new_with_label("rafraichir");
            break;
        case 1:
            refresh = gtk_button_new_with_label("Refresh");
            break;
        default:
            refresh = gtk_button_new_with_label("rafraichir");
            break;
    }
    g_signal_connect(G_OBJECT(refresh), "clicked", G_CALLBACK(refresh_page), window);


    //Insertion d'une box horizontal puis du label/input/btn dans la fenetre
    // RAPPEL (WINDOW>SCROLLABLE>VERTICALE BOX > MULTIPLE HORIZONTALE BOX> INFOS)
    gtk_box_pack_start(GTK_BOX(pVBox), pHBox, FALSE, FALSE, 0);

    gtk_box_pack_start(GTK_BOX(pHBox), label1, FALSE, FALSE, 0);

    gtk_box_pack_start(GTK_BOX(pHBox), refresh, FALSE, FALSE, 0);

    gtk_box_pack_start(GTK_BOX(pHBox), btn, FALSE, FALSE, 0);


    //Boucle pour rentrer les infos


    retrieveProducts(productList, &rowCount);


    GtkWidget *bodyDiv[rowCount];
    GtkWidget *bodyLabel[rowCount][4];
    GtkWidget *showMoreBtn[rowCount];
    identifier id[rowCount];

    for (unsigned long k = 0; k < rowCount; ++k) {
        bodyDiv[k] = gtk_hbox_new(TRUE, 0);
        gtk_box_pack_start(GTK_BOX(pVBox), bodyDiv[k], FALSE, FALSE, 0);
        for (int l = 0; l < 5; ++l) {
            switch (l) {
                case 0:
                    id[counter].id = atoi(productList[k][l]);
                    showMoreBtn[k] = gtk_button_new_with_label("...");
                    g_signal_connect(G_OBJECT(showMoreBtn[k]), "clicked", G_CALLBACK(more), &id[counter].id);
                    gtk_box_pack_end(GTK_BOX(bodyDiv[k]), showMoreBtn[k], FALSE, FALSE, 0);
                    break;
                case 1:
                    bodyLabel[k][l] = gtk_label_new(productList[k][l]);
                    gtk_box_pack_start(GTK_BOX(bodyDiv[k]), bodyLabel[k][l], FALSE, FALSE, 0);
                    break;
                case 2:
                    break;
                case 3:
                    bodyLabel[k][l] = gtk_label_new(strcat(productList[k][l], "€"));
                    gtk_box_pack_start(GTK_BOX(bodyDiv[k]), bodyLabel[k][l], FALSE, FALSE, 0);
                    break;
                case 4:
                    if (language == 1) {
                        if (atoi(productList[k][l])) {
                            bodyLabel[k][l] = gtk_label_new("In stock");
                            gtk_box_pack_start(GTK_BOX(bodyDiv[k]), bodyLabel[k][l], FALSE, FALSE, 0);
                        } else {
                            bodyLabel[k][l] = gtk_label_new("Out of stock");
                            gtk_box_pack_start(GTK_BOX(bodyDiv[k]), bodyLabel[k][l], FALSE, FALSE, 0);
                        }
                    } else {
                        if (atoi(productList[k][l])) {
                            bodyLabel[k][l] = gtk_label_new("En Stock");
                            gtk_box_pack_start(GTK_BOX(bodyDiv[k]), bodyLabel[k][l], FALSE, FALSE, 0);
                        } else {
                            bodyLabel[k][l] = gtk_label_new("Rupture");
                            gtk_box_pack_start(GTK_BOX(bodyDiv[k]), bodyLabel[k][l], FALSE, FALSE, 0);
                        }
                    }
                    break;

            }
            counter++;
        }


    }
    freeProductList(productList, &rowCount);


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
    gtk_entry_set_max_length(entry_name, 20);
    entry_url = gtk_entry_new();
    entry_description = gtk_entry_new();

    switch (language) {
        case 0:
            labelDescription = gtk_label_new("Description");
            labelName = gtk_label_new("Nom");
            labelUrl = gtk_label_new("Url");

            confirmBtn = gtk_button_new_with_label("Confirmer");
            break;
        case 1:
            labelDescription = gtk_label_new("Description");
            labelName = gtk_label_new("Name");
            labelUrl = gtk_label_new("Url");

            confirmBtn = gtk_button_new_with_label("Confirm");
            break;
        default:
            labelDescription = gtk_label_new("Description");
            labelName = gtk_label_new("Nom");
            labelUrl = gtk_label_new("Url");

            confirmBtn = gtk_button_new_with_label("Confirmer");
            break;
    }

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

    char **rowCopy;
    rowCopy = malloc(2 * sizeof(char *));

    GtkWidget *window;
    GtkWidget *pVBox;
    GtkWidget *footer;
    GtkWidget *modif_div;
    GtkWidget *title;
    GtkWidget *deleteButton;
    GtkWidget *closeButton;
    GtkWidget *modifyButton;
    GtkWidget *labelTitle;
    GtkWidget *labelHeader[3];
    GtkWidget *divHeader;
    GtkWidget *labelDescription;
    GtkWidget *goOnBtn;
    GtkWidget *historyDiv[5];
    GtkWidget *label[5][5];
    char ***historyArray;
    unsigned long rowCount = 0;
    char *sUtf8;

    historyArray = malloc(5 * sizeof(char *));

    entry_description = gtk_entry_new();

    entry_name = gtk_entry_new();

    entry_url = gtk_entry_new();


    pVBox = gtk_vbox_new(FALSE, 25);
    footer = gtk_hbox_new(TRUE, 25);
    modif_div = gtk_hbox_new(TRUE, 25);

    switch (language) {
        case 0:
            deleteButton = gtk_button_new_with_label("Supprimer");
            closeButton = gtk_button_new_with_label("Fermer");
            modifyButton = gtk_button_new_with_label("Modifier");
            goOnBtn = gtk_button_new_with_label("Ouvrir dans le navigateur");
            break;
        case 1:
            deleteButton = gtk_button_new_with_label("Delete");
            closeButton = gtk_button_new_with_label("Close");
            modifyButton = gtk_button_new_with_label("Modify");
            goOnBtn = gtk_button_new_with_label("Open in browser");
            break;
        default:
            deleteButton = gtk_button_new_with_label("Supprimer");
            closeButton = gtk_button_new_with_label("Fermer");
            modifyButton = gtk_button_new_with_label("Modifier");
            goOnBtn = gtk_button_new_with_label("Ouvrir dans le navigateur");
            break;
    }
    g_signal_connect(G_OBJECT(goOnBtn), "clicked", G_CALLBACK(go_on), window);

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
    gtk_box_pack_start(GTK_BOX(pVBox), goOnBtn, NULL, NULL, 0);


    gtk_box_pack_end(GTK_BOX(pVBox), footer, FALSE, TRUE, 15);


    retrieveOneProductInfo(id->id, rowCopy);

    for (int i = 0; i < 3; ++i) {
        switch (i) {
            case 0 :

                gtk_entry_set_text(entry_name, rowCopy[i]);
                break;
            case 1 :

                gtk_entry_set_text(entry_description, rowCopy[i]);
                break;
            case 2 :
                gtk_entry_set_text(entry_url, rowCopy[i]);
                break;
        }


        free(rowCopy[i]);
    }
    free(rowCopy);

    retrieveProductHistory(id->id, historyArray, &rowCount);


    labelHeader[0] = gtk_label_new(NULL);
    labelHeader[1] = gtk_label_new(NULL);
    labelHeader[2] = gtk_label_new(NULL);

    switch (language) {
        case 0:
            sUtf8 = g_locale_to_utf8(
                    "<u>Prix</u>", -1, NULL,
                    NULL, NULL);
            gtk_label_set_markup(GTK_LABEL(labelHeader[0]), sUtf8);

            sUtf8 = g_locale_to_utf8(
                    "<u>Status</u>", -1, NULL,
                    NULL, NULL);
            gtk_label_set_markup(GTK_LABEL(labelHeader[1]), sUtf8);

            sUtf8 = g_locale_to_utf8(
                    "<u>Date</u>", -1, NULL,
                    NULL, NULL);
            gtk_label_set_markup(GTK_LABEL(labelHeader[2]), sUtf8);
            break;
        case 1:
            sUtf8 = g_locale_to_utf8(
                    "<u>Price</u>", -1, NULL,
                    NULL, NULL);
            gtk_label_set_markup(GTK_LABEL(labelHeader[0]), sUtf8);

            sUtf8 = g_locale_to_utf8(
                    "<u>Status</u>", -1, NULL,
                    NULL, NULL);
            gtk_label_set_markup(GTK_LABEL(labelHeader[1]), sUtf8);

            sUtf8 = g_locale_to_utf8(
                    "<u>Date</u>", -1, NULL,
                    NULL, NULL);
            gtk_label_set_markup(GTK_LABEL(labelHeader[2]), sUtf8);
            break;
        default:
            sUtf8 = g_locale_to_utf8(
                    "<u>Prix</u>", -1, NULL,
                    NULL, NULL);
            gtk_label_set_markup(GTK_LABEL(labelHeader[0]), sUtf8);

            sUtf8 = g_locale_to_utf8(
                    "<u>Status</u>", -1, NULL,
                    NULL, NULL);
            gtk_label_set_markup(GTK_LABEL(labelHeader[1]), sUtf8);

            sUtf8 = g_locale_to_utf8(
                    "<u>Date</u>", -1, NULL,
                    NULL, NULL);
            gtk_label_set_markup(GTK_LABEL(labelHeader[2]), sUtf8);
            break;
    }


    divHeader = gtk_hbox_new(TRUE, 25);

    gtk_box_pack_start(GTK_BOX(pVBox), divHeader, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(divHeader), labelHeader[0], FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(divHeader), labelHeader[1], FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(divHeader), labelHeader[2], FALSE, FALSE, 0);


    for (unsigned long k = 0; k < rowCount; ++k) {
        historyDiv[k] = gtk_hbox_new(TRUE, 25);
        gtk_box_pack_start(GTK_BOX(pVBox), historyDiv[k], FALSE, FALSE, 0);

        for (int l = 0; l < 5; ++l) {
            switch (l) {
                case 2 :
                    label[k][l] = gtk_label_new(strcat(historyArray[k][l], "€"));
                    gtk_box_pack_start(GTK_BOX(historyDiv[k]), label[k][l], FALSE, FALSE, 0);
                    break;
                case 3 :
                    if (atoi(historyArray[k][l])) {
                        label[k][l] = gtk_label_new("En Stock");
                        gtk_box_pack_start(GTK_BOX(historyDiv[k]), label[k][l], FALSE, FALSE, 0);
                    } else {
                        label[k][l] = gtk_label_new("Rupture");
                        gtk_box_pack_start(GTK_BOX(historyDiv[k]), label[k][l], FALSE, FALSE, 0);
                    }
                    break;
                case 4 :
                    label[k][l] = gtk_label_new(historyArray[k][l]);
                    gtk_box_pack_start(GTK_BOX(historyDiv[k]), label[k][l], FALSE, FALSE, 0);
                    break;
            }

            free(historyArray[k][l]);
        }
    }
    free(historyArray);


    gtk_box_pack_start(GTK_BOX(footer), deleteButton, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(footer), modifyButton, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(footer), closeButton, FALSE, FALSE, 0);
    g_signal_connect(G_OBJECT(deleteButton), "clicked", G_CALLBACK(close_window), window);
    g_signal_connect(G_OBJECT(closeButton), "clicked", G_CALLBACK(close_window), window);
    g_signal_connect(G_OBJECT(modifyButton), "clicked", G_CALLBACK(close_window), window);
    g_signal_connect(G_OBJECT(deleteButton), "clicked", G_CALLBACK(delete), id->id);
    g_signal_connect(G_OBJECT(modifyButton), "clicked", G_CALLBACK(modify), id->id);
    gtk_widget_show_all(window);


}

void close_window(GtkWidget *pWidget, gpointer data) {
    gtk_window_close(data);
}

void refresh_page(GtkWidget *pWidget, gpointer data) {
    refreshLog(language);
    gtk_window_close(data);
    main_page();

}


void delete(GtkWidget *pWidget, gpointer data) {
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

    char sql[50];
    sprintf(sql, "DELETE FROM Product WHERE id = %d", data); // On insère l'id dans la string query sql
    // Exécution d'une requête SQL. Exit avec erreur sinon.
    if (mysql_query(con, sql)) {
        finish_with_error(con);
    }

    MYSQL_RES *result = mysql_store_result(con);

    if (result != NULL) {
        finish_with_error(con);

    }

    // J'imagine qu'on free la mémoire prise par tous les mallocs des fonctions mysql pour stocker les strings
    mysql_free_result(result);

    // On ferme la connexion au serveur
    mysql_close(con);
}

void modify(GtkWidget *pWidget, gpointer data) {
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

    char sql[50];
    sprintf(sql, "UPDATE Product SET titre = '%s', description = '%s' WHERE id = %d",
            gtk_entry_get_text(GTK_ENTRY(entry_name)),
            gtk_entry_get_text(GTK_ENTRY(entry_description)),
            data);
    // On insère l'id dans la string query sql
    // Exécution d'une requête SQL. Exit avec erreur sinon.
    if (mysql_query(con, sql)) {
        finish_with_error(con);
    }

    MYSQL_RES *result = mysql_store_result(con);

    if (result == NULL) {
        finish_with_error(con);
    }

    // J'imagine qu'on free la mémoire prise par tous les mallocs des fonctions mysql pour stocker les strings
    mysql_free_result(result);

    // On ferme la connexion au serveur
    mysql_close(con);
}

void go_on(GtkWidget *pWidget, gpointer data) {

    char *URL = "xdg-open ";
    char *url = gtk_entry_get_text(entry_url);
    char *res;

    res = malloc(strlen(URL) + strlen(url) + 1);
    if (!res) {
        fprintf(stderr, "malloc() failed: insufficient memory!\n");

    } else {

        strcpy(res, URL);
        strcat(res, url);


        system(res);
        free(res);
    }
}

void OnDestroy(GtkWidget *pWidget, gpointer data) {
    gtk_main_quit;

}

int switchLanguage(int *lang) {
    char buffer[255];
    FILE *config = fopen("./config.txt", "r");

    //     On vérifie que l'on ouvre bien le fichier
    if (config == NULL) {
        printf("\nErreur il manque le fichier de config\n");
        return EXIT_FAILURE;
    }

    fseek(config, 0, SEEK_SET);
    fread(buffer, 255, 1, config);
    if (strstr(buffer, "langue:fr")) {
        printf("\nLangue mise en français\n");
        *lang = 0;
    } else {
        if (strstr(buffer, "langue:en")) {
            printf("\nLanguage set to english\n");
            *lang = 1;
        } else {
            printf("\nErreur dans la lecture de la configuration de la langue, langue par défaut (français).\n");
            printf("\nError in reading the configuration file. Language set to default (french.\n");
            *lang = 0;
        }
    }

    return EXIT_SUCCESS;

}

int commandLine() {
    int choice = -1;
    bool endIt = false;
    if (language == 1) {
        printf("Hello, World (and Senora Sananes)!\n");

        printf("\n WHat would you like to do ?");

        do {
            printf("\n\nMain Menu\n\n");
            printf("    1. Show the product list \n");
            printf("    2. Update the current products' status \n");
            printf("    3. Exit\n");
            scanf("%d", &choice);
            fflush(stdin);
            switch (choice) {
                case 1:
                    printf("\nHere is the list of products\n");
                    showProductsCL();
                    break;
                case 2:
                    printf("\nUpdating the informations of the products in the database\n");
                    refreshLog(language);
                    break;
                case 3:
                    printf("Sad to see you leave king...\n");
                    endIt = true;
                    break;
                default:
                    printf("Wrong choice, TRY AGAIN!\n");
                    break;
            }
        } while (!endIt);
    } else {
        printf("Hello, World (et Monsieur Sananes)!\n");

        printf("\n Que voulez-vous faire ?");

        do {
            printf("\n\nMenu Principal\n\n");
            printf("    1. Afficher la liste des produits \n");
            printf("    2. Mettre à jour le statut des produits \n");
            printf("    3. Exit\n");
            scanf("%d", &choice);
            fflush(stdin);
            switch (choice) {
                case 1:
                    printf("\nVoici la liste des produits\n");
                    showProductsCL();
                    break;
                case 2:
                    printf("\nRafraichissement des informations en base de données\n");
                    refreshLog(language);
                    break;
                case 3:
                    printf("Triste que vous partiez...\n");
                    endIt = true;
                    break;
                default:
                    printf("Mauvais choix, réessayez\n");
                    break;
            }
        } while (!endIt);
    }


    printf("\n\nFin du programme bye bye hehehehehe\n");
    return 0;
}

int showProductsCL() {
    char ***productList = malloc(1);
    unsigned long rowCount = 0;
    char stock[] = "En stock";
    char stock1[] = "Pas en stock";
    char stockEN[] = "In stock";
    char stock1EN[] = "Out of stock";
    retrieveProducts(productList, &rowCount);
    for (int i = 0; i < rowCount; ++i) {
            if (language == 1) {
                printf("\n      %s : price:%s, status: %s, updated on %s", productList[i][1], productList[i][3],
                       productList[i][4] ? stockEN : stock1EN, productList[i][2]);
                printf("\n");
            } else {
                printf("\n      %s : prix:%s, status: %s, mis à jour le %s", productList[i][1], productList[i][3],
                       productList[i][4] ? stock : stock1, productList[i][2]);
                printf("\n");
            }
        }


    freeProductList(productList, &rowCount);
    return EXIT_SUCCESS;
}
