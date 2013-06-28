/* 
 * florence - Florence is a simple virtual keyboard for Gnome.

 * Copyright (C) 2013 François Agrech

 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.  

*/

#include "system.h"
#include "florence.h"
#include <gio/gio.h>

struct florence {
	GMainLoop *loop;
	guint dbus_id;
	GDBusConnection *connection;
	terminate_func terminate;
	florence_error error;
};
struct florence *florence=NULL;

/* Called when dbus name has appeared */
void florence_on_name_appeared(GDBusConnection *con, const gchar *name, const gchar *name_owner, gpointer user_data)
{
	florence->error=FLORENCE_SUCCESS;
	florence->connection=con;
	g_main_loop_quit(florence->loop);
}

/* Called when dbus name has vanished */
void florence_on_name_vanished(GDBusConnection *con, const gchar *name, gpointer user_data)
{
	florence->error=FLORENCE_FAIL;
	if (florence->terminate) florence->terminate();
	g_main_loop_quit(florence->loop);
}

/* Exit main loop after timeout */
gboolean florence_timeout(gpointer user_data)
{
	florence->error=FLORENCE_TIMEOUT;
	g_main_loop_quit(florence->loop);
	return FALSE;
}

/* Exit the main loop when done */
void florence_done (GObject *source_object, GAsyncResult *res, gpointer user_data)
{
	florence->error=FLORENCE_SUCCESS;
	g_main_loop_quit(florence->loop);
}

/* Initialize the dbus connection */
florence_error florence_init(terminate_func terminate)
{
	guint source_id;
	g_type_init();
	florence=(struct florence *)g_malloc(sizeof(struct florence));
	if (!florence) return FLORENCE_FAIL;
	memset(florence, 0, sizeof(struct florence));
	florence->terminate=terminate;
	florence->dbus_id=g_bus_watch_name(G_BUS_TYPE_SESSION, "org.florence.Keyboard",
		G_BUS_NAME_WATCHER_FLAGS_NONE, florence_on_name_appeared,
		florence_on_name_vanished, florence, NULL);
	florence->loop = g_main_loop_new(NULL, FALSE);
	/* 3 seconds timeout */
	source_id=g_timeout_add_seconds(3, florence_timeout, florence);
	g_main_loop_run(florence->loop);
	g_source_remove(source_id);
	return florence->error;
}

/* Free memory */
florence_error florence_exit()
{
	if (!florence) return FLORENCE_FAIL;
	g_bus_unwatch_name(florence->dbus_id);
	g_main_loop_unref(florence->loop);
	g_free(florence);
	return FLORENCE_SUCCESS;
}

/* Send a dbus command to florence Keyboard object */
florence_error florence_send(gchar *command)
{
	guint source_id;
	if (florence) {
		if (florence->connection) {
			g_dbus_connection_call(florence->connection, "org.florence.Keyboard",
				"/org/florence/Keyboard", "org.florence.Keyboard", command,
				NULL, NULL, G_DBUS_CALL_FLAGS_NONE, -1, NULL, florence_done, NULL);
			/* 3 seconds timeout */
			source_id=g_timeout_add_seconds(3, florence_timeout, florence);
			g_main_loop_run(florence->loop);
			g_source_remove(source_id);
		} else return FLORENCE_FAIL;
	} else return FLORENCE_FAIL;
	return florence->error;
}

/* Show the keyboard */
florence_error florence_show()
{
	return florence_send("show");
}

/* Hide the keyboard */
florence_error florence_hide()
{
	return florence_send("hide");
}

