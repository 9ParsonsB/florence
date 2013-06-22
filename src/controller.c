/* 
 * florence - Florence is a simple virtual keyboard for Gnome.

 * Copyright (C) 2007-2013 François Agrech

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

#include "controller.h"
#include "trace.h"
#include "settings.h"
#include "tools.h"
#include <cairo-xlib.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/shape.h>

#ifdef ENABLE_AT_SPI2

void controller_icon_hide (GtkWidget *widget, gpointer user_data);
void controller_icon_show (GtkWidget *widget, gpointer user_data);

/* on expose event: display florence icon */
void controller_icon_expose (GtkWidget *window, cairo_t* context, void *userdata)
{
	START_FUNC
	cairo_t *mask_context;
	RsvgHandle *handle;
	GError *error=NULL;
	gdouble w, h;
	cairo_surface_t *mask=NULL;
	Pixmap shape;
	Display *disp=(Display *)gdk_x11_get_default_xdisplay();

	cairo_set_operator(context, CAIRO_OPERATOR_SOURCE);

	handle=rsvg_handle_new_from_file(ICONDIR "/florence.svg", &error);
	if (error) flo_error(_("Error loading florence icon: %s"), error->message);
	else {
		w=settings_get_double(SETTINGS_SCALEX)*2;
		h=settings_get_double(SETTINGS_SCALEY)*2;
		shape=XCreatePixmap(disp, GDK_WINDOW_XID(gtk_widget_get_window(window)), w, h, 1);
		mask=cairo_xlib_surface_create_for_bitmap(disp, shape,
			DefaultScreenOfDisplay(disp), w, h);
		mask_context=cairo_create(mask);
		cairo_set_source_rgba(mask_context, 0.0, 0.0, 0.0, 0.0);
		cairo_set_operator(mask_context, CAIRO_OPERATOR_SOURCE);
		cairo_paint(mask_context);
		cairo_set_operator(mask_context, CAIRO_OPERATOR_OVER);
		style_render_svg(mask_context, handle, w, h, TRUE, NULL);
		XShapeCombineMask(disp, GDK_WINDOW_XID(gtk_widget_get_window(window)),
			ShapeBounding, 0, 0, cairo_xlib_surface_get_drawable(mask), ShapeSet);

		cairo_set_source_rgba(context, 0.0, 0.0, 0.0, 100.0);
		cairo_set_operator(context, CAIRO_OPERATOR_DEST_OUT);
		cairo_paint(context);
		cairo_set_operator(context, CAIRO_OPERATOR_OVER);
		style_render_svg(context, handle, w, h, TRUE, NULL);
		cairo_destroy(mask_context);
		cairo_surface_destroy(mask);
		g_object_unref(G_OBJECT(handle));
	}
	END_FUNC
}

/* on button-press events: destroy the icon and show the actual keyboard */
void controller_icon_press (GtkWidget *window, GdkEventButton *event, gpointer user_data)
{
	START_FUNC
	struct controller *controller=(struct controller *)user_data;
	controller_icon_hide(NULL, (gpointer)controller);
	view_show(controller->status->view, controller->obj);
	END_FUNC
}

/* Show an intermediate icon before showing the keyboard (if intermediate_icon is activated) 
 * otherwise, directly show the keyboard */
void controller_show (struct controller *controller)
{
	START_FUNC
	if (!controller->status->view) return;
	if (gtk_widget_get_visible(GTK_WIDGET(controller->status->view->window)))
		view_hide(controller->status->view);
	if (settings_get_bool(SETTINGS_INTERMEDIATE_ICON)) {
		if (!controller->icon) {
			controller->icon=GTK_WINDOW(gtk_window_new(GTK_WINDOW_TOPLEVEL));
			gtk_window_set_keep_above(controller->icon, TRUE);
			gtk_window_set_skip_taskbar_hint(controller->icon, TRUE);
			gtk_widget_set_size_request(GTK_WIDGET(controller->icon), settings_get_double(SETTINGS_SCALEX)*2,
				settings_get_double(SETTINGS_SCALEY)*2);
			gtk_container_set_border_width(GTK_CONTAINER(controller->icon), 0);
			gtk_window_set_decorated(controller->icon, FALSE);
			gtk_window_set_position(controller->icon, GTK_WIN_POS_MOUSE);
			gtk_window_set_accept_focus(controller->icon, FALSE);
			gtk_widget_set_events(GTK_WIDGET(controller->icon),
				GDK_EXPOSURE_MASK|GDK_BUTTON_PRESS_MASK|GDK_BUTTON_RELEASE_MASK);
			g_signal_connect(G_OBJECT(controller->icon), "draw", G_CALLBACK(controller_icon_expose), controller);
			g_signal_connect(G_OBJECT(controller->icon), "button-press-event",
				G_CALLBACK(controller_icon_press), controller);
			g_signal_connect(G_OBJECT(controller->status->view->window), "show",
				G_CALLBACK(controller_icon_hide), controller);
			g_signal_connect(G_OBJECT(controller->status->view->window), "hide",
				G_CALLBACK(controller_icon_show), controller);
			g_signal_connect(G_OBJECT(controller->icon), "screen-changed",
				G_CALLBACK(view_screen_changed), NULL);
			view_screen_changed(GTK_WIDGET(controller->icon), NULL, NULL);
		}
		tools_window_move(controller->icon, controller->obj);
		gtk_widget_show(GTK_WIDGET(controller->icon));
	} else view_show(controller->status->view, controller->obj);
	END_FUNC
}

/* Called to hide the icon */
void controller_icon_hide (GtkWidget *widget, gpointer user_data)
{
	START_FUNC
	struct controller *controller=(struct controller *)user_data;
	if (controller->icon) {
		gtk_widget_hide(GTK_WIDGET(controller->icon));
	}
	END_FUNC
}

