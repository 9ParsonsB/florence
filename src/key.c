/* 
   Florence - Florence is a simple virtual keyboard for Gnome.

   Copyright (C) 2008 François Agrech

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

#include "system.h"
#include "key.h"
#include "trace.h"
#include <math.h>
#include <string.h>
#include <cspi/spi.h>

#define PI M_PI

/* instanciates a key
 * the key may have a static label which will be always drawn in place of the symbol */
struct key *key_new(void *userdata, guint code, GdkModifierType mod, gboolean lock, gdouble x,
	gdouble y, gdouble w, gdouble h, struct shape *shape)
{
	struct key *key=g_malloc(sizeof(struct key));
	memset(key, 0, sizeof(struct key));
	key->userdata=userdata;
	key->code=code;
	key->pressed=FALSE;
	key->modifier=mod;
	key->locker=lock;
	key->shape=shape;
	key->x=x;key->y=y;key->w=w;key->h=h;
	return key;
}

/* liberate memory used by the key */
void key_free(struct key *key)
{
	g_free(key);
}

/* Send a key press event */
void key_press(struct key *key, GList **pressedmodkeys, GdkModifierType *globalmod)
{
	GList *list=*pressedmodkeys;
	while (list) {
		SPI_generateKeyboardEvent(((struct key *)list->data)->code, NULL, SPI_KEY_PRESS);
		list=list->next;
	}
	SPI_generateKeyboardEvent(key->code, NULL, SPI_KEY_PRESS);
	list=*pressedmodkeys;
	while (list) {
		SPI_generateKeyboardEvent(((struct key *)list->data)->code, NULL, SPI_KEY_RELEASE);
		list=list->next;
	}
	if (key->locker) key->pressed=!key->pressed; else { 
		if (key_get_modifier(key)) {
			key->pressed=!key->pressed;
			if (key->pressed) *pressedmodkeys=g_list_prepend(*pressedmodkeys, key);
			else *pressedmodkeys=g_list_remove(*pressedmodkeys, key);
		} else {
			key->pressed=TRUE;
			list=*pressedmodkeys;
			while (list) {
				((struct key *)list->data)->pressed=FALSE;
				*globalmod&=~key_get_modifier((struct key *)list->data);
				list=g_list_delete_link(list, list);
			}
			*pressedmodkeys=NULL;
		}
	}
	if (key_get_modifier(key)) {
		if (key_is_pressed(key)) *globalmod|=key_get_modifier(key);
		else *globalmod&=~key_get_modifier(key);
	}
}

/* Send a key release event */
void key_release(struct key *key)
{
	SPI_generateKeyboardEvent(key->code, NULL, SPI_KEY_RELEASE);
	if (!key_get_modifier(key)) key->pressed=FALSE;
}

/* get keyval according to modifier */
guint key_getKeyval(struct key *key, GdkModifierType mod)
{
	guint keyval=0;
	if (!gdk_keymap_translate_keyboard_state(gdk_keymap_get_default(), key->code, mod, 0,
		&keyval, NULL, NULL, NULL)) {
		keyval=0;
		/*flo_warn(_("Unable to translate keyboard state: keycode=%d modifiers=%d"), key->code, mod);*/
	}
	return keyval;
}

/* draw the key to the hitmap of florence */
void key_hitmap_draw(struct key *key, guchar *hitmap, guint w, guint h, gdouble x, gdouble y, gdouble z)
{
	unsigned char *mask;
	cairo_surface_t *surface;
	int mstride;
	gint kx=(gint)(z*(x+key->x-(key->w/2.0))), ky=(gint)(z*(y+key->y-(key->h/2.0)));
	gint kw=(gint)(z*key->w), kh=(gint)(z*key->h);
	gint i, j;

	surface=style_shape_get_mask(key->shape, (guint)(z*key->w), (guint)(z*key->h));
	mask=cairo_image_surface_get_data(surface);
	mstride=cairo_image_surface_get_stride(surface);

	for (i=(kx<0?kw-kx:0);i<((kx+kw)>(gint)w?(gint)w-kx:kw);i++) {
		for (j=(ky<0?kh-ky:0);j<((ky+kh)>(gint)h?(gint)h-ky:kh);j++) {
			if (mask[i+(j*mstride)]>128) {
				hitmap[kx+i+(w*(ky+j))]=key->code;
			}
		}
	}
}

