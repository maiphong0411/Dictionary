#include <string.h>
#include <gtk/gtk.h>
#include "dcallback.h"
#include "soundex.h"
#include "bt-5.0.0/inc/btree.h"
// show introduction "About"
void show_about_dialog(GtkButton *button, gpointer none)
{
    //GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file_at_size
    GtkWidget *dialog = gtk_about_dialog_new();

    gtk_about_dialog_set_program_name(GTK_ABOUT_DIALOG(dialog), "DICTIONARY");
    gtk_about_dialog_set_version(GTK_ABOUT_DIALOG(dialog), "v1.0");
    gtk_about_dialog_set_comments(GTK_ABOUT_DIALOG(dialog), "Grab");
    
    gtk_window_set_transient_for(dialog, window);
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}
//done
void delete_button_clicked(GtkButton *button, gpointer none)
{
    gtk_label_set_label(GTK_LABEL(notify_label), "Bạn có muốn xóa từ này?");
    gtk_widget_set_visible(GTK_WIDGET(info_bar), TRUE);
    gtk_widget_set_visible(GTK_WIDGET(yes_delete), TRUE);
    gtk_widget_set_visible(GTK_WIDGET(yes_add), FALSE);
    gtk_widget_set_visible(GTK_WIDGET(yes_update), FALSE);
}
//done
void update_button_clicked(GtkButton *button, gpointer none)
{
    gtk_text_view_set_editable(GTK_TEXT_VIEW(meaning_textview), TRUE);
    gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(meaning_textview), TRUE);
    gtk_label_set_label(GTK_LABEL(notify_label), "Bạn có muốn sửa từ này?");
    gtk_widget_set_visible(GTK_WIDGET(info_bar), TRUE);
    gtk_widget_set_visible(GTK_WIDGET(yes_update), TRUE);
    gtk_widget_set_visible(GTK_WIDGET(yes_delete), FALSE);
    gtk_widget_set_visible(GTK_WIDGET(yes_add), FALSE);
}
//done
void add_button_clicked(GtkButton *button, gpointer none)
{
    char *word = gtk_entry_get_text(GTK_ENTRY(search_entry));

    gtk_label_set_label(GTK_LABEL(word_label), word);

    gtk_text_buffer_set_text(GTK_TEXT_BUFFER(textbuffer), "", -1);
    gtk_text_view_set_buffer(GTK_TEXT_VIEW(meaning_textview), GTK_TEXT_BUFFER(textbuffer));
    gtk_text_view_set_editable(GTK_TEXT_VIEW(meaning_textview), TRUE);
    gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(meaning_textview), TRUE);

    gtk_label_set_label(GTK_LABEL(notify_label), "Bạn có muốn thêm từ này?");
    gtk_widget_set_visible(GTK_LABEL(status_label), FALSE);
    gtk_widget_set_visible(GTK_WIDGET(info_bar), TRUE);
    gtk_widget_set_visible(GTK_WIDGET(yes_add), TRUE);
    gtk_widget_set_visible(GTK_WIDGET(yes_delete), FALSE);
    gtk_widget_set_visible(GTK_WIDGET(yes_update), FALSE);
}
//done
gboolean search_entry_key_press(GtkEntry *entry, GdkEvent *event, gpointer none)
{
    GdkEventKey *pressed_key=(GdkEventKey*)event;
	char word[WORD_LENGHT];
	int length;

	strcpy(word,gtk_entry_get_text(GTK_ENTRY(entry))); // Lấy từ đang gõ trên thanh tìm kiếm 
	length=strlen(word);

	if(pressed_key->keyval == GDK_KEY_Tab) // nếu kí tự vừa nhập là Tab
	{
		gtk_widget_grab_focus(update_button);   // ??
        find_next_word(word);
	}
	else  //Nếu kí tự  không là backspace,enter,mũi tên lên xuốnng
	{
		if(pressed_key->keyval != GDK_KEY_BackSpace)
		{
			if((pressed_key->keyval != 65364)&&(pressed_key->keyval != 65362))
				word[length]=pressed_key->keyval;
			word[length+1]='\0';
		}
		else word[length-1]='\0';
		show_completion(word);
	}
	return FALSE;     //Return FALSE to allow signal to another widget
}
//done
void search_entry_activate(GtkEntry *entry, gpointer none)
{
    gtk_widget_set_visible(GTK_LABEL(status_label), FALSE);

    char word[WORD_LENGHT];
    char meaning[MEANING_LENGHT];

    strcpy(word, gtk_entry_get_text(search_entry));

    int rsize = 0;
    btsel(data, word, meaning, MEANING_LENGHT, &rsize);

    if (!rsize) {
        gtk_label_set_label(GTK_LABEL(word_label), "");
        gtk_text_buffer_set_text(GTK_TEXT_BUFFER(textbuffer), "", -1);
        gtk_text_view_set_buffer(GTK_TEXT_VIEW(meaning_textview), GTK_TEXT_BUFFER(textbuffer));
        gtk_label_set_label(GTK_LABEL(status_label), "Không tìm thấy");
        gtk_widget_set_visible(GTK_LABEL(status_label), TRUE);
        gtk_widget_set_sensitive(GTK_WIDGET(add_button), TRUE);
        gtk_widget_set_sensitive(GTK_WIDGET(update_button), FALSE);
        gtk_widget_set_sensitive(GTK_WIDGET(delete_button), FALSE);

        show_matching_soundex_word(word);
    }
    else {
        gtk_label_set_label(GTK_LABEL(word_label), word);
        gtk_text_buffer_set_text(GTK_TEXT_BUFFER(textbuffer), meaning, -1);
        gtk_text_view_set_buffer(GTK_TEXT_VIEW(meaning_textview), GTK_TEXT_BUFFER(textbuffer));
        gtk_text_view_set_editable(GTK_TEXT_VIEW(meaning_textview), FALSE);
        gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(meaning_textview), FALSE);
        gtk_widget_set_sensitive(GTK_WIDGET(add_button), FALSE);
        gtk_widget_set_sensitive(GTK_WIDGET(update_button), TRUE);
        gtk_widget_set_sensitive(GTK_WIDGET(delete_button), TRUE);
    }
}
//done
void no_button_clicked(GtkButton *button, gpointer none)
{
    gtk_text_view_set_editable(GTK_TEXT_VIEW(meaning_textview), FALSE);
    gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(meaning_textview), FALSE);

    gtk_widget_set_visible(GTK_WIDGET(info_bar), FALSE);
    gtk_widget_set_visible(GTK_WIDGET(status_label), FALSE);    
}
//done
void yes_delete_clicked(GtkButton *button, gpointer none)
{
    char *word = gtk_label_get_text(GTK_LABEL(word_label));
    static char notify[255];
    int fail = delWord(word);
    
    if (!fail) {
        strcpy(notify, "Đã xóa \"");
    }
    else {
        strcpy (notify, "Không thể xóa  \"");
    }
    strcat (notify, word);
    strcat (notify, "\"  khỏi từ điển!");

    gtk_label_set_label(GTK_LABEL(status_label), notify);
    gtk_widget_set_visible(GTK_LABEL(status_label), TRUE);
    gtk_widget_set_visible(GTK_WIDGET(info_bar), FALSE);
}
//done
void yes_add_clicked(GtkButton *button, gpointer none)
{
    char *word = gtk_label_get_text(GTK_LABEL(word_label));
    GtkTextIter startIter;
    GtkTextIter endIter;

    gtk_text_buffer_get_start_iter(textbuffer, &startIter);
    gtk_text_buffer_get_end_iter(textbuffer, &endIter);

    char *meaning = gtk_text_buffer_get_text(GTK_TEXT_BUFFER(textbuffer), &startIter, &endIter, FALSE);

    if (isBlank(meaning)) {
        gtk_label_set_label(GTK_LABEL(status_label), "Không được để trống");
        gtk_widget_set_visible(GTK_LABEL(status_label), TRUE);
        return;
    }

    int fail = addWord(word, meaning);
    static char notify[255];

    if (!fail) {
        strcpy(notify, "Đã thêm \"");
    }
    else {
        strcpy (notify, "Không thể thêm  \"");
    }
    strcat (notify, word);
    strcat (notify, "\"  vào từ điển!");

    gtk_label_set_label(GTK_LABEL(status_label), notify);
    gtk_widget_set_visible(GTK_LABEL(status_label), TRUE);
    gtk_text_view_set_editable(GTK_TEXT_VIEW(meaning_textview), FALSE);
    gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(meaning_textview), FALSE);
    gtk_widget_set_visible(GTK_WIDGET(info_bar), FALSE);
}
//done
void yes_update_clicked(GtkButton *button, gpointer none)
{
    char *word = gtk_label_get_text(GTK_LABEL(word_label));
    
    GtkTextIter startIter;
    GtkTextIter endIter;

    gtk_text_buffer_get_start_iter(textbuffer, &startIter);
    gtk_text_buffer_get_end_iter(textbuffer, &endIter);

    char *meaning = gtk_text_buffer_get_text(GTK_TEXT_BUFFER(textbuffer), &startIter, &endIter, FALSE);
    if (isBlank(meaning)) {
        gtk_label_set_label(GTK_LABEL(status_label), "Không được để trống");
        gtk_widget_set_visible(GTK_LABEL(status_label), TRUE);
        return;
    }

    int fail = updateWord(word, meaning);
    static char notify[255];

    if (!fail) {
        strcpy(notify, "Đã cập nhật \"");
    }
    else {
        strcpy (notify, "Không thể cập nhật  \"");
    }
    strcat (notify, word);
    strcat (notify, "\"  vào từ điển!");

    gtk_label_set_label(GTK_LABEL(status_label), notify);
    gtk_widget_set_visible(GTK_LABEL(status_label), TRUE);
    gtk_widget_set_visible(GTK_WIDGET(info_bar), FALSE);
    gtk_text_view_set_editable (GTK_TEXT_VIEW (meaning_textview), FALSE);
    gtk_text_view_set_cursor_visible (GTK_TEXT_VIEW (meaning_textview), FALSE);
}
//??
int isBlank(char *text)
{
    int k = 0;  //?
    int d = strlen(text);   //?
    while (k < d)
    {
        if (!((text[k] == ' ') || (text[k] == '\n')))
        {
            return 0;
        }
        else
            k++;
    }
    return 1;
}
//??
void find_next_word(char *word)
{
    int size, val;
    if (!bfndky(data, word, &size))
    {
        bnxtky(data, word, &val);
        gtk_entry_set_text(search_entry, word);
        search_entry_activate(search_entry, NULL);
    }
    else
    {
        char nextw[WORD_LENGHT];
        strcpy(nextw, word);
        btins(data, nextw, "", 1);
        bnxtky(data, nextw, &val);
        gtk_entry_set_text(search_entry, nextw);
        search_entry_activate(search_entry, NULL);
        btdel(data, word);
    }
}
//??
void separate_mean(char mean[MEANING_LENGHT])
{
    int m = 0;
    int n = 1;
    while (mean[n] != '\0')
    {
        if (mean[n] == '\\' && mean[n + 1] == 'n')
        {
            mean[m++] = '\n';
            n += 2;
        }
        else
        {
            if (m != n)
                mean[m++] = mean[n++];
            else
            {
                n++;
                m++;
            }
        }
    }
    mean[m] = '\0';
}

