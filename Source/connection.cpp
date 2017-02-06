#include "connection.h"
#include "ui_connection.h"

Connection::Connection(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Connection)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    ui->ipLineEdit->setValidator(new QRegExpValidator(QRegExp("[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}"), this));

    #if defined(Q_OS_ANDROID)
    showMaximized();
    #endif
}

Connection::~Connection()
{
    delete ui;
}

bool Connection::Connected() const
{
    return connected;
}

QHostAddress Connection::IP() const
{
    return ip;
}

void Connection::on_connectPushButton_clicked()
{
    if(QRegExp("[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}").exactMatch(ui->ipLineEdit->text()))
    {
        connected = true;
        ip.setAddress(ui->ipLineEdit->text());

        close();
    }
}

void Connection::on_cancelPushButton_clicked()
{
    connected = false;

    close();
}

void Connection::on_ipLineEdit_textChanged()
{
    if(QRegExp("[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}").exactMatch(ui->ipLineEdit->text()))
    {
        ui->connectPushButton->setEnabled(true);
    }
    else
    {
        ui->connectPushButton->setEnabled(false);
    }
}
