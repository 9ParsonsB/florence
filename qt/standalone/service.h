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

#ifndef SERVICE_H
#define SERVICE_H

#include <QObject>
#include "florence.h"
#include "settingservice.h"

#define SERVICE_NAME "org.florence.Keyboard"

/*! \class Service
  * \brief Implements the DBus interface of the keyboard.
  *
  * DBus methods are :
  * <ul>
  *     <li>show: displays the keyboard on screen.</li>
  *     <li>hide: hides the keyboard from screen.</li>
  * </ul>
  */
class Service : public QObject
{
    Q_OBJECT
public:
    /*! \fn Service()
      * \brief Constuctor.
      *
      * Instantiates a Service object.
      *
      * \param keyboard the Florence keyboard widget to modify.
      * \param settings the Settings object to register with DBus.
      */
    explicit Service(Florence *keyboard, SettingsService *settings, QObject *parent = 0);

    /*! \fn ~Service()
      * \brief Destructor.
      *
      * Destroys a Service object.
      */
    ~Service();

    /*! \fn listen()
      * \brief Listens to DBus at org.florence.Keyboard.
      */
    void listen();

public slots:
    Q_SCRIPTABLE void show();
    Q_SCRIPTABLE void hide();

private:
    Florence *keyboard;
    SettingsService *settings;
};

#endif // SERVICE_H
