/*
 * This module converts keysym values into the corresponding ISO 10646-1
 * (UCS, Unicode) values.
 */

#ifndef KEYSYM2UCS_H
#define KEYSYM2UCS_H

#include <X11/X.h>

long keysym2ucs(KeySym keysym);

#endif // KEYSYM2UCS_H
