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
}

SettingsService::~SettingsService()
{

}

void SettingsService::load()
{
    QSettings settings(SETTINGS_ORGANISATION, SETTINGS_APPLICATION);
    this->setStyle(settings.value(SETTINGS_STYLE, this->styleFile).toString());
    this->setFont(settings.value(SETTINGS_FONT, SETTINGS_FONT_DEFAULT).toString());
    this->setFontSize(static_cast<quint8>(settings.value(SETTINGS_FONT_SIZE, SETTINGS_FONT_SIZE_DEFAULT).toInt()));
    this->setPos(settings.value(SETTINGS_POS, SETTINGS_POS_DEFAULT).toPoint());
    this->setSize(settings.value(SETTINGS_SIZE, SETTINGS_SIZE_DEFAULT).toSize());
}

void SettingsService::save()
{
    QSettings settings(SETTINGS_ORGANISATION, SETTINGS_APPLICATION);
    settings.setValue(SETTINGS_STYLE, this->styleFile);
    settings.setValue(SETTINGS_FONT, this->keyboard->getSettings()->getFont());
    settings.setValue(SETTINGS_FONT_SIZE, this->keyboard->getSettings()->getFontSize());
    settings.setValue(SETTINGS_POS, this->keyboard->pos());
    settings.setValue(SETTINGS_SIZE, this->keyboard->size());
}

void SettingsService::setStyle(QString styleFile)
{
    this->styleFile = styleFile;
    this->keyboard->setStyle(this->styleFile);
}

void SettingsService::setFont(QString font)
{
    this->keyboard->setFont(font);
}

void SettingsService::setFontSize(quint8 size)
{
    this->keyboard->setFontSize(size);
}

void SettingsService::setPos(QPoint pos)
{
    this->keyboard->move(pos);
}

void SettingsService::setSize(QSize size)
{
    QPoint pos = this->keyboard->pos();
    this->keyboard->setGeometry(pos.x(), pos.y(), size.width(), size.height());
}
