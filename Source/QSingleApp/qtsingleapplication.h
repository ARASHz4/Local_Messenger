#ifndef QTSINGLEAPPLICATION_H
#define QTSINGLEAPPLICATION_H

#include "qtlocalpeer.h"
#include "slsettings.h"
#include "messengerwindow.h"

#include <QApplication>
#include <cstdlib>
#include <QFileOpenEvent>
#include <QMessageBox>
#include <QFileInfo>

class QtLocalPeer;

#if defined(Q_OS_WIN)
#  if !defined(QT_QTSINGLEAPPLICATION_EXPORT) && !defined(QT_QTSINGLEAPPLICATION_IMPORT)
#    define QT_QTSINGLEAPPLICATION_EXPORT
#  elif defined(QT_QTSINGLEAPPLICATION_IMPORT)
#    if defined(QT_QTSINGLEAPPLICATION_EXPORT)
#      undef QT_QTSINGLEAPPLICATION_EXPORT
#    endif
#    define QT_QTSINGLEAPPLICATION_EXPORT __declspec(dllimport)
#  elif defined(QT_QTSINGLEAPPLICATION_EXPORT)
#    undef QT_QTSINGLEAPPLICATION_EXPORT
#    define QT_QTSINGLEAPPLICATION_EXPORT __declspec(dllexport)
#  endif
#else
#  define QT_QTSINGLEAPPLICATION_EXPORT
#endif

class QT_QTSINGLEAPPLICATION_EXPORT QtSingleApplication : public QApplication
{
    Q_OBJECT

public:
    QtSingleApplication(int &argc, char *argv[], bool GUIenabled = true);

    QtSingleApplication(const QString &id, int &argc, char *argv[]);

#if QT_VERSION < 0x050000
    QtSingleApplication(int &argc, char **argv, Type type);
#  if defined(Q_WS_X11)
    QtSingleApplication(Display* dpy, Qt::HANDLE visual = 0, Qt::HANDLE colormap = 0);
    QtSingleApplication(Display *dpy, int &argc, char **argv, Qt::HANDLE visual = 0, Qt::HANDLE cmap= 0);
    QtSingleApplication(Display* dpy, const QString &appId, int argc, char **argv, Qt::HANDLE visual = 0, Qt::HANDLE colormap = 0);
#  endif // Q_WS_X11
#endif // QT_VERSION < 0x050000

    bool isRunning();
    QString id() const;

    void setActivationWindow(QWidget* aw, bool activateOnMessage = true);
    QWidget* activationWindow() const;

    // Obsolete:
    void initialize(bool dummy = true)
        { isRunning(); Q_UNUSED(dummy) }

    MessengerWindow mainWindow;

private:
    void sysInit(const QString &appId = QString());

    QtLocalPeer *peer;

    QWidget *actWin;

    QStringList filesAddress;

protected:
    bool event(QEvent *event);

public Q_SLOTS:
    bool sendMessage(const QString &message, int timeout = 5000);

    void activateWindow();

    void startApp();

private slots:
    bool appChackFiles();

Q_SIGNALS:
    void messageReceived(const QString &message);

};

#endif // QTSINGLEAPPLICATION_H
