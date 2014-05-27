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

#ifndef FLORENCEEXCEPTION_H
#define FLORENCEEXCEPTION_H

#include <QString>
#include <exception>

/*! \exception FlorenceException
  * \brief Exception raised by Florence objects.
  */
class FlorenceException : public std::exception
{
public:
    /*! \fn FlorenceException()
      * \brief Constuctor.
      *
      * Instantiates a FlorenceException object.
      */
    FlorenceException( QString message );

    /*! \fn what()
      * \brief Provides text information about the exception.
      */
    virtual const char* what() const throw();

private:
    QString message;
};


#endif // FLORENCEEXCEPTION_H
