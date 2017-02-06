#ifndef OPTIONS_H
#define OPTIONS_H

#include "slsettings.h"

#include <QDialog>
#include <QPushButton>
#include <QTranslator>
#include <QCloseEvent>

namespace Ui {
class Options;
}

class Options : public QDialog
{
    Q_OBJECT

public:
    explicit Options(QWidget *parent = 0);
    ~Options();

private:
    Ui::Options *ui;

    QPushButton OK, Cancel, Apply, RestoreDefaults;

private slots:
    void Start();

    void on_listWidgetOptions_currentRowChanged(int currentRow);

    void Load();

    void Save();

    void reTranslate();

    void OKButton();

    void CancelButton();

    void ApplyButton();

    void RestoreDefaultsButton();

    void closeEvent (QCloseEvent *);

};

#endif // OPTIONS_H
