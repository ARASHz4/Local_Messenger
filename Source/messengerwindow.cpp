#include "messengerwindow.h"
#include "ui_messengerwindow.h"
#include "slsettings.h"
#include "connection.h"

#include <QDebug>

MessengerWindow::MessengerWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MessengerWindow)
{
    ui->setupUi(this);
}

MessengerWindow::~MessengerWindow()
{
    delete ui;
}

void MessengerWindow::Start()
{
    Tray = new QSystemTrayIcon;

    ui->retranslateUi(this);

    //Window Size & Post Setting
    {
        int x, y, w, h, toolBarArea;
        bool window_max, window_fuls;
        std::tie(x, y, w, h, toolBarArea, window_max, window_fuls) = SLSettings::LoadMainWindow();

        if(x != 0 || y != 0 || w != 0 || h != 0)
        {
            setGeometry(x, y, w, h);
        }

        addToolBar(Qt::ToolBarArea(toolBarArea), ui->mainToolBar);

        if(window_fuls && window_max)
        {
            wasMax=true;

            showFullScreen();
        }
        else
        {
            if(window_max)
            {
                showMaximized();
            }

            if(window_fuls)
            {
                showFullScreen();
            }
        }
    }

    //Tray
    {
        QList<QAction *> ActionList;
        QMenu *TrayMenu = new QMenu(this);

        ActionList <<ui->actionAbout <<TrayMenu->addSeparator() <<ui->actionRestoreWindow <<ui->actionExit;
        TrayMenu->addActions(ActionList);

        Tray->connect(Tray, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this,
                        SLOT(RestoreWindowTrigger(QSystemTrayIcon::ActivationReason)));

        Tray->connect(Tray, SIGNAL(messageClicked()), this, SLOT(on_actionRestoreWindow_triggered()));

        #if !defined(Q_OS_MAC)
        Tray->setIcon(QIcon(":/Icon/Icons/Icon.svg"));
        #else
        Tray->setIcon(QIcon(":/Icon/Icons/IconBW.svg"));
        #endif

        Tray->setContextMenu(TrayMenu);

        Tray->show();
    }

    //Shortcut
    {
        QShortcut *shortcutSand1 = new QShortcut(Qt::Key_Enter, this);
        connect(shortcutSand1, SIGNAL(activated()), this, SLOT(on_sendPushButton_clicked()));
        QShortcut *shortcutSand2 = new QShortcut(Qt::Key_Return, this);
        connect(shortcutSand2, SIGNAL(activated()), this, SLOT(on_sendPushButton_clicked()));
    }

    //Messenger
    {
        MessengerSocketSender = new QTcpSocket(this);
        MessengerSocketSender->connect(MessengerSocketSender, SIGNAL(connected()), this, SLOT(ConnectToHost()));

        MessengerServer = new QTcpServer(this);
        MessengerServer->listen(QHostAddress::AnyIPv4, SLSettings::Port());
        MessengerServer->connect(MessengerServer, SIGNAL(newConnection()), this, SLOT(newConnection()));
    }

    if(QApplication::arguments().count() >= 2)
    {
        if(QApplication::arguments()[1] != "-start")
        {
            show();
        }
    }
    else
    {
        show();
    }

    #if defined(Q_OS_ANDROID)
    QApplication::setStyle("Android");
    showMaximized();
    #endif
}

void MessengerWindow::loadFile(QStringList filesAddress)
{

}

void MessengerWindow::closeEvent(QCloseEvent *CloseEvant)
{
    if(SLSettings::MinimizeToTray())
    {
        CloseEvant->ignore();

        hide();
    }
    else
    {
        Tray->hide();

        if(!isFullScreen())
        {
            SLSettings::SaveMainWindow(geometry().x(), geometry().y(), geometry().width(),
                                       geometry().height(), toolBarArea(ui->mainToolBar), isMaximized(), isFullScreen());
        }
        else
        {
            SLSettings::SaveMainWindow(geometry().x(), geometry().y(), geometry().width(),
                                       geometry().height(), toolBarArea(ui->mainToolBar), wasMax, isFullScreen());
        }
    }
}

void MessengerWindow::RestoreWindowTrigger(QSystemTrayIcon::ActivationReason RW)
{
    if(RW == QSystemTrayIcon::DoubleClick)
    {
        on_actionRestoreWindow_triggered();
    }
}

void MessengerWindow::on_actionRestoreWindow_triggered()
{
    if(!isVisible())
    {
        show();
        activateWindow();
    }
    else if (isMinimized())
    {
        setWindowState(windowState() & ~Qt::WindowMinimized);
    }

    if (!isActiveWindow())
    {
        activateWindow();
    }
}

