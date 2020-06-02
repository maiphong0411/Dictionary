#include <gtk/gtk.h>
#include "dcallback.h"
#include "bt-5.0.0/inc/btree.h"

BTA *data = NULL;
GObject *window,
        *about_button, *delete_button, *update_button, *add_button, *search_entry,
        *info_bar, *notify_label, *yes_delete, *yes_add, *yes_update, *no_button,
        *word_label, *meaning_textview, *textbuffer, *status_label,
        *comple, *liststore;

int main(int argc, char *argv[])
{
    //Convert data lan dau chay ct
    if (!file_exists("resource.dat")) {       
        convert_text_to_bt("anhviet.txt");
    }
    data = btopn("resource.dat", 0, 0);
    
    GtkBuilder *builder; 

    //Khoi tao giao dien
    gtk_init(&argc, &argv);

    //Khoi tao GtkBuilder
    builder = gtk_builder_new();
    gtk_builder_add_from_file(builder, "glade/window.glade", NULL);


    //Khoi tao Objects
    window = gtk_builder_get_object(builder, "window");
    
    about_button = gtk_builder_get_object(builder, "about_button");
    delete_button = gtk_builder_get_object(builder, "delete_button");
    update_button = gtk_builder_get_object(builder, "update_button");
    add_button = gtk_builder_get_object(builder, "add_button");      
    search_entry = gtk_builder_get_object(builder, "search_entry");

    info_bar = gtk_builder_get_object(builder, "info_bar");
    notify_label = gtk_builder_get_object(builder, "notify_label");
    yes_delete = gtk_builder_get_object(builder, "yes_delete");
    yes_add = gtk_builder_get_object(builder, "yes_add");
    yes_update = gtk_builder_get_object(builder, "yes_update");
    no_button = gtk_builder_get_object(builder, "no_button");
    
    word_label = gtk_builder_get_object(builder, "word_label");
    meaning_textview = gtk_builder_get_object(builder, "meaning_textview");
    textbuffer = gtk_builder_get_object(builder, "textbuffer");
    status_label = gtk_builder_get_object(builder, "status_label");

    comple = gtk_entry_completion_new();
    gtk_entry_completion_set_text_column(comple, 0);
    liststore = gtk_list_store_new(10, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING);
    gtk_entry_completion_set_model(comple, GTK_TREE_MODEL(liststore));
    gtk_entry_set_completion(GTK_ENTRY(search_entry), comple);
    
    //Ket noi tin hieu
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(about_button, "clicked", G_CALLBACK(show_about_dialog), NULL);
    g_signal_connect(delete_button, "clicked", G_CALLBACK(delete_button_clicked), NULL);
    g_signal_connect(update_button, "clicked", G_CALLBACK(update_button_clicked), NULL);
    g_signal_connect(add_button, "clicked", G_CALLBACK(add_button_clicked), NULL);
    
    g_signal_connect(search_entry, "key-press-event", G_CALLBACK(search_entry_key_press), NULL);
    g_signal_connect(search_entry, "activate", G_CALLBACK(search_entry_activate), NULL);
    g_signal_connect(search_entry, "grab-focus", G_CALLBACK(no_button_clicked), NULL);

    g_signal_connect(yes_delete, "clicked", G_CALLBACK(yes_delete_clicked), NULL); 
    g_signal_connect(yes_add, "clicked", G_CALLBACK(yes_add_clicked), NULL);
    g_signal_connect(yes_update, "clicked", G_CALLBACK(yes_update_clicked), NULL);
    g_signal_connect(no_button, "clicked", G_CALLBACK(no_button_clicked), NULL);

    gtk_widget_show(window);    
    g_object_unref(builder);
    //Chay vong lap main den khi gtk_main_quit duoc goi            
    gtk_main();

    btcls(data);

    return 0;
}

int file_exists(char *pathname)
{
    FILE* f;

    f = fopen(pathname,"r");
    if (f == NULL) {
        return FALSE;
    }
    else {
        fclose(f);
    }
    return TRUE;
}
