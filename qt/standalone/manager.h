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

#ifndef MANAGER_H
#define MANAGER_H

#include <QObject>
#include "florence.h"

/*! \class Manager
  * \brief Implements keyboard user triggered actions
  */
class Manager : public QObject
{
    Q_OBJECT
public:
    /*! \fn Manager
      * \brief Constructor
      */
    explicit Manager( Florence *keyboard, QObject *parent = 0 );

signals:

public slots:
    /*! \fn actionTrigger
      * \brief Called when an action is triggered by the user
      */
    void actionTrigger( QString action );
    /*! \fn actionMove
      * \brief Called when the user moves the keyboard
      */
    void actionMove( int x, int y );

private:
    Florence *keyboard;
};

#endif // MANAGER_H
