//    This file is part of Florence Virtual Keyboard, QT version.
//    Copyright (C) 2021 François Agrech
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
#include <QFontDialog>
#include "florenceexception.h"
#include "settingsdialog.h"
#include "service.h"

SettingsDialog::SettingsDialog( QWidget * parent )
    : QDialog(parent),
      ui(new Ui_config)
{
    ui->setupUi(this);

    if (!QDBusConnection::sessionBus().isConnected()) {
        throw new FlorenceException(tr("Cannot connect to the D-Bus session bus. To start it, run: eval `dbus-launch --auto-syntax`"));
    }

    this->dbus = new QDBusInterface(SERVICE_NAME, "/settings", "", QDBusConnection::sessionBus());
    if (!this->dbus->isValid()) {
        throw new FlorenceException(tr("Cannot connect to the Florence D-Bus service. Florence is not running"));
    }

    QDBusReply<QString> font = this->dbus->call("getFont");
    if (font.isValid()) {
        ui->font->setText(font.value());
    } else {
        qDebug() << font.error().message();
    }

    QDBusReply<bool> transparent = this->dbus->call("getTransparent");
    if (transparent.isValid()) {
        ui->transparent->setChecked(transparent.value());
    } else {
        qDebug() << font.error().message();
    }

    QDBusReply<bool> decorated = this->dbus->call("getDecorated");
    if (decorated.isValid()) {
        ui->decorated->setChecked(decorated.value());
    } else {
        qDebug() << font.error().message();
    }

    QObject::connect(ui->font, SIGNAL(clicked(bool)), this, SLOT(setFont()));
    QObject::connect(ui->transparent, SIGNAL(toggled(bool)), this, SLOT(setTransparent(bool)));
    QObject::connect(ui->decorated, SIGNAL(clicked(bool)), this, SLOT(setDecorated(bool)));

    QObject::connect(this, SIGNAL(finished(int)), this, SLOT(end(int)));
}

SettingsDialog::~SettingsDialog()
{
    delete this->dbus;
}

void SettingsDialog::setFont()
{
    bool ok;
    QFont original;
    original.fromString(ui->font->text().replace("&", ""));
    QFont font = QFontDialog::getFont(
                    &ok, original, this);
    if (ok) {
        QDBusReply<bool> reply = this->dbus->call("setFont", font.toString());
        if (reply.isValid()) {
            ui->font->setText(font.toString());
        } else {
            qDebug() << reply.error().message();
        }
    }
}

void SettingsDialog::setTransparent( bool transparent )
{
    QDBusReply<bool> reply = this->dbus->call("setTransparent", transparent);
    if (!reply.isValid()) {
        qDebug() << reply.error().message();
    }
}

void SettingsDialog::setDecorated( bool decorated )
{
    QDBusReply<bool> reply = this->dbus->call("setDecorated", decorated);
    if (!reply.isValid()) {
        qDebug() << reply.error().message();
    }
}

void SettingsDialog::end( int accept )
{
    if (accept) {
        QDBusReply<bool> reply = this->dbus->call("save");
        if (!reply.isValid()) {
            qDebug() << reply.error().message();
        }
    } else {
        QDBusReply<bool> reply = this->dbus->call("load");
        if (!reply.isValid()) {
            qDebug() << reply.error().message();
        }
    }
}
