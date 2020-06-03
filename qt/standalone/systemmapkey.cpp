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

#include "settings.h"
#include "systemmapkey.h"

extern "C" {
    #include "keysym2ucs.h"
}

SystemMapKey::SystemMapKey(SystemMap *map, quint8 code)
{
    this->code = code;
    this->map = map;
    this->locker = false;
    this->modifier = this->map->getDesc()->map->modmap[code];

    if ( XkbKeyAction( this->map->getDesc(), static_cast<KeyCode>(code), 0 ) ) {
        switch (XkbKeyAction(this->map->getDesc(), code, 0)->type) {
        case XkbSA_LockMods: this->locker = true; break;
        case XkbSA_SetMods: this->modifier = XkbKeyAction( this->map->getDesc(), code, 0 )->mods.mask;
            break;
        }
    }

    quint8 info = XkbKeyGroupInfo(this->map->getDesc(), code);
    unsigned int num_groups = XkbKeyNumGroups(this->map->getDesc(), code);

    quint8 group = 0x00;
    switch (XkbOutOfRangeGroupAction(info)) {
    case XkbRedirectIntoRange:
        group = XkbOutOfRangeGroupInfo(info);
        if (group >= num_groups) {
            group = 0;
        }
        break;
    case XkbClampIntoRange:
        group = static_cast<quint8>(num_groups - 1);
        break;
    case XkbWrapIntoRange:
    default:
        if (num_groups != 0) {
            group %= num_groups;
        }
        break;
    }

    XkbKeyTypePtr key_type = XkbKeyKeyType(this->map->getDesc(), code, group);
    this->modsMask = key_type->mods.mask;

    quint8 i, level = 0;
    this->symbols.append( this->generateSymbol( 0, group, level ) );
    for (i = 0; i < key_type->map_count; i++) {
        if ( key_type->map[i].active &&
             ( key_type->map[i].mods.mask & this->modsMask ) ) {
            level = key_type->map[i].level;
            this->symbols.append(
                        this->generateSymbol( key_type->map[i].mods.mask, group, level ) );
        }
    }
}

ModifiedSymbol *SystemMapKey::generateSymbol( quint8 mod, quint8 group, quint8 level )
{
    KeySym sym = XkbKeycodeToKeysym( this->map->getDisplay(), code, group, level );
    QString name = XKeysymToString( sym );

    Settings *settings = this->map->getSettings();
    Style *style = settings->getStyle();
    ModifiedSymbol *symbol = nullptr;
    if ( style->getSymbol( name ) ) {
        symbol = new ModifiedSymbol( name, mod, settings );
    } else {
        uint unicode = static_cast<uint>(keysym2ucs( sym ));
        if (unicode)
            symbol = new ModifiedSymbol( QString( QChar( unicode ) ), mod, settings );
        else
            symbol = new ModifiedSymbol( name, mod, settings );
    }
    return symbol;
}

Symbol *SystemMapKey::getSymbol( quint8 mod )
{
    quint8 mask = mod & this->modsMask;
    return KeymapKey::getSymbol( mask );
}