/* Called to show the icon */
void controller_icon_show (GtkWidget *widget, gpointer user_data)
{
	START_FUNC
	struct controller *controller=(struct controller *)user_data;
	if (controller->icon && controller->obj) {
		controller_show(controller);
	}
	END_FUNC
}

/* Called when a widget is focused.
 * Check if the widget is editable and show the keyboard or hide if not. */
void controller_focus_event (const AtspiEvent *event, void *user_data)
{
	START_FUNC
	struct controller *controller=(struct controller *)user_data;
	gboolean hide=FALSE;
	GError *error=NULL;

	AtspiStateSet *state_set=atspi_accessible_get_state_set(event->source);
	AtspiRole role=atspi_accessible_get_role(event->source, &error);
	if (error) flo_error(_("Event error: %s"), error->message);
	if (role==ATSPI_ROLE_TERMINAL ||
		(((role==ATSPI_ROLE_TEXT) || (role==ATSPI_ROLE_PASSWORD_TEXT)) &&
		 state_set && atspi_state_set_contains(state_set, ATSPI_STATE_EDITABLE))) {
		if (event->detail1) {
			if (controller->obj) g_object_unref(controller->obj);
			controller->obj = event->source;
			g_object_ref(controller->obj);
			controller_show(controller);
		} else {
			hide=TRUE;
		}
	} else {
		hide=TRUE;
	}
	if (hide) {
		view_hide(controller->status->view);
		controller_icon_hide(NULL, (gpointer)controller);
		if (controller->obj) g_object_unref(controller->obj);
		controller->obj=NULL;
	}
	END_FUNC
}

/* Called when a window is created */
void controller_window_create_event (const AtspiEvent *event, gpointer user_data)
{
	START_FUNC
	/* For some reason, focus state change does happen after traverse 
	 * ==> did I misunderstand? */
	/* TODO: remettre le keyboard au front. Attention: always_on_screen désactive cette fonction */
	// flo_traverse((struct florence *)user_data, event->source);
	END_FUNC
}

/* Registered the spi events to monitor focus and show on editable widgets. */
void controller_register_events (struct controller *controller)
{
	START_FUNC
	if (!status_spi_is_enabled(controller->status)) {
		flo_warn(_("SPI is disabled: Unable to switch auto-hide mode on."));
	} else {
		view_hide(controller->status->view);
		if (!atspi_event_listener_register_from_callback(controller_focus_event, (void*)controller, NULL, "object:state-changed:focused", NULL))
			flo_error(_("ATSPI listener register failed"));
		if (!atspi_event_listener_register_from_callback(controller_window_create_event, (void*)controller, NULL, "window:activate", NULL))
			flo_error(_("ATSPI listener register failed"));
	}
	END_FUNC
}

/* Deregistered the spi events. */
void controller_deregister_events (struct controller *controller)
{
	START_FUNC
	if (!atspi_event_listener_deregister_from_callback(controller_focus_event, (void*)controller, "object:state-changed:focused", NULL)) {
		flo_warn(_("AT SPI: problem deregistering focus listener"));
	}
	if (!atspi_event_listener_deregister_from_callback(controller_window_create_event, (void*)controller, "window:activate", NULL)) {
		flo_warn(_("AT SPI: problem deregistering window listener"));
	}
	controller_icon_hide(NULL, (gpointer)controller);
	view_show(controller->status->view, NULL);
	controller->obj=NULL;
	END_FUNC
}

/* Set auto hide mode on or off. */
void controller_set_mode (struct controller *controller, gboolean auto_hide)
{
	if (auto_hide) {
		controller_register_events(controller);
	} else {
		controller_deregister_events(controller);
	}
}

/* Triggered by gconf when the "auto_hide" parameter is changed. */
void controller_set_auto_hide(GSettings *settings, gchar *key, gpointer user_data)
{
	START_FUNC
	struct controller *controller=(struct controller *)user_data;
	controller_set_mode(controller, settings_get_bool(SETTINGS_AUTO_HIDE));
	if ((!settings_get_bool(SETTINGS_AUTO_HIDE)) && (controller->icon)) {
		gtk_widget_destroy(GTK_WIDGET(controller->icon));
		controller->icon=NULL;
	}
	END_FUNC
}

#endif

/* create a new instance of controller. */
struct controller *controller_new(struct status *status)
{
	START_FUNC
	struct controller *controller=(struct controller *)g_malloc(sizeof(struct controller));
	if (!controller) flo_fatal(_("Unable to allocate memory for the controller"));
	memset(controller, 0, sizeof(struct controller));

	controller->status=status;

#ifdef ENABLE_AT_SPI2
	if (atspi_init()) {
		flo_warn(_("AT-SPI has been disabled at run time: auto-hide mode is disabled."));
		status_spi_disable(status);
	}
	settings_changecb_register(SETTINGS_AUTO_HIDE, controller_set_auto_hide, controller);

	if (settings_get_bool(SETTINGS_HIDE_ON_START) && (!settings_get_bool(SETTINGS_AUTO_HIDE)))
		view_hide(status->view);
	else controller_set_mode(controller, settings_get_bool(SETTINGS_AUTO_HIDE));
#else
	flo_warn(_("AT-SPI has been disabled at compile time: auto-hide mode is disabled."));
	status_spi_disable(status);
#endif

	return controller;
}

/* liberate all the memory used by the controller */
void controller_free(struct controller *controller)
{
	START_FUNC

	controller->status->view = NULL;
	if (controller->icon) gtk_widget_destroy(GTK_WIDGET(controller->icon));
	controller->icon=NULL;
#ifdef ENABLE_AT_SPI2
	atspi_exit();
#endif

	g_free(controller);
	END_FUNC
}

