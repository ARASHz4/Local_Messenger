#include "slsettings.h"

int SLSettings::language, SLSettings::port;
bool SLSettings::automaticLanguage, SLSettings::minimizeToTray, SLSettings::runStartUp;

SLSettings::SLSettings()
{

}

void SLSettings::setMinimizeToTray(bool value)
{
    minimizeToTray = value;
}

bool SLSettings::MinimizeToTray()
{
    return minimizeToTray;
}

void SLSettings::setRunStartUp(bool value)
{
    runStartUp = value;
}

bool SLSettings::RunStartUp()
{
    return runStartUp;
}

int SLSettings::Port()
{
    return port;
}

void SLSettings::setPort(int value)
{
    port = value;
}

void SLSettings::setLanguage(int value)
{
    language = value;
}

int SLSettings::Language()
{
    return language;
}

void SLSettings::setAutomaticLanguage(bool value)
{
    automaticLanguage = value;
}

bool SLSettings::AutomaticLanguage()
{
    return automaticLanguage;
}

void SLSettings::LoadSettings()
{
    QSettings Settings (QApplication::organizationName(), QApplication::applicationName());
    Settings.beginGroup("Options");

    //General
    {
        if((QString(Settings.value("MinimizeToTray").toString()).isEmpty())
                || (QString(Settings.value("MinimizeToTray").toString())!="true"
                    && QString(Settings.value("MinimizeToTray").toString())!="false"))
        {
            Settings.setValue("MinimizeToTray", "true");
        }
        SLSettings::setMinimizeToTray(Settings.value("MinimizeToTray").toBool());

        if((QString(Settings.value("RunStartUp").toString()).isEmpty())
                || (QString(Settings.value("RunStartUp").toString())!="true"
                    && QString(Settings.value("RunStartUp").toString())!="false"))
        {
            Settings.setValue("RunStartUp", "true");
        }
        SLSettings::setRunStartUp(Settings.value("RunStartUp").toBool());

        if(RunStartUp())
        {
            #if defined(Q_OS_WIN)
            QSettings settingAddStartUp("HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", QSettings::NativeFormat);
            settingAddStartUp.setValue(QApplication::applicationName(), "\"" +
                                       QDir::toNativeSeparators(QApplication::applicationFilePath()) + "\" " + "-start");
            #elif defined(Q_OS_MAC) && !defined(Q_OS_IOS)
            QFile file(QStandardPaths::writableLocation(QStandardPaths::HomeLocation) + "/Library/LaunchAgents/"
                       + QApplication::organizationName().toLower().replace(" ", "") + "."
                       + QApplication::applicationName().toLower().replace(" ", "") + ".plist");
            if(file.open(QIODevice::ReadWrite))
            {
                QTextStream stream(&file);
                stream << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
                          "\n<!DOCTYPE plist PUBLIC \"-//Apple//DTD PLIST 1.0//EN\" \"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">"
                          "\n<plist version=\"1.0\">"
                          "\n<dict>"
                          "\n	<key>Label</key>"
                          "\n	<string>" + QApplication::organizationName().toLower().replace(" ", "") + "."
                                + QApplication::applicationName().toLower().replace(" ", "") + "</string>"
                          "\n	<key>ProgramArguments</key>"
                          "\n	<array>"
                          "\n       <string>" + QApplication::applicationFilePath() + "</string>"
                          "\n       <string>-start</string>"
                          "\n   </array>"
                          "\n   <key>RunAtLoad</key>"
                          "\n   <true/>"
                          "\n</dict>"
                          "\n</plist>" << endl;
            }
            #elif defined(Q_OS_LINUX) && !defined(Q_OS_ANDROID)
            QFile file(QStandardPaths::writableLocation(QStandardPaths::HomeLocation) + "/.config/autostart/"
                       + QApplication::applicationName().replace(" ", "") + ".desktop");

            if(!QDir(QFileInfo(file).path()).exists())
            {
                QDir().mkdir(QFileInfo(file).path());
            }

            if(file.open(QIODevice::ReadWrite))
            {
                QTextStream stream(&file);
                stream << "[Desktop Entry]\nType=Application\nExec=" + QApplication::applicationDirPath() + "/"
                          + QApplication::applicationName().replace(" ", "") + ".sh -start" +
                          "\nHidden=false\nNoDisplay=false\nX-GNOME-Autostart-enabled=true\nName=" + QApplication::applicationName() << endl;
            }
            #endif
        }
    }

    //Messenger
    {
        if(!Settings.value("Port").toString().isEmpty() && Settings.value("Port").toInt() != 0)
        {
            SLSettings::setPort(Settings.value("Port").toInt());
        }
        else
        {
            SLSettings::setPort(4256);
            Settings.setValue("Port", SLSettings::Port());
        }
    }

    //Language
    {
        if(!Settings.value("Language").toString().isEmpty() &&
                (Settings.value("Language").toInt() == 0
                 || Settings.value("Language").toInt() == QLocale::English
                 || Settings.value("Language").toInt() == QLocale::Persian))
        {
            SLSettings::setLanguage(Settings.value("Language").toInt());
        }
        else
        {
            SLSettings::setLanguage(0);
            Settings.setValue("Language", SLSettings::Language());
        }

        QTranslator *Translator = new QTranslator;

        if(SLSettings::Language() == 0)
        {
            if(QLocale::system().language() == QLocale::English)
            {
                Translator->load(":/Language/Language/English.qm");
                QApplication::installTranslator(Translator);

                SLSettings::setLanguage(QLocale::English);
            }
            else if(QLocale::system().language() == QLocale::Persian)
            {
                Translator->load(":/Language/Language/Persian.qm");
                QApplication::installTranslator(Translator);

                SLSettings::setLanguage(QLocale::Persian);
            }
            else
            {
                Translator->load(":/Language/Language/English.qm");
                QApplication::installTranslator(Translator);

                SLSettings::setLanguage(QLocale::English);
            }

            SLSettings::setAutomaticLanguage(true);
        }
        else
        {
            if(SLSettings::Language() == QLocale::English)
            {
                Translator->load(":/Language/Language/English.qm");
                QApplication::installTranslator(Translator);
            }
            else if(SLSettings::Language() == QLocale::Persian)
            {
                Translator->load(":/Language/Language/Persian.qm");
                QApplication::installTranslator(Translator);
            }

            SLSettings::setAutomaticLanguage(false);
        }

        if(SLSettings::Language() == QLocale::English)
        {
            QApplication::setLayoutDirection(Qt::LeftToRight);
        }
        else if(SLSettings::Language() == QLocale::Persian)
        {
            QApplication::setLayoutDirection(Qt::RightToLeft);
        }
    }

    Settings.endGroup();
}

