#ifndef SLSETTINGS_H
#define SLSETTINGS_H

#include <QSettings>
#include <QApplication>
#include <QDesktopWidget>
#include <QMessageBox>
#include <QStandardPaths>
#include <QTextStream>
#include <QIcon>
#include <QLocale>
#include <QTranslator>
#include <QDir>

class SLSettings
{
public:
    SLSettings();

    static void setMinimizeToTray(bool value);

    static bool MinimizeToTray();

    static void setRunStartUp(bool value);

    static bool RunStartUp();

    static int Port();

    static void setPort(int value);

    static void setLanguage(int value);

    static int Language();

    static void setAutomaticLanguage(bool value);

    static bool AutomaticLanguage();

    static void SaveSettings();

    static void LoadSettings();

    static std::tuple<int, int, int, int, int, bool, bool> LoadMainWindow();

    static void SaveMainWindow(int x, int y, int w, int h, int toolBarArea, bool window_max, bool window_fuls);

    static std::tuple<int, int, int, int> LoadOptionsWindow();

    static void SaveOptionsWindow(int x, int y, int w, int h);

private:
    static bool automaticLanguage, minimizeToTray, runStartUp;
    static int language, port;
};

#endif // SLSETTINGS_H
