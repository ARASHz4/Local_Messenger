#include "qtsingleapplication.h"

QtSingleApplication::QtSingleApplication(int &argc, char *argv[], bool GUIenabled) : QApplication(argc, argv, GUIenabled)
{
    sysInit();
}

QtSingleApplication::QtSingleApplication(const QString &appId, int &argc, char *argv[])  : QApplication(argc, argv)
{
    sysInit(appId);
}

#if QT_VERSION < 0x050000

QtSingleApplication::QtSingleApplication(int &argc, char **argv, Type type) : QApplication(argc, argv, type)
{
    sysInit();
}

#  if defined(Q_WS_X11)

QtSingleApplication::QtSingleApplication(Display* dpy, Qt::HANDLE visual, Qt::HANDLE cmap)
    : QApplication(dpy, visual, cmap)
{
    sysInit();
}

QtSingleApplication::QtSingleApplication(Display *dpy, int &argc, char **argv, Qt::HANDLE visual, Qt::HANDLE cmap)
    : QApplication(dpy, argc, argv, visual, cmap)
{
    sysInit();
}

QtSingleApplication::QtSingleApplication(Display* dpy, const QString &appId, int argc, char **argv, Qt::HANDLE visual, Qt::HANDLE cmap)
    : QApplication(dpy, argc, argv, visual, cmap)
{
    sysInit(appId);
}
#  endif // Q_WS_X11
#endif // QT_VERSION < 0x050000

void QtSingleApplication::sysInit(const QString &appId)
{
    actWin = 0;
    peer = new QtLocalPeer(this, appId);
    connect(peer, SIGNAL(messageReceived(const QString&)), SIGNAL(messageReceived(const QString&)));
}

bool QtSingleApplication::isRunning()
{
    return peer->isClient();
}

bool QtSingleApplication::sendMessage(const QString &message, int timeout)
{
    return peer->sendMessage(message, timeout);
}

QString QtSingleApplication::id() const
{
    return peer->applicationId();
}

void QtSingleApplication::setActivationWindow(QWidget* aw, bool activateOnMessage)
{
    actWin = aw;
    if (activateOnMessage)
        connect(peer, SIGNAL(messageReceived(const QString&)), this, SLOT(activateWindow()));
    else
        disconnect(peer, SIGNAL(messageReceived(const QString&)), this, SLOT(activateWindow()));
}

QWidget* QtSingleApplication::activationWindow() const
{
    return actWin;
}

void QtSingleApplication::activateWindow()
{
    if (actWin)
    {
        actWin->setWindowState(actWin->windowState() & ~Qt::WindowMinimized);
        actWin->raise();
        actWin->activateWindow();
        actWin->show();
    }
}

bool QtSingleApplication::event(QEvent *event)
{
    if (event->type() == QEvent::FileOpen)
    {
        QFileOpenEvent *openEvent = static_cast<QFileOpenEvent *>(event);

        mainWindow.loadFile(QStringList(openEvent->file()));
    }

    return QApplication::event(event);
}

void QtSingleApplication::startApp()
{
    if(appChackFiles())
    {
        SLSettings::LoadSettings();

        mainWindow.Start();

        if(arguments().count()>1)
        {
            for(int i=1; i<=arguments().count()-1; i++)
            {
                if(QFileInfo(arguments()[i]).isFile())
                {
                    filesAddress << arguments()[i];
                }
            }
            mainWindow.loadFile(filesAddress);
        }
    }
    else
    {
        std::exit(1);
    }
}