void SLSettings::SaveSettings()
{
    QSettings Settings (QApplication::organizationName(), QApplication::applicationName());
    Settings.beginGroup("Options");

    Settings.setValue("MinimizeToTray", SLSettings::MinimizeToTray());

    Settings.setValue("RunStartUp", SLSettings::RunStartUp());

    if(RunStartUp())
    {
        #if defined(Q_OS_WIN)
        QSettings settingAddStartUp("HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", QSettings::NativeFormat);
        settingAddStartUp.setValue(QApplication::applicationName(), "\"" +
                                   QDir::toNativeSeparators(QApplication::applicationFilePath()) + "\" " + "-start");
        #elif defined(Q_OS_MAC) && !defined(Q_OS_IOS)
        QFile file(QStandardPaths::writableLocation(QStandardPaths::HomeLocation) + "/Library/LaunchAgents/"
                   + QApplication::organizationName().toLower().replace(" ", "") + "."
                   + QApplication::applicationName().toLower().replace(" ", "") + ".plist");
        if(file.open(QIODevice::ReadWrite))
        {
            QTextStream stream(&file);
            stream << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
                      "\n<!DOCTYPE plist PUBLIC \"-//Apple//DTD PLIST 1.0//EN\" \"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">"
                      "\n<plist version=\"1.0\">"
                      "\n<dict>"
                      "\n	<key>Label</key>"
                      "\n	<string>" + QApplication::organizationName().toLower().replace(" ", "") + "."
                            + QApplication::applicationName().toLower().replace(" ", "") + "</string>"
                      "\n	<key>ProgramArguments</key>"
                      "\n	<array>"
                      "\n       <string>" + QApplication::applicationFilePath() + "</string>"
                      "\n       <string>-start</string>"
                      "\n   </array>"
                      "\n   <key>RunAtLoad</key>"
                      "\n   <true/>"
                      "\n</dict>"
                      "\n</plist>" << endl;
        }
        else
        {
            QMessageBox msg;
            msg.setIcon(QMessageBox::Warning);
            msg.setWindowTitle(QObject::tr("Startup Error"));
            msg.setWindowIcon(QIcon(":/Icon/Icons/Icon.svg"));
            msg.setText(QObject::tr("Can't save startup"));
            msg.setButtonText(QMessageBox::Ok, QObject::tr("OK"));
            msg.setStandardButtons(QMessageBox::Ok);
            msg.setDefaultButton(QMessageBox::Ok);
            msg.exec();
        }
        #elif defined(Q_OS_LINUX) && !defined(Q_OS_ANDROID)
        QFile file(QStandardPaths::writableLocation(QStandardPaths::HomeLocation)
                   + "/.config/autostart/" + QApplication::applicationName().replace(" ", "") + ".desktop");

        if(!QDir(QFileInfo(file).path()).exists())
        {
            QDir().mkdir(QFileInfo(file).path());
        }

        if(file.open(QIODevice::ReadWrite))
        {
            QTextStream stream(&file);
            stream << "[Desktop Entry]\nType=Application\nExec=" + QApplication::applicationDirPath() + "/"
                      + QApplication::applicationName().replace(" ", "") + ".sh -start"
                      + "\nHidden=false\nNoDisplay=false\nX-GNOME-Autostart-enabled=true\nName=" + QApplication::applicationName() << endl;
        }
        else
        {
            QMessageBox msg;
            msg.setIcon(QMessageBox::Warning);
            msg.setWindowTitle(QObject::tr("Startup Error"));
            msg.setWindowIcon(QIcon(":/Icon/Icons/Icon.svg"));
            msg.setText(QObject::tr("Can't save startup"));
            msg.setInformativeText(QObject::tr("Access denied :") + "\n"
                                   + QStandardPaths::writableLocation(QStandardPaths::HomeLocation) + "/.config/autostart/"
                                   + QApplication::applicationName().replace(" ", "") + ".desktop");
            msg.setButtonText(QMessageBox::Ok, QObject::tr("OK"));
            msg.setStandardButtons(QMessageBox::Ok);
            msg.setDefaultButton(QMessageBox::Ok);
            msg.exec();
        }
        #endif
    }
    else
    {
        #if defined(Q_OS_WIN)
        QSettings settingDeleteStartUp("HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", QSettings::NativeFormat);
        settingDeleteStartUp.remove(QApplication::applicationName());
        #elif defined(Q_OS_MAC) && !defined(Q_OS_IOS)
        QFile file(QStandardPaths::writableLocation(QStandardPaths::HomeLocation) + "/Library/LaunchAgents/"
                   + QApplication::organizationName().toLower().replace(" ", "") + "."
                   + QApplication::applicationName().toLower().replace(" ", "") + ".plist");
        if(file.open(QIODevice::ReadWrite))
        {
            QTextStream stream(&file);
            stream << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
                      "\n<!DOCTYPE plist PUBLIC \"-//Apple//DTD PLIST 1.0//EN\" \"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">"
                      "\n<plist version=\"1.0\">"
                      "\n<dict>"
                      "\n	<key>Label</key>"
                      "\n	<string>" + QApplication::organizationName().toLower().replace(" ", "") + "."
                            + QApplication::applicationName().toLower().replace(" ", "") + "</string>"
                      "\n	<key>ProgramArguments</key>"
                      "\n	<array>"
                      "\n       <string>" + QApplication::applicationFilePath() + "</string>"
                      "\n       <string>-start</string>"
                      "\n   </array>"
                      "\n   <key>RunAtLoad</key>"
                      "\n   <false/>"
                      "\n</dict>"
                      "\n</plist>" << endl;
        }
        else
        {
            QMessageBox msg;
            msg.setIcon(QMessageBox::Warning);
            msg.setWindowTitle(QObject::tr("Startup Error"));
            msg.setWindowIcon(QIcon(":/Icon/Icons/Icon.svg"));
            msg.setText(QObject::tr("Can't save startup"));
            msg.setButtonText(QMessageBox::Ok, QObject::tr("OK"));
            msg.setStandardButtons(QMessageBox::Ok);
            msg.setDefaultButton(QMessageBox::Ok);
            msg.exec();
        }
        #elif defined(Q_OS_LINUX) && !defined(Q_OS_ANDROID)
        QFile file(QStandardPaths::writableLocation(QStandardPaths::HomeLocation)
                   + "/.config/autostart/" + QApplication::applicationName().replace(" ", "") + ".desktop");

        if(!QDir(QFileInfo(file).path()).exists())
        {
            QDir().mkdir(QFileInfo(file).path());
        }

        if(file.open(QIODevice::ReadWrite))
        {
            QTextStream stream(&file);
            stream << "[Desktop Entry]\nType=Application\nExec=" + QApplication::applicationDirPath() + "/"
                      + QApplication::applicationName().replace(" ", "") + ".sh -start"
                      + "\nHidden=false\nNoDisplay=false\nX-GNOME-Autostart-enabled=false\nName=" + QApplication::applicationName() << endl;
        }
        else
        {
            QMessageBox msg;
            msg.setIcon(QMessageBox::Warning);
            msg.setWindowTitle(QObject::tr("Startup Error"));
            msg.setWindowIcon(QIcon(":/Icon/Icons/Icon.svg"));
            msg.setText(QObject::tr("Can't save startup"));
            msg.setInformativeText(QObject::tr("Access denied :") + "\n"
                                   + QStandardPaths::writableLocation(QStandardPaths::HomeLocation) + "/.config/autostart/"
                                   + QApplication::applicationName().replace(" ", "") + ".desktop");
            msg.setButtonText(QMessageBox::Ok, QObject::tr("OK"));
            msg.setStandardButtons(QMessageBox::Ok);
            msg.setDefaultButton(QMessageBox::Ok);
            msg.exec();
        }
        #endif
    }

    Settings.setValue("Port", SLSettings::Port());

    if(SLSettings::AutomaticLanguage())
    {
        Settings.setValue("Language", 0);
    }
    else
    {
        Settings.setValue("Language", SLSettings::Language());
    }

    Settings.endGroup();
}

