/*
Copyright (C) 2010 Srivats P.

This file is part of "Ostinato"

This is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>
*/

#include "mainwindow.h"
#include "../common/ostprotolib.h"
#include "../common/protocolmanager.h"
#include "../common/protocolwidgetfactory.h"
#include "params.h"
#include "preferences.h"
#include "settings.h"

#include <QApplication>
#include <QDateTime>
#include <QFile>
#include <QSettings>
#include <QtGlobal>
#include <QTextCodec>
#include <QTranslator>
#include <QLibraryInfo>

#include <google/protobuf/stubs/common.h>

extern const char* version;
extern const char* revision;
extern ProtocolManager *OstProtocolManager;
extern ProtocolWidgetFactory *OstProtocolWidgetFactory;

Params appParams;
QSettings *appSettings;
QMainWindow *mainWindow;

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    int exitCode;

    app.setApplicationName("NetTester");
    app.setOrganizationName("Academy");
    app.setProperty("version", version);
    app.setProperty("revision", revision);

    QTranslator qtHelpTranslator;
    qtHelpTranslator.load("qt_help_ru",QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    app.installTranslator(&qtHelpTranslator);

    QTranslator qtTranslator;
    qtTranslator.load("qt_ru",QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    app.installTranslator(&qtTranslator);

    QTextCodec::setCodecForTr(QTextCodec::codecForName ("utf8"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName ("utf8"));

    appParams.parseCommandLine(argc, argv);

    OstProtocolManager = new ProtocolManager();
    OstProtocolWidgetFactory = new ProtocolWidgetFactory();

    /* (Portable Mode) If we have a .ini file in the same directory as the 
       executable, we use that instead of the platform specific location
       and format for the settings */
    QString portableIni = QCoreApplication::applicationDirPath() 
            + "/ostinato.ini";
    if (QFile::exists(portableIni))
        appSettings = new QSettings(portableIni, QSettings::IniFormat);
    else
        appSettings = new QSettings();

    OstProtoLib::setExternalApplicationPaths(
        appSettings->value(kTsharkPathKey, kTsharkPathDefaultValue).toString(),
        appSettings->value(kGzipPathKey, kGzipPathDefaultValue).toString(),
        appSettings->value(kDiffPathKey, kDiffPathDefaultValue).toString(),
        appSettings->value(kAwkPathKey, kAwkPathDefaultValue).toString());

    Preferences::initDefaults();
    qsrand(QDateTime::currentDateTime().toTime_t());

    mainWindow = new MainWindow;
    mainWindow->show();
    exitCode =  app.exec();

    delete mainWindow;
    delete appSettings;
    delete OstProtocolManager;
    google::protobuf::ShutdownProtobufLibrary();

    return exitCode;
}
