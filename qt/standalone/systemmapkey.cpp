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

#include "systemmapkey.h"

SystemMapKey::SystemMapKey(SystemMap *map, quint8 code, QObject *parent) :
    QObject(parent)
{
    this->map = map;
    this->locker = false;
    this->modifier = this->map->getDesc()->map->modmap[code];

    if ( XkbKeyAction( this->map->getDesc(), code, 0 ) ) {
        switch (XkbKeyAction(this->map->getDesc(), code, 0)->type) {
        case XkbSA_LockMods: this->locker = true; break;
        case XkbSA_SetMods: this->modifier = XkbKeyAction( this->map->getDesc(), code, 0 )->mods.mask;
            break;
        }
    }
}

quint8 SystemMapKey::getModifier()
{
    return this->modifier;
}

bool SystemMapKey::isLocker()
{
    return this->locker;
}

Symbol *SystemMapKey::getSymbol( quint8 mod )
{
    unsigned char info = XkbKeyGroupInfo(this->map->getDesc(), code);
    unsigned int num_groups = XkbKeyNumGroups(this->map->getDesc(), code);

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

    XkbKeyTypePtr key_type = XkbKeyKeyType(this->map->getDesc(), code, group);
    unsigned int active_mods = mod & key_type->mods.mask;

    int i, level = 0;
    for (i = 0; i < key_type->map_count; i++) {
        if (key_type->map[i].active && key_type->map[i].mods.mask == active_mods) {
            level = key_type->map[i].level;
        }
    }

    KeySym sym = XkbKeycodeToKeysym( this->map->getDisplay(), code, group, level );

    return NULL;
    //return XKeysymToString( sym );
    //return QString(QChar( (int)sym ));
}