int convert_text_to_bt(char *file)
{
    FILE *data;
    if ((data = fopen(file, "r")) == NULL)
    {
        printf("Khong mo duoc file\n");
        return 0;
    }

    BTA *convert;
    convert = btcrt("resource.dat", 0, 0);  //?

    char word[WORD_LENGHT], mean[MEANING_LENGHT];
    int line;   //?

    while (fscanf(data, "%[^\t]", word) == 1)
    {
        fgets(mean, MEANING_LENGHT, data);
        line++;
        separate_mean(mean);
        btins(convert, word, mean, strlen(mean) + 1);
    }
    
    printf("%d lines.\n", line);
    btcls(convert);
    fclose(data);
    return 1;
}

//done
int addWord(char *word, char *meaning)
{
    int fail = btins(data, word, meaning, strlen(meaning) + 1);
    return fail;
}
//done
int delWord(char *word)
{
    int fail = btdel(data, word);
    return fail;
}
//done
int updateWord(char *word, char *meaning)
{
    int fail = btupd(data, word, meaning, strlen(meaning) + 1);
    return fail;
}

//Auto complete word
//done
// Kiem tra tinh prefix cua string1 vs string2(string1<string2)
int prefix(const char *string1, const char *string2)
{
	int length1 = strlen(string1);
	int length2 = strlen(string2);
	int index;

	if (length1 > length2)
		return 0;

	for (index = 0; index < length1; index++)
	{
		if (string1[index] != string2[index])
			return 0;
	}

	return 1;
}

