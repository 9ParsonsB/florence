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

SystemMap::SystemMap(QObject *parent) :
    QObject(parent)
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

    this->keyboardMap = XkbGetMap(display, XkbAllClientInfoMask, XkbUseCoreKbd);
}

SystemMap::~SystemMap(){
    XkbFreeClientMap(this->keyboardMap, XkbAllClientInfoMask, True);
}

QString SystemMap::getKeySym( quint8 code, quint8 modifierMask )
{
    unsigned char info = XkbKeyGroupInfo(this->keyboardMap, code);
    unsigned int num_groups = XkbKeyNumGroups(this->keyboardMap, code);

    unsigned int group = 0x00;
    switch (XkbOutOfRangeGroupAction(info)) {
    case XkbRedirectIntoRange:
        group = XkbOutOfRangeGroupInfo(info);
        if (group >= num_groups) {
            group = 0;
        }
        break;
    case XkbClampIntoRange:
        group = num_groups - 1;
        break;
    case XkbWrapIntoRange:
    default:
        if (num_groups != 0) {
            group %= num_groups;
        }
        break;
    }

    XkbKeyTypePtr key_type = XkbKeyKeyType(this->keyboardMap, code, group);
    unsigned int active_mods = modifierMask & key_type->mods.mask;

    int i, level = 0;
    for (i = 0; i < key_type->map_count; i++) {
        if (key_type->map[i].active && key_type->map[i].mods.mask == active_mods) {
            level = key_type->map[i].level;
        }
    }

    KeySym sym = XkbKeycodeToKeysym( display, code, group, level );
    //XKeysymToString( sym );
    return QString(QChar( (int)sym ));
}

XkbDescPtr SystemMap::getDesc()
{
    return this->keyboardMap;
}

Display *SystemMap::getDisplay()
{
    return this->display;
}
