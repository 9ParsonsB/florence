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
}

void SettingsService::save()
{
    QSettings settings(SETTINGS_ORGANISATION, SETTINGS_APPLICATION);
    settings.setValue(SETTINGS_STYLE, this->styleFile);
    settings.setValue(SETTINGS_FONT, this->keyboard->getSettings()->getFont());
    settings.setValue(SETTINGS_FONT_SIZE, this->keyboard->getSettings()->getFontSize());
    settings.setValue(SETTINGS_POS, this->keyboard->pos());
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