bool QtSingleApplication::appChackFiles()
{
    bool enable = false /*true false*/;

    if(enable)
    {
        QString Message;

        #if defined(Q_OS_WIN)
        if(!QFileInfo(QApplication::applicationDirPath() + "/Qt5Svg.dll").isFile())
        {
            Message = Message + "\n" + "Qt5Svg.dll";
        }

        if(!QFileInfo(QApplication::applicationDirPath() + "/imageformats/qgif.dll").isFile())
        {
            Message = Message + "\n" + "qgif.dll";
        }

        if(!QFileInfo(QApplication::applicationDirPath() + "/imageformats/qjpeg.dll").isFile())
        {
            Message = Message + "\n" + "qjpeg.dll";
        }

        if(!QFileInfo(QApplication::applicationDirPath() + "/imageformats/qsvg.dll").isFile())
        {
            Message = Message + "\n" + "qsvg.dll";
        }

        if(!QFileInfo(QApplication::applicationDirPath() + "/imageformats/qtga.dll").isFile())
        {
            Message = Message + "\n" + "qtga.dll";
        }

        if(!QFileInfo(QApplication::applicationDirPath() + "/imageformats/qtiff.dll").isFile())
        {
            Message = Message + "\n" + "qtiff.dll";
        }

        if(!QFileInfo(QApplication::applicationDirPath() + "/imageformats/qwbmp.dll").isFile())
        {
            Message = Message + "\n" + "qwbmp.dll";
        }

        if(!QFileInfo(QApplication::applicationDirPath() + "/imageformats/qwebp.dll").isFile())
        {
            Message = Message + "\n" + "qwebp.dll";
        }

        #elif defined(Q_OS_MAC)
        QStringList CD (QApplication::applicationDirPath().split("/"));
        QString Back;
        for(int i=0; i<CD.count()-1; i++)
        {
            if(i == 0)
            {
                Back = CD[i];
            }
            else
            {
                Back = Back + "/" + CD[i];
            }
        }
        QString PlugIns = Back + "/PlugIns";
        QString Frameworks = Back + "/Frameworks";

        if(!QDir(Frameworks + "/QtSvg.framework").exists())
        {
            Message = Message + "\n" + "QtSvg.framework";
        }

        if(!QFileInfo(PlugIns + "/imageformats/libqgif.dylib").isFile())
        {
            Message = Message + "\n" + "libqgif.dylib";
        }

        if(!QFileInfo(PlugIns + "/imageformats/libqjpeg.dylib").isFile())
        {
            Message = Message + "\n" + "libqjpeg.dylib";
        }

        if(!QFileInfo(PlugIns + "/imageformats/libqsvg.dylib").isFile())
        {
            Message = Message + "\n" + "libqsvg.dylib";
        }

        if(!QFileInfo(PlugIns + "/imageformats/libqtga.dylib").isFile())
        {
            Message = Message + "\n" + "libqtga.dylib";
        }

        if(!QFileInfo(PlugIns + "/imageformats/libqtiff.dylib").isFile())
        {
            Message = Message + "\n" + "libqtiff.dylib";
        }

        if(!QFileInfo(PlugIns + "/imageformats/libqwbmp.dylib").isFile())
        {
            Message = Message + "\n" + "libqwbmp.dylib";
        }

        if(!QFileInfo(PlugIns + "/imageformats/libqwebp.dylib").isFile())
        {
            Message = Message + "\n" + "libqwebp.dylib";
        }

        #elif defined(Q_OS_LINUX)
        if(!QFileInfo(QApplication::applicationDirPath() + "/lib/libQt5Svg.so.5").isFile())
        {
            Message = Message + "\n" + "libQt5Svg.so.5";
        }

        if(!QFileInfo(QApplication::applicationDirPath() + "/plugins/imageformats/libqgif.so").isFile())
        {
            Message = Message + "\n" + "libqgif.so";
        }

        if(!QFileInfo(QApplication::applicationDirPath() + "/plugins/imageformats/libqjpeg.so").isFile())
        {
            Message = Message + "\n" + "libqjpeg.so";
        }

        if(!QFileInfo(QApplication::applicationDirPath() + "/plugins/imageformats/libqsvg.so").isFile())
        {
            Message = Message + "\n" + "libqsvg.so";
        }

        if(!QFileInfo(QApplication::applicationDirPath() + "/plugins/imageformats/libqtga.so").isFile())
        {
            Message = Message + "\n" + "libqtga.so";
        }

        if(!QFileInfo(QApplication::applicationDirPath() + "/plugins/imageformats/libqtiff.so").isFile())
        {
            Message = Message + "\n" + "libqtiff.so";
        }

        if(!QFileInfo(QApplication::applicationDirPath() + "/plugins/imageformats/libqwbmp.so").isFile())
        {
            Message = Message + "\n" + "libqwbmp.so";
        }

        if(!QFileInfo(QApplication::applicationDirPath() + "/plugins/imageformats/libqwebp.so").isFile())
        {
            Message = Message + "\n" + "libqwebp.so";
        }
        #endif

        if(!Message.isEmpty())
        {
            QMessageBox msg;
            msg.setIcon(QMessageBox::Critical);
            msg.setWindowTitle(QApplication::applicationName() + " Error");
            msg.setWindowIcon(QIcon(":/Icon/Icons/Icon.svg"));
            msg.setText("The application can't start because some files is missing from your computer.");
            msg.setInformativeText("Try reinstalling the application to fix this problem.");
            msg.setDetailedText("This files is missing :" + Message);
            msg.setStandardButtons(QMessageBox::Ok);
            msg.setDefaultButton(QMessageBox::Ok);
            msg.exec();

            return false;
        }
        else
        {
            return true;
        }
    }
    else
    {
        return true;
    }
}
