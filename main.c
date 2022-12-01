#include <gtk/gtk.h>

static gchar **args = NULL;

static GOptionEntry options[] = {
        { G_OPTION_REMAINING, 0, 0, G_OPTION_ARG_STRING_ARRAY, &args, "", "args" },
        { NULL }
};

int main(gint argc, gchar **argv)
{
        gtk_init_with_args(&argc, &argv, "", options, NULL, NULL);

	GtkWidget *win  = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	GtkWidget *vbox = gtk_vbox_new(FALSE, 0);
	GtkWidget *sock = gtk_socket_new();
	g_signal_connect(sock, "plug-removed", gtk_main_quit, NULL);
	g_signal_connect(win,  "delete-event", gtk_main_quit, NULL);
	gtk_widget_set_size_request(sock, 200, 200);
	gtk_box_pack_start(GTK_BOX(vbox), sock, TRUE,  TRUE, 0);
	gtk_container_add(GTK_CONTAINER(win), vbox);
	gtk_widget_show_all(win);

	/* Embed emacs */
	GdkNativeWindow id = gtk_socket_get_id(GTK_SOCKET(sock));
	gchar *command = g_strdup_printf("emacs --parent-id %d %s", id, g_strjoinv(NULL, args));
	g_spawn_command_line_async(command, NULL);

	/* Run */
	gtk_main();

	return 0;
}
