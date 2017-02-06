#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "slsettings.h"
#include "options.h"
#include "about.h"

#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QDesktopServices>
#include <QTcpSocket>
#include <QTcpServer>
#include <QBuffer>
#include <QHostAddress>
#include <QShortcut>
#include <QFileDialog>
#include <QUuid>
#if defined(Q_OS_ANDROID)
#include <QAndroidJniObject>
#endif

namespace Ui {
class MessengerWindow;
}

class MessengerWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MessengerWindow(QWidget *parent = 0);
    ~MessengerWindow();

private:
    Ui::MessengerWindow *ui;

    QSystemTrayIcon *Tray;

    bool wasMax=false;

    QTcpSocket *MessengerSocketSender;
    QTcpSocket *MessengerSocketReceiver;
    QTcpServer *MessengerServer;

public slots:
    void Start();

    void loadFile(QStringList filesAddress);

private slots:
    void closeEvent(QCloseEvent *CloseEvant);

    void RestoreWindowTrigger(QSystemTrayIcon::ActivationReason RW);

    void on_actionRestoreWindow_triggered();

    void on_actionOptions_triggered();

    void on_actionExit_triggered();

    void on_actionHomePage_triggered();

    void on_actionAbout_triggered();

    void on_actionConnect_triggered();

    void ConnectToHost();

    void newConnection();

    void on_sendPushButton_clicked();

    void addMessage();

    void on_sendPhotoPushButton_clicked();
};

#endif // MAINWINDOW_H
