#ifndef D_CALLBACK_
#define D_CALLBACK_

#include <string.h>
#include <gtk/gtk.h>
#include "soundex.h"
#include "bt-5.0.0/inc/btree.h"

#define WORD_LENGHT 255
#define MEANING_LENGHT 10000

extern BTA *data;
extern GObject *window,
        *about_button, *delete_button, *update_button, *add_button, *search_entry,
        *info_bar, *notify_label, *yes_delete, *yes_add, *yes_update, *no_button,
        *word_label, *meaning_textview, *textbuffer, *status_label,
        *comple, *liststore;

void show_about_dialog(GtkButton *button, gpointer none);
void delete_button_clicked(GtkButton *button, gpointer none);
void update_button_clicked(GtkButton *button, gpointer none);
void add_button_clicked(GtkButton *button, gpointer none);
gboolean search_entry_key_press(GtkEntry *entry, GdkEvent *event, gpointer none);
void search_entry_activate(GtkEntry *entry, gpointer none);
void no_button_clicked(GtkButton *button, gpointer none);
void yes_delete_clicked(GtkButton *button, gpointer none);
void yes_add_clicked(GtkButton *button, gpointer none);
void yes_update_clicked(GtkButton *button, gpointer none);
int isBlank(char *text);
void find_next_word(char *word);
void separate_mean(char *mean);
int convert_text_to_bt(char *file);
int addWord(char *word, char *meaning);
int delWord(char *word);
int updateWord(char *word, char *meaning);
int prefix(const char *string1, const char *string2);
void show_completion(char *word);
void show_matching_soundex_word(char *word);

#endif