std::tuple<int, int, int, int, int, bool, bool> SLSettings::LoadMainWindow()
{
    QRect ScreenSize = QApplication::desktop()->screenGeometry();

    QSettings Settings (QApplication::organizationName(), QApplication::applicationName());
    Settings.beginGroup("MainWindowSizePos");

    if(Settings.value("toolBarArea").toInt() != Qt::TopToolBarArea && Settings.value("toolBarArea").toInt() != Qt::RightToolBarArea
    && Settings.value("toolBarArea").toInt() != Qt::LeftToolBarArea && Settings.value("toolBarArea").toInt() != Qt::BottomToolBarArea)
    {
        Settings.setValue("toolBarArea", Qt::TopToolBarArea);
    }

    if((QString(Settings.value("window_max").toString()).isEmpty())
         || (QString(Settings.value("window_max").toString())!="true"
         && QString(Settings.value("window_max").toString())!="false"))
    {
        Settings.setValue("window_max", "false");
    }

    if((QString(Settings.value("window_fuls").toString()).isEmpty())
         || (QString(Settings.value("window_fuls").toString())!="true"
         && QString(Settings.value("window_fuls").toString())!="false"))
    {
        Settings.setValue("window_fuls", "false");
    }

    if((Settings.value("window_posx").toInt() != 0 && Settings.value("window_posy").toInt() != 0) &&
       (Settings.value("window_sizew").toInt() <= ScreenSize.width() && Settings.value("window_sizeh").toInt() <= ScreenSize.height()))
    {
        return std::make_tuple(Settings.value("window_posx").toInt(), Settings.value("window_posy").toInt(),
                               Settings.value("window_sizew").toInt(), Settings.value("window_sizeh").toInt(),
                               Settings.value("toolBarArea").toInt(), Settings.value("window_max").toBool(),
                               Settings.value("window_fuls").toBool());
    }
    else
    {
        return std::make_tuple(0, 0, 0, 0, Settings.value("toolBarArea").toInt(),
                               Settings.value("window_max").toBool(), Settings.value("window_fuls").toBool());
    }

    Settings.endGroup();
}

