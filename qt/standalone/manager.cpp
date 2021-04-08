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

#include <QWindow>
#include <QScreen>
#include <QDebug>
#include "manager.h"
#include "settingsdialog.h"

Manager::Manager( Florence *keyboard, SettingsService *settings, QObject *parent ) : QObject(parent)
{
    this->keyboard = keyboard;
    this->settings = settings;
}

void Manager::actionTrigger( QString action )
{
    if (action == QStringLiteral("move") || action == QStringLiteral("resize")) {
        this->settings->save();
    } else if (action == QStringLiteral("config")) {
        SettingsDialog *config = new SettingsDialog(this->keyboard);
        config->show();
    } else if (action == QStringLiteral("reduce")) {
        this->keyboard->showMinimized();
    } else {
        qDebug() << "trigger " << action;
    }
}

void Manager::actionMove( int dx, int dy )
{
    QScreen *screen = this->keyboard->window()->windowHandle()->screen();
    QRect geometry = screen->availableGeometry();
    QSize size = keyboard->size();
    QPoint pos = this->keyboard->mapToGlobal(this->keyboard->rect().topLeft());
    int x = pos.x() + dx;
    int y = pos.y() + dy;
    if (x < geometry.left()) {
        x = geometry.left();
    }
    if (x + size.width() > geometry.right()) {
        x = geometry.right() - size.width();
    }
    if (y < geometry.top()) {
        y = geometry.top();
    }
    if (y + size.height() > geometry.bottom()) {
        y = geometry.bottom() - size.height();
    }
    this->keyboard->setGeometry(x, y, size.width(), size.height());
}

void Manager::actionResize( int dw, int dh )
{
    QScreen *screen = this->keyboard->window()->windowHandle()->screen();
    QRect geometry = screen->availableGeometry();
    QPoint pos = this->keyboard->mapToGlobal(this->keyboard->rect().topLeft());
    QSize size = this->keyboard->size();
    int w = size.width() + dw;
    int h = size.height() + dh;
    if (w < 100) {
        w = 100;
    }
    if (pos.x() + w > geometry.right()) {
        w = geometry.right() - pos.x();
    }
    if (h < 50) {
        h = 50;
    }
    if (pos.y() + h > geometry.bottom()) {
        h = geometry.bottom() - pos.y();
    }
    this->keyboard->setGeometry(pos.x(), pos.y(), w, h);
}
