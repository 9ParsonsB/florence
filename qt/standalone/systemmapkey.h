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

#ifndef SYSTEMMAPKEY_H
#define SYSTEMMAPKEY_H

#include "symbol.h"
#include <QObject>
#include "systemmap.h"

/*! \class SystemMapKey
  * \brief This class associates a key code with zero, one or several symbols.
  *
  * The key properties are fetched from XKB.
  * \see Keymap
  */
class SystemMapKey : public KeymapKey
{
    Q_OBJECT
public:
    /*! \fn SystemMapKey()
      * \brief Constuctor.
      *
      * Instantiates a SystemMapKey object.
      */
    SystemMapKey(SystemMap *map, quint8 code);

    /*! \fn getSymbol( quint8 mod )
      * \brief Returns the Symbol associated with this key and the provided modifier.
      * \param mod Modifier applied to this key.
      * \returns A pointer to the Symbol associated with this key code and the provided modifier.
      * \see Symbol
      */
    Symbol *getSymbol( quint8 mod );

private:
    SystemMap *map;
    quint8 code;
    quint8 modsMask;

    ModifiedSymbol *generateSymbol( quint8 mod, quint8 group, quint8 level );
};

#endif // SYSTEMMAPKEY_H
