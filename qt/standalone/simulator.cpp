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

#include "simulator.h"
#include <QX11Info>
#include <X11/extensions/XTest.h>

SimulatorException::SimulatorException( QString message )
{
    this->message = message;
}

const char* SimulatorException::what () const throw()
{
    return this->message.toLatin1();
}

Simulator::Simulator ()
{
    int event_base, error_base, major, minor;
    if ( !XTestQueryExtension(
             QX11Info::display(),
             &event_base, &error_base, &major, &minor) ) {
        throw new SimulatorException("Failed to initialize XTST.");
    }
}

void Simulator::keyPress( quint8 code )
{
    XTestFakeKeyEvent(
                QX11Info::display(),
                code, true, 0);
}

void Simulator::keyRelease( quint8 code )
{
    XTestFakeKeyEvent(
                QX11Info::display(),
                code, false, 0);
}