/* Draw the representation of the auto-click timer on the key
 * value is between 0 and 1 */
void key_timer_draw(struct key *key, struct style *style, cairo_t *cairoctx, gdouble z, double value)
{
	cairo_save(cairoctx);
	style_cairo_set_color(style, cairoctx, STYLE_MOUSE_OVER_COLOR);
	cairo_move_to(cairoctx, key->w/2.0, key->h/2.0);
	cairo_line_to(cairoctx, key->w/2.0, 0.0);
	if (value>0.125) cairo_line_to(cairoctx, key->w, 0.0);
	if (value>0.375) cairo_line_to(cairoctx, key->w, key->h);
	if (value>0.625) cairo_line_to(cairoctx, 0.0, key->h);
	if (value>0.875) cairo_line_to(cairoctx, 0.0, 0.0);
	if (value<0.125 || value>0.875) cairo_line_to(cairoctx, key->w/2+(key->w/2*tan(value*2.0*PI)), 0.0);
	else if (value>0.125 && value<0.375) cairo_line_to(cairoctx, key->w, key->h/2+(key->h/2*tan((value-0.25)*2.0*PI)));
	else if (value>0.375 && value<0.625) cairo_line_to(cairoctx, key->w/2-(key->w/2*tan((value-0.5)*2.0*PI)), key->h);
	else if (value>0.625 && value<0.875) cairo_line_to(cairoctx, 0.0, key->h/2-(key->h/2*tan((value-0.75)*2.0*PI)));
	cairo_close_path(cairoctx);
	cairo_clip(cairoctx);
	cairo_scale(cairoctx, 1.0/z, 1.0/z);
	cairo_mask_surface(cairoctx, style_shape_get_mask(key->shape, (guint)(z*key->w), (guint)(z*key->h)), 0.0, 0.0);
	cairo_restore(cairoctx);
}

/* Draw a colored layer on top of the shape */
void key_color_draw(struct key *key, struct style *style, cairo_t *cairoctx, gdouble z, enum style_colours c)
{
	cairo_save(cairoctx);
	style_cairo_set_color(style, cairoctx, c);
	cairo_scale(cairoctx, 1.0/z, 1.0/z);
	cairo_mask_surface(cairoctx, style_shape_get_mask(key->shape, (guint)ceil(z*key->w), (guint)ceil(z*key->h)), 0.0, 0.0);
	cairo_restore(cairoctx);
}

/* Draw the key with the cairo context of the keyboard 
 * the global modifier mod determines the symbol to draw on the key */
void key_draw(struct key *key, struct style *style, cairo_t *cairoctx, gdouble z, GdkModifierType mod,
	gdouble timer, gboolean activated)
{
	cairo_save(cairoctx);
	cairo_translate(cairoctx, key->x-(key->w/2.0), key->y-(key->h/2.0));
	style_shape_draw(key->shape, cairoctx, key->w, key->h);
	if (key->pressed) key_color_draw(key, style, cairoctx, z, STYLE_ACTIVATED_COLOR);
	if (activated) key_color_draw(key, style, cairoctx, z, STYLE_MOUSE_OVER_COLOR);
	if (timer>0.0) key_timer_draw(key, style, cairoctx, z, timer);
	style_symbol_draw(style, cairoctx, key_getKeyval(key, mod), key->w, key->h);
	cairo_restore(cairoctx);
}

/* getters and setters */
void key_set_pressed(struct key *key, gboolean pressed) { key->pressed=pressed; }
gboolean key_is_pressed(struct key *key) { return key->pressed; }
gboolean key_is_locker(struct key *key) { return key->locker; }
void *key_get_userdata(struct key *key) { return key->userdata; }
GdkModifierType key_get_modifier(struct key *key) { return key->modifier; }