void SLSettings::SaveMainWindow(int x, int y, int w, int h, int toolBarArea, bool window_max, bool window_fuls)
{
    QSettings Settings (QApplication::organizationName(), QApplication::applicationName());

    Settings.beginGroup("MainWindowSizePos");

    if(window_max == false && window_fuls == false)
    {
        Settings.setValue("window_posx", x);
        Settings.setValue("window_posy", y);

        Settings.setValue("window_sizew", w);
        Settings.setValue("window_sizeh", h);
    }

    Settings.setValue("toolBarArea", toolBarArea);

    Settings.setValue("window_max", window_max);
    Settings.setValue("window_fuls", window_fuls);

    Settings.endGroup();
}

std::tuple<int, int, int, int> SLSettings::LoadOptionsWindow()
{
    QRect ScreenSize = QApplication::desktop()->screenGeometry();

    QSettings Settings (QApplication::organizationName(), QApplication::applicationName());
    Settings.beginGroup("OptionsWindowSizePos");

    if((Settings.value("window_posx").toInt() != 0 && Settings.value("window_posy").toInt() != 0) &&
       (Settings.value("window_sizew").toInt() <= ScreenSize.width() && Settings.value("window_sizeh").toInt() <= ScreenSize.height()))
    {
        return std::make_tuple(Settings.value("window_posx").toInt(), Settings.value("window_posy").toInt(),
                               Settings.value("window_sizew").toInt(), Settings.value("window_sizeh").toInt());
    }
    else
    {
        return std::make_tuple(0, 0, 0, 0);
    }
    Settings.endGroup();
}

void SLSettings::SaveOptionsWindow(int x, int y, int w, int h)
{
    QSettings Settings (QApplication::organizationName(), QApplication::applicationName());

    Settings.beginGroup("OptionsWindowSizePos");

    Settings.setValue("window_posx", x);
    Settings.setValue("window_posy", y);

    Settings.setValue("window_sizew", w);
    Settings.setValue("window_sizeh", h);

    Settings.endGroup();
}
