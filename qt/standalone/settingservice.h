
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
#define SETTINGS_FONT_DEFAULT "sans"
#define SETTINGS_FONT_SIZE "fontSize"
#define SETTINGS_FONT_SIZE_DEFAULT "16"
#define SETTINGS_POS "pos"
#define SETTINGS_POS_DEFAULT "0,0"

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
    Q_SCRIPTABLE void load();
    Q_SCRIPTABLE void save();
    Q_SCRIPTABLE void setStyle( QString styleFile );
    Q_SCRIPTABLE void setFont( QString font );
    Q_SCRIPTABLE void setFontSize( quint8 size );
    Q_SCRIPTABLE void setPos( QPoint pos );

private:
    Florence *keyboard;
    QString styleFile;
};

#endif // FLORENCE_SETTINGS_H
