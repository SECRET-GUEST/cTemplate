#ifndef PROGRAM_H
#define PROGRAM_H

#include <gtk/gtk.h>

void process_image(const gchar *disk_path, const gchar *image_path, gboolean extract_to_disk);
void execute_command(const gchar *cmd, gchar **stdout_output, gchar **stderr_output, gint *exit_status);

#endif // PROGRAM_H