void MessengerWindow::on_actionOptions_triggered()
{
    Options OD(this);
    OD.exec();

    ui->retranslateUi(this);
}

void MessengerWindow::on_actionExit_triggered()
{
    Tray->hide();

    SLSettings::setMinimizeToTray(false);

    close();
}

void MessengerWindow::on_actionHomePage_triggered()
{
    QDesktopServices::openUrl(QUrl("https://arashz4.ga"));
}

void MessengerWindow::on_actionAbout_triggered()
{
    About AD(this);
    AD.exec();
}

void MessengerWindow::on_actionConnect_triggered()
{
    Connection CW(this);

    CW.exec();

    if(CW.Connected())
    {
        if(MessengerSocketSender->isOpen())
        {
            MessengerSocketSender->abort();
        }

        MessengerSocketSender->connectToHost(CW.IP(), SLSettings::Port());
    }
}

void MessengerWindow::ConnectToHost()
{
    ui->statusBar->showMessage(MessengerSocketSender->peerAddress().toString() + ":"
                               + QString::number(SLSettings::Port()) + " " + tr("Connected"), 0);
}

void MessengerWindow::newConnection()
{
    MessengerSocketReceiver = MessengerServer->nextPendingConnection();
    MessengerSocketReceiver->connect(MessengerSocketReceiver, SIGNAL(readyRead()), this, SLOT(addMessage()));

    ui->statusBar->showMessage(MessengerSocketReceiver->peerAddress().toString() + ":"
                               + QString::number(SLSettings::Port()) + " " + tr("Connected"), 0);

    if(!MessengerSocketSender->isOpen())
    {
        MessengerSocketSender->connectToHost(MessengerSocketReceiver->peerAddress(), SLSettings::Port());
    }
}

void MessengerWindow::on_sendPushButton_clicked()
{
    if(MessengerSocketSender->isOpen())
    {
        QString fontSize;
        #if !defined(Q_OS_ANDROID)
        fontSize = QString::number(ui->messageTextEdit->font().pointSize() + 2);
        #else
        fontSize = "";
        #endif

        if(!ui->sendLineEdit->text().isEmpty())
        {
            if(ui->messageTextEdit->toPlainText().isEmpty())
            {
                ui->messageTextEdit->setHtml("<p align=""right""><span style=""font-size:" + fontSize
                                             + "pt;color:#00bf00;"">" + ui->sendLineEdit->text() + "</span></p>");
            }
            else
            {
                ui->messageTextEdit->setHtml(ui->messageTextEdit->toHtml() + "<p align=""right""><span style=""font-size:"
                                             + fontSize + "pt;color:#00bf00;"">" + ui->sendLineEdit->text() + "</span></p>");
            }

            QTextCursor c =  ui->messageTextEdit->textCursor();
            c.movePosition(QTextCursor::End);
            ui->messageTextEdit->setTextCursor(c);

            MessengerSocketSender->write(ui->sendLineEdit->text().toUtf8());

            ui->sendLineEdit->setText(NULL);
        }
    }
    else
    {
        ui->statusBar->showMessage(tr("No Connection!"), 2000);
    }
}

void MessengerWindow::on_sendImagePushButton_clicked()
{
    if(MessengerSocketSender->isOpen())
    {
        QFileDialog OpenImage(this);
        OpenImage.setFileMode(QFileDialog::ExistingFile);
        OpenImage.setNameFilter(tr("Image") + " (*.png ; *.jpg ; *.jpeg ; *.bmp ; *.tif ; *.tiff ; *.webp ; *.svg ;"
                                              " *.gif ; *.xpm ; *.pnm ; *.ppm ; *.pgm ; *.wbmp ; *.xbm ; *.pbm);;");

        OpenImage.setWindowTitle(tr("Open Image"));

        OpenImage.setLabelText(QFileDialog::LookIn, tr("Look in:"));
        OpenImage.setLabelText(QFileDialog::FileType, tr("Files of type:"));
        OpenImage.setLabelText(QFileDialog::FileName, tr("File name:"));
        OpenImage.setLabelText(QFileDialog::Accept, tr("Open"));
        OpenImage.setLabelText(QFileDialog::Reject, tr("Cancel"));

        //Load Directory
        {
            QSettings SettingsAP (QApplication::organizationName(), QApplication::applicationName());
            SettingsAP.beginGroup("Directory");

            if (!SettingsAP.value("Directory").toString().isEmpty())
            {
                OpenImage.setDirectory(SettingsAP.value("Directory").toString());
            }
            else
            {
                OpenImage.setDirectory(QStandardPaths::writableLocation(QStandardPaths::HomeLocation));
            }

            SettingsAP.endGroup();
        }

        #if defined(Q_OS_ANDROID)
        OpenImage.showMaximized();
        #endif

        if (OpenImage.exec())
        {
            //Seve Directory
            {
                QSettings SettingsAP (QApplication::organizationName(), QApplication::applicationName());
                SettingsAP.beginGroup("Directory");
                SettingsAP.setValue("Directory", OpenImage.directory().path() );
                SettingsAP.endGroup();
            }

            if(ui->messageTextEdit->toPlainText() == "\n")
            {
                ui->messageTextEdit->setHtml("<p align=""right""><img src=\"" + OpenImage.selectedFiles()[0] + "\"/></p>");
            }
            else
            {
                ui->messageTextEdit->setHtml(ui->messageTextEdit->toHtml() + "<p align=""right""><img src=\""
                                             + OpenImage.selectedFiles()[0] + "\"/></p>");
            }

            QTextCursor c =  ui->messageTextEdit->textCursor();
            c.movePosition(QTextCursor::End);
            ui->messageTextEdit->setTextCursor(c);

            QImage image(OpenImage.selectedFiles()[0]);
            QByteArray ba;
            QBuffer buffer(&ba);

            image.save(&buffer, "JPG");

            MessengerSocketSender->write(ba);
        }
    }
    else
    {
        ui->statusBar->showMessage(tr("No Connection!"), 2000);
    }
}