//done
// Đưa ra gợi ý các từ dựa trên cơ sở phân tiền tố giống nhau
void show_completion(char *word)
{
    GtkTreeIter Iter;
    
    // delte all rows from liststore
    gtk_list_store_clear(liststore);

	char suggest_word[WORD_LENGHT];	
    int rsize;
	int existed = !bfndky(data, word, &rsize);// ham tim kiem word trong file data, rsize;
	
	if(existed)
	{
		gtk_list_store_append(liststore, &Iter);// add a new row
		gtk_list_store_set(liststore, &Iter, 0, word, -1);// them gia tri word vao cot 0
	}

	int index, number = 10; // so luong tu goi y
    strcpy(suggest_word, word);
	for(index = 0; (index <= number) && (number < 100); index++)
	{
        int value;
		if(!bnxtky(data, suggest_word, &value)){    // Tim kiem tu tiep theo cua word trong tu dien
            if(prefix(word, suggest_word))
			{
				gtk_list_store_append(liststore, &Iter);
				gtk_list_store_set(liststore, &Iter, 0, suggest_word, -1);
			}
            else number++;
        }  							
	}
}

// Hàm này dùng để in ra những từ đồng âm gợi ý khi nhập sai
/* Thuật toán soundex
	1.Lưu chữ cái đầu tiên trong xâu.
	2.Loại bỏ mọi chữ cái sau đây, trừ khi nó là chữ cái đầu tiên: a, e, h, i, o, u, w, y
	3.Gán các trọng số cho các chữ cái còn lại (sau chữ cái đầu tiên) như sau:
	  b, f, p, v = 1
	  c, g, j, k, q, s, x, z = 2
	  d, t = 3
	  l = 4,m, n = 5,r = 6
	4.. Nếu 2 hay nhiều chữ cái có trọng số giống nhau ở sát nhau trong từ ban đầu (trước bước 1), hay cách nhau chỉ bởi chữ h hay chữ w (chỉ có trong tiếng Mỹ), thì bỏ qua tất cả.
		Trả lại 4 ký tự đầu tiên, thêm 0 vào bên phải nếu ít hơn 4 ký tự được trả về.*/
