/* 
   Florence - Florence is a simple virtual keyboard for Gnome.

   Copyright (C) 2008, 2009, 2010 François Agrech

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software Foundation,
   Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.  

*/

#ifndef FLO_STATUS
#define FLO_STATUS

#include "system.h"
#ifdef ENABLE_XTST
#include <X11/extensions/XTest.h>
#include <X11/extensions/record.h>
#include <gdk/gdkx.h>
#endif
#include <gtk/gtk.h>
#include "key.h"
#include "view.h"
#include "xkeyboard.h"

/* the type of the key calculated for the fsm table */
enum status_key_type {
	STATUS_KEY_NORMAL,
	STATUS_KEY_MODIFIER,
	STATUS_KEY_LOCKER,
	STATUS_KEY_TYPE_NUM
};

/* the type of event, used as input for the fsm table */
enum status_event {
	STATUS_PRESS, /* the press event must be sent */
	STATUS_RELEASE, /* the release event must be sent */
	STATUS_PRESSED, /* the press event has been sent */
	STATUS_RELEASED, /* the release event has been sent */
	STATUS_EVENT_NUM
};

/* describes a window that has the focus */
struct status_focus {
	Window w; /* window that has the focus */
	int revert_to; /* focus state of the focussed window */
};

/* This represents the status of florence */
struct status {
	struct key *focus; /* focus key (key located under the pointer) or NULL */
	gboolean focus_zoom; /* zoom the focused key (if composite screen or mask is disabled) */
	GTimer *timer; /* auto click timer: amount of time the mouse has been over the current key */
	struct key *pressed; /* key currently being pressed or NULL */
	GList *latched_keys; /* the list of all currently latched keys */
	GList *locked_keys; /* the list of all currently locked keys */
	GdkModifierType globalmod; /* global modifier mask */
	struct view *view; /* view to update on status change */
	gboolean spi; /* tell if spi events are enabled */
	gboolean moving; /* true when moving key is pressed */
	struct status_focus *w_focus; /* window that has the focus, or NULL */
#ifdef ENABLE_XRECORD
	XRecordContext RecordContext; /* Context to record keyboard events */
	Display *data_disp; /* Data display to record events */
	struct key *keys[256]; /* keys by keycode. used to look up for key. */
#endif
	struct xkeyboard *xkeyboard; /* data from xkb */
};

#ifdef ENABLE_XRECORD
/* Add keys to the keycode indexed keys */
void status_keys_add(struct status *status, GSList *keys);
#endif

/* update the focus key */
void status_focus_set(struct status *status, struct key *focus);
/* return the focus key */
struct key *status_focus_get(struct status *status);
/* return the currently pressed key */
struct key *status_pressed_get(struct status *status);
/* update the pressed key: send the press event and update the view 
 * if pressed is NULL, then release the last pressed key */
void status_pressed_set(struct status *status, struct key *pressed);
/* returns the key currently focussed */
struct key *status_hit_get(struct status *status, gint x, gint y);
/* Calculate single key status after key is pressed */
void status_key_press_update(struct status *status, struct key *key);
/* Calculate single key status after key is released */
void status_key_release_update(struct status *status, struct key *key);

/* start the timer */
void status_timer_start(struct status *status, GSourceFunc update, gpointer data);
/* stop the timer */
void status_timer_stop(struct status *status);
/* get timer value */
gdouble status_timer_get(struct status *status);

/* get the list of latched keys */
GList *status_list_latched(struct status *status);
/* get the list of locked keys */
GList *status_list_locked(struct status *status);

/* get and set the global modifier mask */
GdkModifierType status_globalmod_get(struct status *status);
void status_globalmod_set(struct status *status, GdkModifierType mod);

/* allocate memory for status */
struct status *status_new(const gchar *focus_back);
/* liberate status memory */
void status_free(struct status *status);
/* reset the status to its original state */
void status_reset(struct status *status);

/* sets the view to update on status change */
void status_view_set(struct status *status, struct view *view);

/* disable sending of spi events: send xtest events instead */
void status_spi_disable(struct status *status);
/* tell if spi is enabled */
gboolean status_spi_is_enabled(struct status *status);

/* set/get moving status */
void status_set_moving(struct status *status, gboolean moving);
gboolean status_get_moving(struct status *status);

/* get focussed window */
struct status_focus *status_w_focus_get(struct status *status);
/* zoom the focused key */
void status_focus_zoom_set(struct status *status, gboolean focus_zoom);
gboolean status_focus_zoom_get(struct status *status);

/* process the fsm actions and state change */
void status_fsm_process(struct status *status, struct key *key, enum status_event event);

#endif


