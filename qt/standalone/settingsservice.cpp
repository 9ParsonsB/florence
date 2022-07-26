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

#include <QDebug>
#include "settingservice.h"

SettingsService::SettingsService(Florence *keyboard, QObject *parent) : QObject(parent)
{
    this->keyboard = keyboard;
    this->styleFile = SETTINGS_STYLE_DEFAULT;
    qRegisterMetaTypeStreamOperators<QStringList>("list");
}

SettingsService::~SettingsService()
{

}

bool SettingsService::load()
{
    QSettings settings(SETTINGS_ORGANISATION, SETTINGS_APPLICATION);
    this->setStyle(settings.value(SETTINGS_STYLE, this->styleFile).toString());
    this->setFont(settings.value(SETTINGS_FONT, SETTINGS_FONT_DEFAULT).toString());
    this->setPos(settings.value(SETTINGS_POS, SETTINGS_POS_DEFAULT).toPoint());
    this->setSize(settings.value(SETTINGS_SIZE, SETTINGS_SIZE_DEFAULT).toSize());
    this->setTransparent(settings.value(SETTINGS_TRANSPARENT, SETTINGS_TRANSPARENT_DEFAULT).toBool());
    this->setDecorated(settings.value(SETTINGS_DECORATED, SETTINGS_DECORATED_DEFAULT).toBool());
    this->setExtensions(settings.value(SETTINGS_EXTENSIONS, SETTINGS_EXTENSIONS_DEFAULT).toStringList());
    this->setOpacity(settings.value(SETTINGS_OPACITY, SETTINGS_OPACITY_DEFAULT).toReal());

    return true;
}

bool SettingsService::save()
{
    QSettings settings(SETTINGS_ORGANISATION, SETTINGS_APPLICATION);
    settings.setValue(SETTINGS_STYLE, this->styleFile);
    settings.setValue(SETTINGS_FONT, this->keyboard->getSettings()->getFont());
    settings.setValue(SETTINGS_POS, this->keyboard->geometry().topLeft());
    settings.setValue(SETTINGS_SIZE, this->keyboard->size());
    settings.setValue(SETTINGS_TRANSPARENT, this->getTransparent());
    settings.setValue(SETTINGS_DECORATED, this->getDecorated());
    settings.setValue(SETTINGS_EXTENSIONS, QVariant::fromValue(this->getExtensions()));
    settings.setValue(SETTINGS_OPACITY, this->getOpacity());

    return true;
}

void SettingsService::setStyle(QString styleFile)
{
    this->styleFile = styleFile;
    this->keyboard->setStyle(this->styleFile);
}

QString SettingsService::getFont()
{
    return this->keyboard->getSettings()->getFont();
}

bool SettingsService::setFont(QString font)
{
    this->keyboard->setFont(font);
    return true;
}

void SettingsService::setPos(QPoint pos)
{
    QSize size = this->keyboard->size();
    this->keyboard->setGeometry(pos.x(), pos.y(), size.width(), size.height());
}

void SettingsService::setSize(QSize size)
{
    QPoint pos = this->keyboard->geometry().topLeft();
    this->keyboard->setGeometry(pos.x(), pos.y(), size.width(), size.height());
}

bool SettingsService::setTransparent( bool transparent )
{
    this->keyboard->setAttribute(Qt::WA_TranslucentBackground, transparent);
    return true;
}

bool SettingsService::getTransparent()
{
    return this->keyboard->testAttribute(Qt::WA_TranslucentBackground);
}

bool SettingsService::setDecorated( bool decorated )
{
    if (decorated) {
        this->keyboard->setWindowFlags(this->keyboard->windowFlags() & (~Qt::FramelessWindowHint));
    } else {
        this->keyboard->setWindowFlags(this->keyboard->windowFlags() | Qt::FramelessWindowHint);
    }
    this->keyboard->show();
    return true;
}

bool SettingsService::getDecorated()
{
    return !(this->keyboard->windowFlags() & Qt::FramelessWindowHint);
}

bool SettingsService::setOpacity( qreal opacity )
{
    this->keyboard->setOpacity(opacity);
    return true;
}

qreal SettingsService::getOpacity()
{
    return this->keyboard->getSettings()->getOpacity();
}

bool SettingsService::setExtensions( QStringList extensions )
{
    this->keyboard->getSettings()->setExtensions(extensions.toVector());
    return true;
}

QStringList SettingsService::getExtensions()
{
    return this->keyboard->getSettings()->getExtensions().toList();
}
