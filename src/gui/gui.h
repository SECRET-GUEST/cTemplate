#ifndef GUI_H
#define GUI_H

#include <gtk/gtk.h>

typedef struct {
    GtkWidget *disk_entry;
    GtkWidget *disk_selector_combo;
    GtkWidget *image_entry;
    GtkWidget *execute_button;
    GtkWidget *extract_checkbox;
} AppWidgets;

void run_gui(int argc, char *argv[]);
void update_execute_button_label(GtkToggleButton *checkbox, GtkWidget *execute_button);
void on_disk_selector_changed(GtkComboBoxText *combo_box, gpointer user_data);
void on_open_button_clicked(GtkButton *button, gpointer user_data);
void on_execute_button_clicked(GtkButton *button, gpointer user_data);
void initialize_disk_selector(GtkComboBoxText *combo_box);

#endif // GUI_H
