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

#ifndef LIB_FLORENCE
#define LIB_FLORENCE

typedef enum { FLORENCE_SUCCESS=0, FLORENCE_FAIL, FLORENCE_TIMEOUT } florence_error;
typedef enum { FLORENCE_TERMINATE, FLORENCE_SHOW, FLORENCE_HIDE } florence_signal;
typedef void (*florence_signal_cb)(void *user_data);

/* Initialize the dbus connection */
florence_error florence_init();

/* Free memory */
florence_error florence_exit();

/* Show the keyboard */
florence_error florence_show();

/* Hide the keyboard */
florence_error florence_hide();

/* Move the keyboard */
florence_error florence_move(unsigned int x, unsigned int y);

/* Move the keyboard to near rect */
florence_error florence_move_to(unsigned int x, unsigned int y, unsigned int w, unsigned int h);

/* Register for signal */
florence_error florence_register(florence_signal signal, florence_signal_cb signalcb, void *user_data);

#endif

