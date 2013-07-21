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
	florence_signal_cb terminate, show, hide;
	guint terminate_id, show_id, hide_id;
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
	//florence->error=FLORENCE_FAIL;
	//if (florence->terminate) florence->terminate(NULL);
	//g_main_loop_quit(florence->loop);
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
florence_error florence_init()
{
	guint source_id;
	g_type_init();
	florence=(struct florence *)g_malloc(sizeof(struct florence));
	if (!florence) return FLORENCE_FAIL;
	memset(florence, 0, sizeof(struct florence));
	florence->dbus_id=g_bus_watch_name(G_BUS_TYPE_SESSION, "org.florence.Keyboard",
		G_BUS_NAME_WATCHER_FLAGS_NONE, florence_on_name_appeared,
		florence_on_name_vanished, florence, NULL);
	florence->loop = g_main_loop_new(NULL, FALSE);
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
florence_error florence_send(gchar *command, GVariant *parameters)
{
	guint source_id;
	if (florence) {
		if (florence->connection) {
			g_dbus_connection_call(florence->connection, "org.florence.Keyboard",
				"/org/florence/Keyboard", "org.florence.Keyboard", command,
				parameters, NULL, G_DBUS_CALL_FLAGS_NONE, -1, NULL, florence_done, NULL);
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
	return florence_send("show", NULL);
}

/* Hide the keyboard */
florence_error florence_hide()
{
	return florence_send("hide", NULL);
}

/* Move the keyboard */
florence_error florence_move(unsigned int x, unsigned int y)
{
	return florence_send("move", g_variant_new("(uu)", (guint)x, (guint)y));
}

/* Move the keyboard to near rect */
florence_error florence_move_to(unsigned int x, unsigned int y, unsigned int w, unsigned int h)
{
	return florence_send("move_to", g_variant_new("(uuuu)", (guint)x, (guint)y, (guint)w, (guint)h));
}

/* Callback called when a signal is received */
void florence_on_signal (GDBusConnection *connection, const gchar *sender_name,
		const gchar *object_path, const gchar *interface_name, const gchar *signal_name,
		GVariant *parameters, gpointer user_data)
{
	if (!florence) return;
	if (!strcmp(signal_name, "terminate")) {
		if (florence->terminate) florence->terminate(user_data);
	} else if (!strcmp(signal_name, "show")) {
		if (florence->show) florence->show(user_data);
	} else if (!strcmp(signal_name, "hide")) {
		if (florence->hide) florence->hide(user_data);
	} 
}

/* Register for signal */
florence_error florence_register(florence_signal signal, florence_signal_cb signalcb, void *user_data)
{
	guint *id=NULL;
	gchar *name=NULL;

	if (!florence) return FLORENCE_FAIL;
	if (!florence->connection) return FLORENCE_FAIL;

	switch(signal) {
		case FLORENCE_TERMINATE:
			florence->terminate = signalcb;
			id=&(florence->terminate_id);
			name="terminate";
			break;
		case FLORENCE_SHOW:
			florence->show = signalcb;
			id=&(florence->show_id);
			name="show";
			break;
		case FLORENCE_HIDE:
			florence->hide = signalcb;
			id=&(florence->hide_id);
			name="hide";
			break;
		default:
			return FLORENCE_FAIL;
	}

	if (*id) g_dbus_connection_signal_unsubscribe(florence->connection, *id);
	*id=g_dbus_connection_signal_subscribe(florence->connection, "org.florence.Keyboard",
		"org.florence.Keyboard", name, "/org/florence/Keyboard", NULL, G_DBUS_SIGNAL_FLAGS_NONE,
		florence_on_signal, user_data, NULL);

	return FLORENCE_SUCCESS;
}

