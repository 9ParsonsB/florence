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

#ifndef SILUMATOR_H
#define SIMULATOR_H

#include <QObject>
#include <exception>

/*! \exception SimulatorException
  * \brief Exception raised by Simulator objects.
  */
class SimulatorException : public std::exception
{
public:
    /*! \fn SimulatorException()
      * \brief Constuctor.
      *
      * Instantiates a SimulatorException object.
      */
    SimulatorException( QString message );

    /*! \fn what()
      * \brief Provides text information about the exception.
      */
    virtual const char* what() const throw();

private:
    QString message;
};

/*! \class Simulator
  * \brief This class simulates key events
  *
  * This class uses XTST to simulate keyboard events in X11.
  */
class Simulator : public QObject
{
    Q_OBJECT

public:
    /*! \fn Simulator()
      * \brief Constuctor.
      *
      * Instantiates a Simulator object.
      */
    Simulator();

public slots:
    /*! \fn keyPress()
      * \brief Simulate a key press event.
      *
      * Sends a key press event to X11, using XTST.
      * \param code hardware key code of the key.
      */
    void keyPress( quint8 code );
    /*! \fn keyRelease()
      * \brief Simulate a key press event.
      *
      * Sends a key release event to X11, using XTST.
      * \param code hardware key code of the key.
      */
    void keyRelease( quint8 code );
};

#endif
