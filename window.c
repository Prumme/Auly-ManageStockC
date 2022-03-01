#include <stdio.h>
#include <gtk-3.0/gtk/gtk.h>



static void
print_hello(GtkWidget *widget,
            gpointer data) {
    g_print("Hello World \n");
}

extern void
activate(GtkApplication *app,
         gpointer user_data) {
    GtkWidget *window;
    GtkWidget *button;

    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW (window), "Window");
    gtk_window_set_default_size(GTK_WINDOW (window),700, 700);

    button = gtk_button_new_with_label("Hello World");
    g_signal_connect(button, "clicked", G_CALLBACK(print_hello), NULL);

    gtk_widget_show(button);

    gtk_container_add (GTK_CONTAINER (window), button);

    gtk_widget_show(window);
}



