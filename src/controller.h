/* 
   Florence - Florence is a simple virtual keyboard for Gnome.

   Copyright (C) 2007-2013 François Agrech

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

#ifndef CONTROLLER
#define CONTROLLER

#include "system.h"
#include "status.h"
#include "view.h"

#ifdef ENABLE_AT_SPI2
#include <dbus/dbus.h>
#include <atspi/atspi.h>
#endif

/* controller structure  */
struct controller {
#ifdef ENABLE_AT_SPI2
	GtkWindow *icon; /* intermediate icon */
	AtspiAccessible *obj; /* editable object being selected */
	gboolean atspi_enabled; /* Set to false to disable atspi. */
#endif
};

/* Set auto hide mode on or off. */
void controller_set_mode (struct controller *controller, gboolean auto_hide);

/* create a new instance of controller. */
struct controller *controller_new();
/* liberate all the memory used by the controller */
void controller_free(struct controller *controller);

#endif