void MessengerWindow::addMessage()
{
    QByteArray data = MessengerSocketReceiver->readAll();

    QImage receiveImage;
    receiveImage.loadFromData(data);

    if(!receiveImage.isNull())
    {
        QFile saveFile(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)
                       + QDir::separator() + QUuid::createUuid().toString() + ".jpg");

        QPixmap SaveImage = QPixmap::fromImage(receiveImage);

        if(!QDir(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)).exists())
        {
            if(!QDir(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)).cdUp())
            {
                QStringList CD (QFileInfo(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)).absolutePath().split("/"));
                QString Back;
                for(int i=0; i<CD.count(); i++)
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

                QDir().mkdir(Back);
                QDir().mkdir(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
            }
            else
            {
                QDir().mkdir(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
            }
        }

        if(saveFile.open(QIODevice::ReadWrite))
        {
            if(SaveImage.save(&saveFile, "JPG"))
            {
                if(ui->messageTextEdit->toPlainText() == "\n")
                {
                    ui->messageTextEdit->setHtml("<p align=""left""><img src=\"" + saveFile.fileName() + "\"/></p>");
                }
                else
                {
                    ui->messageTextEdit->setHtml(ui->messageTextEdit->toHtml() + "<p align=""left""><img src=\""
                                                 + saveFile.fileName() + "\"/></p>");
                }

                #if !defined(Q_OS_ANDROID)
                Tray->showMessage(tr("New message"), tr("Image"), QSystemTrayIcon::NoIcon);
                #else
                QAndroidJniObject AndroidNotification = QAndroidJniObject::fromString(tr("Image"));
                QAndroidJniObject::callStaticMethod<void>("com/arashz4/localmessenger/NotificationClient", "showNotification",
                                                          "(Ljava/lang/String;)V", AndroidNotification.object<jstring>());
                #endif
            }
        }
    }
    else
    {
        QString Message = QString(data);

        QString fontSize;
        #if !defined(Q_OS_ANDROID)
        fontSize = QString::number(ui->messageTextEdit->font().pointSize() + 2);
        #else
        fontSize = "";
        #endif

        if(ui->messageTextEdit->toPlainText() == "\n")
        {
            ui->messageTextEdit->setHtml("<p align=""left""><span style=""font-size:" + fontSize
                                         + "pt;color:#0055ff;"">" + Message + "</span></p>");
        }
        else
        {
            ui->messageTextEdit->setHtml(ui->messageTextEdit->toHtml() + "<p align=""left""><span style=""color:#0055ff;"">"
                                         + Message + "</span></p>");
        }

        #if !defined(Q_OS_ANDROID)
        Tray->showMessage(tr("New message"), Message, QSystemTrayIcon::NoIcon);
        #else
        QAndroidJniObject AndroidNotification = QAndroidJniObject::fromString(Message);
        QAndroidJniObject::callStaticMethod<void>("com/arashz4/localmessenger/NotificationClient", "showNotification",
                                                  "(Ljava/lang/String;)V", AndroidNotification.object<jstring>());
        #endif
    }

    QTextCursor c =  ui->messageTextEdit->textCursor();
    c.movePosition(QTextCursor::End);
    ui->messageTextEdit->setTextCursor(c);
}
