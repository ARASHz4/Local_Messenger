#ifndef ABOUT_H
#define ABOUT_H

#include "slsettings.h"

#include <QDialog>
#include <QMouseEvent>
#include <QPixmap>

namespace Ui {
class About;
}

class About : public QDialog
{
    Q_OBJECT

public:
    explicit About(QWidget *parent = 0);
    ~About();

private slots:
    void Start();

    void on_CloseButton_clicked();

    void on_AboutQtButton_clicked();

    void mouseDoubleClickEvent(QMouseEvent *);

    void closeEvent (QCloseEvent *CloseEvant);

private:
    Ui::About *ui;

    bool ds;
};

#endif // ABOUT_H
