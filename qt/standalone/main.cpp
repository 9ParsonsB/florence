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

#include <QApplication>
#include <QCommandLineParser>
#include "qtextstream.h"
#include "simulator.h"
#include "florence.h"
#include "systemmap.h"
#include "service.h"
#include "settingservice.h"
#include "manager.h"
#include "settingsdialog.h"

void openConfig() {
    SettingsDialog *config = new SettingsDialog();
    config->show();
}

void run() {
    Simulator *simulator = new Simulator();
    Florence *keyboard = new Florence();
    SettingsService *settings = new SettingsService(keyboard);
    settings->load();
    Service *service = new Service(keyboard, settings);
    Manager *manager = new Manager(keyboard, settings);

    settings->load();

    QVector<QString> extensions;
    extensions << "actionkys";
    keyboard->getSettings()->setExtensions(extensions);
    SystemMap *map = new SystemMap();
    map->load(keyboard->getSettings());
    keyboard->setKeymap(map);

    QObject::connect( keyboard, SIGNAL(keyPressed(quint8)), simulator, SLOT(keyPress(quint8)) );
    QObject::connect( keyboard, SIGNAL(keyReleased(quint8)), simulator, SLOT(keyRelease(quint8)) );
    QObject::connect( keyboard, SIGNAL(actionTrigger(QString)), manager, SLOT(actionTrigger(QString)) );
    QObject::connect( keyboard, SIGNAL(actionMove(int, int)), manager, SLOT(actionMove(int, int)) );
    QObject::connect( keyboard, SIGNAL(actionResize(int, int)), manager, SLOT(actionResize(int, int)) );
    service->listen();

    keyboard->setWindowFlags(Qt::WindowStaysOnTopHint | Qt::WindowDoesNotAcceptFocus);

    keyboard->show();
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QApplication::setApplicationName("Florence");
    QApplication::setApplicationVersion("1.0");

    QCommandLineParser parser;
    parser.setApplicationDescription("Florence virtual keyboard");
    parser.addHelpOption();
    parser.addVersionOption();

    QCommandLineOption openSettings("c", QApplication::translate("main", "Open settings dialog"));
    parser.addOption(openSettings);

    parser.process(a);
    bool config = parser.isSet(openSettings);
    if (config) {
        openConfig();
    } else {
        run();
    }

    return a.exec();
}
