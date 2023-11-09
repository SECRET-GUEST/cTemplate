#include "gui.h"
#include "error_handler.h"
#include "translator.h"
#include "program.h"
#include <gtk/gtk.h>
#include "path_helper.h"


// Forward declarations for signal handlers which will be implemented in program.c
void on_disk_selector_changed(GtkComboBoxText *combo_box, gpointer user_data);
void on_open_button_clicked(GtkButton *button, gpointer user_data);
void on_execute_button_clicked(GtkButton *button, gpointer user_data);

// Function to free resources when the application is closed
void on_application_destroy(GtkWidget *widget, gpointer user_data) {
    AppWidgets *appWidgets = (AppWidgets *)user_data;
    g_slice_free(AppWidgets, appWidgets);
}

void run_gui(int argc, char *argv[]) {
    // Initialize GTK+
    gtk_init(&argc, &argv);

    // Create a structure to store widget references.
    AppWidgets *appWidgets = g_slice_new(AppWidgets);

    // Create the main window
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), translate("gui.txt1"));  // "ISOMaker"
    gtk_window_set_default_size(GTK_WINDOW(window), 555, 108);
    g_signal_connect(window, "destroy", G_CALLBACK(on_application_destroy), appWidgets);




    /* STYLE */
    // Set window transparency
    gtk_widget_set_app_paintable(window, TRUE);
    GdkScreen *screen = gtk_widget_get_screen(window);
    GdkVisual *visual = gdk_screen_get_rgba_visual(screen);
    if (visual && gdk_screen_is_composited(screen)) {
        gtk_widget_set_visual(window, visual);
    }

    // Apply CSS styling
    GtkCssProvider *provider = gtk_css_provider_new();
    char* css_file_path = resource_path("src/gui/style/style.css");
    gtk_css_provider_load_from_path(provider, css_file_path, NULL);
    gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),
                                             GTK_STYLE_PROVIDER(provider),
                                             GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

    // Create a background box with the 'transparent-bg' class
    GtkWidget *background_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_style_context_add_class(gtk_widget_get_style_context(background_box), "transparent-bg");
    gtk_container_add(GTK_CONTAINER(window), background_box);



    /* WIDGETS */
    appWidgets->disk_entry = gtk_entry_new();
    gtk_widget_set_name(appWidgets->disk_entry, "disk_entry");
    gtk_entry_set_placeholder_text(GTK_ENTRY(appWidgets->disk_entry), translate("gui.txt2")); // "Select disk"  

    appWidgets->disk_selector_combo = gtk_combo_box_text_new();
    gtk_widget_set_name(appWidgets->disk_selector_combo, "disk_selector_combo");
    g_signal_connect(appWidgets->disk_selector_combo, "changed", G_CALLBACK(on_disk_selector_changed), appWidgets->disk_entry); 

    appWidgets->extract_checkbox = gtk_check_button_new_with_label(translate("gui.txt3")); // "Extract image to the disk"
    gtk_widget_set_name(appWidgets->extract_checkbox, "extract_checkbox");
    g_signal_connect(appWidgets->extract_checkbox, "toggled", G_CALLBACK(update_execute_button_label), appWidgets->execute_button); 

    appWidgets->image_entry = gtk_entry_new();
    gtk_widget_set_name(appWidgets->image_entry, "image_entry");
    gtk_entry_set_placeholder_text(GTK_ENTRY(appWidgets->image_entry), translate("gui.txt4")); // "Image path"  

    GtkWidget *open_button = gtk_button_new_with_label(translate("gui.txt5")); // "Open"
    gtk_widget_set_name(open_button, "open_button");
    g_signal_connect(open_button, "clicked", G_CALLBACK(on_open_button_clicked), appWidgets->image_entry);  

    appWidgets->execute_button = gtk_button_new_with_label(translate("gui.txt6")); // "Execute"
    gtk_widget_set_name(appWidgets->execute_button, "execute_button");
    g_signal_connect(appWidgets->execute_button, "clicked", G_CALLBACK(on_execute_button_clicked), appWidgets); 



    /* LAYOUTS */
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(background_box), vbox);

    GtkWidget *hbox_disk_selector = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_box_pack_start(GTK_BOX(hbox_disk_selector), appWidgets->disk_entry, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(hbox_disk_selector), appWidgets->disk_selector_combo, FALSE, FALSE, 0);

    GtkWidget *hbox_image_path = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_box_pack_start(GTK_BOX(hbox_image_path), appWidgets->image_entry, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(hbox_image_path), open_button, FALSE, FALSE, 0);

    // Adding widgets and layouts to the main window
    gtk_box_pack_start(GTK_BOX(vbox), hbox_disk_selector, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), appWidgets->extract_checkbox, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), hbox_image_path, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), appWidgets->execute_button, FALSE, FALSE, 0);




    /* LAUNCH */
    // Initializing the disk selector
    initialize_disk_selector(GTK_COMBO_BOX_TEXT(appWidgets->disk_selector_combo));

    // Displaying all the widgets in the window
    gtk_widget_show_all(window);

    // Starting the GTK+ main loop
    gtk_main();
}


