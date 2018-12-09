//    This file is part of Florence Virtual Keyboard, QT version.
//    Copyright (C) 2019 François Agrech
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


#include <QtDBus/QtDBus>
#include "service.h"
#include "florenceexception.h"

Service::Service(Florence *keyboard, SettingsService *settings, QObject *parent) : QObject(parent)
{
    this->keyboard = keyboard;
    this->settings = settings;
}

Service::~Service() {
    QDBusConnection::sessionBus().unregisterObject("/settings");
    QDBusConnection::sessionBus().unregisterObject("/");
    QDBusConnection::sessionBus().unregisterService(SERVICE_NAME);
}

void Service::listen() {
    if (!QDBusConnection::sessionBus().isConnected()) {
        throw new FlorenceException(tr("Cannot connect to the D-Bus session bus. To start it, run: eval `dbus-launch --auto-syntax`"));
    }

    if (!QDBusConnection::sessionBus().registerService(SERVICE_NAME)) {
        throw new FlorenceException(qPrintable(QDBusConnection::sessionBus().lastError().message()));
    }

    QDBusConnection::sessionBus().registerObject("/", this, QDBusConnection::ExportAllSlots);
    QDBusConnection::sessionBus().registerObject("/settings", settings, QDBusConnection::ExportAllSlots);
}

void Service::show() {
    keyboard->show();
}

void Service::hide() {
    keyboard->hide();
}

