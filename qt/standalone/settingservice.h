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

#ifndef FLORENCE_SETTINGS_H
#define FLORENCE_SETTINGS_H

#include <QObject>
#include <QSettings>
#include "florence.h"

#define SETTINGS_ORGANISATION "Florence"
#define SETTINGS_APPLICATION "Keyboard"

#define SETTINGS_STYLE "style"
#define SETTINGS_STYLE_DEFAULT "default"
#define SETTINGS_FONT "font"
#define SETTINGS_FONT_DEFAULT "sans,12"
#define SETTINGS_POS "pos"
#define SETTINGS_POS_DEFAULT QPoint(0,0)
#define SETTINGS_SIZE "size"
#define SETTINGS_SIZE_DEFAULT QSize(640,200)
#define SETTINGS_TRANSPARENT "transparent"
#define SETTINGS_TRANSPARENT_DEFAULT false
#define SETTINGS_DECORATED "decorated"
#define SETTINGS_DECORATED_DEFAULT true
#define SETTINGS_EXTENSIONS "extensions"
#define SETTINGS_EXTENSIONS_DEFAULT {"actionkys"}
#define SETTINGS_OPACITY "opacity"
#define SETTINGS_OPACITY_DEFAULT 1.0

/*! \class Settings
  * \brief Stores application settings.
  */
class SettingsService : public QObject
{
    Q_OBJECT
public:
    /*! \fn SettingsService()
      * \brief Constuctor.
      *
      * Instantiates a SettingsService object.
      *
      * \param keyboard the Florence keyboard widget to apply the settings to.
      */
    explicit SettingsService( Florence *keyboard, QObject *parent = 0 );

    /*! \fn ~SettingsService()
      * \brief Destructor.
      *
      * Destroys a SettingsService object.
      */
    ~SettingsService();

public slots:
    Q_SCRIPTABLE bool load();
    Q_SCRIPTABLE bool save();
    Q_SCRIPTABLE void setStyle( QString styleFile );

    Q_SCRIPTABLE bool setFont( QString font );
    Q_SCRIPTABLE QString getFont();

    Q_SCRIPTABLE bool setTransparent( bool transparent );
    Q_SCRIPTABLE bool getTransparent();

    Q_SCRIPTABLE bool setDecorated( bool decorated );
    Q_SCRIPTABLE bool getDecorated();

    Q_SCRIPTABLE bool setOpacity( qreal opacity );
    Q_SCRIPTABLE qreal getOpacity();

    Q_SCRIPTABLE void setPos( QPoint pos );
    Q_SCRIPTABLE void setSize( QSize size );

    Q_SCRIPTABLE bool setExtensions( QStringList extensions );
    Q_SCRIPTABLE QStringList getExtensions();

private:
    Florence *keyboard;
    QString styleFile;
};

#endif // FLORENCE_SETTINGS_H
