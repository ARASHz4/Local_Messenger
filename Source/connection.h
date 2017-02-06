#ifndef CONNECTION_H
#define CONNECTION_H

#include <QDialog>
#include <QHostAddress>

namespace Ui {
class Connection;
}

class Connection : public QDialog
{
    Q_OBJECT

public:
    explicit Connection(QWidget *parent = 0);
    ~Connection();

    bool Connected() const;

    QHostAddress IP() const;

private:
    Ui::Connection *ui;

    bool connected=false;
    QHostAddress ip;

private slots:
    void on_connectPushButton_clicked();

    void on_cancelPushButton_clicked();

    void on_ipLineEdit_textChanged();

};

#endif // CONNECTION_H