// Helper function to update the execute button label based on the state of the checkbox
void update_execute_button_label(GtkToggleButton *checkbox, GtkWidget *execute_button) {
    const gchar *new_label;
    if (gtk_toggle_button_get_active(checkbox)) {
        new_label = translate("gui.txt7");  // "Extract to Disk"
    } else {
        new_label = translate("gui.txt8");  // "Extract Image"
    }
    gtk_button_set_label(GTK_BUTTON(execute_button), new_label);
}

// Function to be called when the execute button is clicked
void on_execute_button_clicked(GtkButton *button, gpointer user_data) {
    // Cast user_data to the struct containing the pointers to the widgets
    AppWidgets *widgets = (AppWidgets *)user_data;

    const gchar *disk_path = gtk_entry_get_text(GTK_ENTRY(widgets->disk_entry));
    const gchar *image_path = gtk_entry_get_text(GTK_ENTRY(widgets->image_entry));
    gboolean extract_to_disk = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widgets->extract_checkbox));

    // Validate the inputs
    if (disk_path == NULL || strlen(disk_path) == 0) {
        display_error_message(translate("gui.txt9"));  // "Disk path is empty."
        return;
    }
    if (image_path == NULL || strlen(image_path) == 0) {
        display_error_message(translate("gui.txt10"));  // "Image path is empty."
        return;
    }

    // Proceed with the image processing
    process_image(disk_path, image_path, extract_to_disk);
}

// Prototype for helper functions
void execute_command(const gchar *cmd, gchar **stdout_output, gchar **stderr_output, gint *exit_status);

// Function to be called when the disk selector combo box changes
void on_disk_selector_changed(GtkComboBoxText *combo_box, gpointer user_data) {
    GtkEntry *entry = GTK_ENTRY(user_data);
    const gchar *disk_path = gtk_combo_box_text_get_active_text(combo_box);
    if (disk_path != NULL) {
        gtk_entry_set_text(entry, disk_path);
    }
}

// Function to be called when the open button is clicked
void on_open_button_clicked(GtkButton *button, gpointer user_data) {
    GtkEntry *entry = GTK_ENTRY(user_data);
    GtkWidget *dialog = gtk_file_chooser_dialog_new(translate("gui.txt11"),  // "Open File"
                                                    GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(button))),
                                                    GTK_FILE_CHOOSER_ACTION_OPEN,
                                                    "_Cancel", GTK_RESPONSE_CANCEL,
                                                    "_Open", GTK_RESPONSE_ACCEPT,
                                                    NULL);

    GtkFileChooser *chooser = GTK_FILE_CHOOSER(dialog);
    gtk_file_chooser_set_local_only(chooser, TRUE);
    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
        char *filename = gtk_file_chooser_get_filename(chooser);
        gtk_entry_set_text(entry, filename);
        g_free(filename);
    }

    gtk_widget_destroy(dialog);
}


void initialize_disk_selector(GtkComboBoxText *combo_box) {
    FILE *fp;
    char path[1035];

    /* Open the command for reading. */
    fp = popen("/bin/lsblk -d --output NAME --noheadings", "r");
    if (fp == NULL) {
        // Handle error if popen failed
        fprintf(stderr, "Failed to run command\n");
        display_error_message(translate("gui.txt12"));  // "Unable to list disk devices."
        return;
    }

    /* Read the output a line at a time - output it. */
    while (fgets(path, sizeof(path), fp) != NULL) {
        // Remove newline character at the end if present
        size_t len = strlen(path);
        if (len > 0 && path[len-1] == '\n') path[len-1] = '\0';

        // Here we prepend /dev/ to the name to get the full path
        char full_path[1040];
        snprintf(full_path, sizeof(full_path), "/dev/%s", path);
        
        gtk_combo_box_text_append_text(combo_box, full_path);
    }

    /* close */
    pclose(fp);
}
