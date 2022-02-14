#include <gtk-3.0/gtk/gtk.h>

#include <stdlib.h>
#include <stdio.h>

void OnDestroy(GtkWidget * pWidget, gpointer pData);
void form(int arc, char **argv);
void go_to_form(int arc, char **argv);
void confirm(GtkWidget *pEntry, gpointer data);

int main(int argc, char **argv) {

    GtkWidget *window;
    GtkLabel *label1;
    GtkLabel *label[5][3];
    gchar* sUtf8;
    GtkWidget *pVBox;
    GtkWidget *pHBoxProduct[5];
    GtkWidget *pHBox;
    GtkWidget* btn;
    GtkWidget *entry;
    GtkWidget *scrollbar;






    gtk_init(&argc, &argv);

    label1 = gtk_label_new(NULL);
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


    sUtf8 = g_locale_to_utf8("<span font_desc=\"Times New Roman 16\" foreground=\"#880000\"><u>Liste des produits</u></span>",-1,NULL,NULL,NULL);
    gtk_label_set_markup(GTK_LABEL(label1), sUtf8);

    g_free(sUtf8);

    gtk_label_set_justify(GTK_LABEL(label1), GTK_JUSTIFY_CENTER);


    scrollbar = gtk_scrolled_window_new(NULL, NULL);



    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW (window), "Auly ManageStockC - Liste");
    gtk_window_set_default_size(GTK_WINDOW (window),700, 200);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_window_set_resizable(GTK_WINDOW(window), TRUE);

    gtk_container_add(GTK_CONTAINER(window), scrollbar);



    pVBox = gtk_vbox_new(FALSE, 25);

    pHBoxProduct[0] = gtk_hbox_new(TRUE, 0);
    pHBoxProduct[1] = gtk_hbox_new(TRUE, 0);
    pHBoxProduct[2] = gtk_hbox_new(TRUE, 0);
    pHBoxProduct[3] = gtk_hbox_new(TRUE, 0);
    pHBoxProduct[4] = gtk_hbox_new(TRUE, 0);

    pHBox = gtk_hbox_new(TRUE, 0);
    gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(scrollbar), pVBox);

    entry = gtk_entry_new();



    g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(OnDestroy), NULL);

    btn = gtk_button_new_with_label("Ajouter un produit");
    g_signal_connect(G_OBJECT(btn), "clicked", G_CALLBACK(go_to_form), NULL);

    gtk_box_pack_start(GTK_BOX(pVBox), pHBox, FALSE, FALSE, 0);

    gtk_box_pack_start(GTK_BOX(pHBox), label1, FALSE, FALSE, 0);

    gtk_box_pack_start(GTK_BOX(pHBox), entry, FALSE,FALSE,0);

    gtk_box_pack_start(GTK_BOX(pHBox), btn, FALSE,FALSE,0);

    for(int i = 0; i < 5; i++){
        gtk_box_pack_start(GTK_BOX(pVBox), pHBoxProduct[i], FALSE, FALSE, 0);
        gtk_box_pack_start(GTK_BOX(pHBoxProduct[i]), label[i][0], FALSE, FALSE, 0);
        gtk_box_pack_start(GTK_BOX(pHBoxProduct[i]), label[i][1], FALSE, FALSE, 0);
        gtk_box_pack_start(GTK_BOX(pHBoxProduct[i]), label[i][2], FALSE, FALSE, 0);
    }

    gtk_widget_show_all(window);
    gtk_main();
    return EXIT_SUCCESS;

}

void go_to_form(int argc, char **argv){

    form(argc, argv);
}

void form(int argc, char **argv){
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


    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW (window), "Auly ManageStockC - Formulaire");
    gtk_window_set_default_size(GTK_WINDOW (window),700, 700);
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

void confirm(GtkWidget *pEntry, gpointer data){
    GtkWidget *tempEntryName;
    GtkWidget *tempLabelName;
    GList *list;
    const gchar *text;

    list = gtk_container_get_children(GTK_CONTAINER((GtkWidget*)data));
    tempEntryName = GTK_WIDGET(list->data);
    g_print("%p\n", data);
}

void OnDestroy(GtkWidget * pWidget, gpointer pData){
    gtk_main_quit;
}