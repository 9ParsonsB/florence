//    This file is part of Florence Virtual Keyboard, QT version.
//    Copyright (C) 2014 François Agrech
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef SYSTEMLAYOUT_H
#define SYSTEMLAYOUT_H

#include "keymap.h"
#include <QX11Info>

extern "C" {
    #include <X11/X.h>
    #include <X11/XKBlib.h>
}

/*! \class SystemMap
  * \brief This class provides information about the system keyboad map layout
  *
  * This class uses XKB to find keyboard layout information
  */
class SystemMap : public Keymap
{
    Q_OBJECT
public:
    /*! \fn SystemMap()
      * \brief Constuctor.
      *
      * Instantiates a SystemLayout object.
      */
    SystemMap();

    /*! \fn ~SystemMap()
      * \brief Destructor.
      *
      * Destroys a SystemMap object.
      */
    ~SystemMap();

    /*! \fn load( Settings *settings )
      * \brief Loads the Keymap from XKB.
      * \param settings The settings object used to instantiate the symbols.
      * \returns true for success, false for failure.
      */
    bool load( Settings *settings );

    /*! \fn getKeySym()
      * \brief Get symbol from hardware key code.
      * \param code the hardware key code
      */
    QString getKeySym( quint8 code, quint8 modifierMask );

    /*! \fn getSymbol()
      * \brief Get symbol from hardware key code.
      * \param code the hardware key code
      */
    Symbol *getSymbol( quint8 code );

    /*! \fn getDesc()
      * \brief Get XKB description map.
      */
    XkbDescPtr getDesc();

    /*! \fn getDisplay()
      * \brief Get X11 Display object.
      */
    Display *getDisplay();

    /*! \fn getDisplay()
      * \brief Get Settings object.
      */
    Settings *getSettings();

private:
    Display *display;
    XkbDescPtr keyboardMap;
    Settings *settings;
};

#endif // SYSTEMLAYOUT_H
