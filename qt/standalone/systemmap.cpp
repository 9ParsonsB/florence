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

#include "florenceexception.h"
#include "systemmap.h"
#include "systemmapkey.h"

SystemMap::SystemMap() : Keymap()
{
    int rc=0;
    int maj=XkbMajorVersion;
    int min=XkbMinorVersion;
    int opcodeRtrn=0, errorRtrn=0;
    int baseEventCode;

    display = QX11Info::display();

    if ( !( rc = XkbLibraryVersion(&maj, &min) ) ) {
        throw new FlorenceException(tr( ("Failed to query XKB version. Version=" +
                                         QString::number(maj) + "." + QString::number(min) +
                                         " ; rc = " + QString::number(rc)).toLatin1() ));
    }

    if ( !( rc = XkbQueryExtension(display,
                                   &opcodeRtrn,
                                   &baseEventCode,
                                   &errorRtrn, &maj,
                                   &min) ) ) {
        throw new FlorenceException(tr( ("Failed to query XKB extension. Version=" +
                                         QString::number(maj) + "." + QString::number(min) +
                                         " ; rc = " + QString::number(rc)).toLatin1() ));
    }

    this->keyboardMap = XkbGetMap(display, XkbAllClientInfoMask|XkbKeyActionsMask|XkbModifierMapMask, XkbUseCoreKbd);
}

SystemMap::~SystemMap()
{
    XkbFreeClientMap(this->keyboardMap, XkbAllClientInfoMask, True);
}

bool SystemMap::load(Settings *settings)
{
    this->settings = settings;
    for ( int i = 0 ; i < 256 ; i++ ) {
        if ( this->keys[i] ) {
            delete this->keys[i];
        }
        if (i >= this->getDesc()->min_key_code && i <= this->getDesc()->max_key_code)
            this->keys[i] = new SystemMapKey(this, i);
        else
            this->keys[i] = NULL;
    }

    return true;
}

XkbDescPtr SystemMap::getDesc()
{
    return this->keyboardMap;
}

Display *SystemMap::getDisplay()
{
    return this->display;
}

Settings *SystemMap::getSettings()
{
    return this->settings;
}

Symbol *SystemMap::getSymbol( quint8 code )
{
    if ( this->keys[code] ) {
        return static_cast<SystemMapKey *>(this->keys[code])->getSymbol( this->modifier );
    } else return NULL;
}