//done
void show_matching_soundex_word(char *word)
{
	char previous_word[WORD_LENGHT];
	char next_word[WORD_LENGHT];
	char soundex_list[1000] = {0};
	char *soundex_code = soundex(word);
	int sum_word = 6;
	int index, value;
	int MAX = 100;

	// Tìm kiếm từ đồng âm trước và sau word
	for (index = 0; (index < sum_word) && (index < MAX); index++)
	{
		if (bnxtky(data, next_word, &value) == 0)
		{
			if (strcmp(soundex(next_word), soundex_code) == 0)
			{
				strcat(soundex_list, next_word);
				strcat(soundex_list, "\n");
			}
		}
		else
			sum_word++;
	}

	sum_word = 6;
	for (index = 0; (index < sum_word) && (index < MAX); index++)
	{
		if (bprvky(data, previous_word, &value) == 0)
		{
			if (strcmp(soundex(previous_word), soundex_code) == 0)
			{
				strcat(soundex_list, previous_word);
				strcat(soundex_list, "\n");
			}
		}
		else
			sum_word++;
	}

	// In ra soundex_list
	if (soundex_list[0] != 0)
	{
		//gtk_label_set_label: in ra một đoạn văn bản được truyền
		gtk_label_set_label(GTK_LABEL(status_label), "Từ bạn đang tìm có phải là: ");
		//void gtk_text_buffer_set_text (GtkTextBuffer *buffer, const gchar *text, gint len); xóa nội dung của buffer hiện tại và chèn nội dung tương ứng
		gtk_text_buffer_set_text(GTK_TEXT_BUFFER(textbuffer), soundex_list, -1);
		//Sets buffer as the buffer being displayed by text_view
		gtk_text_view_set_buffer(GTK_TEXT_VIEW(meaning_textview), GTK_TEXT_BUFFER(textbuffer));
	}
}
