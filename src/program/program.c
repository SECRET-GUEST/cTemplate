#include <gtk/gtk.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "program.h"
#include "error_handler.h"
#include "translator.h"


// Main processing function to handle image extraction or writing
void process_image(const gchar *disk_path, const gchar *image_path, gboolean extract_to_disk) {
    gchar *stdout_output = NULL;
    gchar *stderr_output = NULL;
    gint exit_status = 0;

    if (extract_to_disk) {
        // Example command to extract image to disk (dd command)
        gchar *cmd = g_strdup_printf("dd if='%s' of='%s' bs=4M status=progress", image_path, disk_path);
        execute_command(cmd, &stdout_output, &stderr_output, &exit_status);
        g_free(cmd);
    } else {
        // Example command to create image from disk and then compress it
        gchar *cmd_create = g_strdup_printf("dd if='%s' of='%s' bs=4M status=progress", disk_path, image_path);
        execute_command(cmd_create, &stdout_output, &stderr_output, &exit_status);
        g_free(cmd_create);

        if (exit_status == 0) { // Check if the dd command was successful
            gchar *cmd_compress = g_strdup_printf("gzip -f '%s'", image_path);
            execute_command(cmd_compress, &stdout_output, &stderr_output, &exit_status);
            g_free(cmd_compress);
        }
    }

    // Check the exit status and stderr_output for errors
    if (exit_status != 0) {
        // In case of an error, display the stderr_output to the user
        display_error_message(stderr_output);
    } else {
        // If successful, provide a message that the operation completed
        display_success_message(stdout_output);
    }

    // Free the allocated memory
    g_free(stdout_output);
    g_free(stderr_output);
}

// Helper function to execute system commands
void execute_command(const gchar *cmd, gchar **stdout_output, gchar **stderr_output, gint *exit_status) {
    GError *error = NULL;
    gboolean success = g_spawn_command_line_sync(cmd, stdout_output, stderr_output, exit_status, &error);

    if (!success) {
        g_warning("Error: %s", error->message);
        display_error_message(error->message);
        g_error_free(error);
    }
}

