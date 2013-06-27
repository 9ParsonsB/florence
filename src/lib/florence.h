/* 
 * florence - Florence is a simple virtual keyboard for Gnome.

 * Copyright (C) 2012-2013 François Agrech

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

#ifndef FLORENCE
#define FLORENCE

typedef enum { FLORENCE_SUCCESS=0, FLORENCE_FAIL, FLORENCE_TIMEOUT } florence_error;
typedef void (*terminate_func)();

/* Initialize the dbus connection */
florence_error florence_init(terminate_func terminate);

/* Free memory */
florence_error florence_exit();

/* Show the keyboard */
florence_error florence_show();

/* Hide the keyboard */
florence_error florence_hide();

#endif

