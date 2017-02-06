#include "options.h"
#include "ui_options.h"

Options::Options(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Options)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    ui->portLineEdit->setValidator(new QRegExpValidator(QRegExp("[0-9]{5}"), this));

    Start();
}

Options::~Options()
{
    delete ui;
}

void Options::Start()
{
    int x, y, w, h;
    std::tie(x, y, w, h) = SLSettings::LoadOptionsWindow();

    if(x != 0 || y != 0 || w != 0 || h != 0)
    {
        this->setGeometry(x, y, w, h);
    }

    OK.connect(&OK, SIGNAL(clicked()), this, SLOT(OKButton()));
    Cancel.connect(&Cancel, SIGNAL(clicked()), this, SLOT(CancelButton()));
    Apply.connect(&Apply, SIGNAL(clicked()), this, SLOT(ApplyButton()));
    RestoreDefaults.connect(&RestoreDefaults, SIGNAL(clicked()), this, SLOT(RestoreDefaultsButton()));
    OK.setText(tr("OK"));
    Cancel.setText(tr("Cancel"));
    Apply.setText(tr("Apply"));
    RestoreDefaults.setText(tr("Restore Defaults"));
    OK.setDefault(true);

    #if !defined(Q_OS_ANDROID)
    ui->OptionsButtonBox->addButton(&OK, QDialogButtonBox::AcceptRole);
    ui->OptionsButtonBox->addButton(&Cancel, QDialogButtonBox::RejectRole);
    ui->OptionsButtonBox->addButton(&Apply, QDialogButtonBox::ApplyRole);
    ui->OptionsButtonBox->addButton(&RestoreDefaults, QDialogButtonBox::ResetRole);
    #else
    ui->OptionsButtonBox->addButton(&OK, QDialogButtonBox::AcceptRole);
    ui->OptionsButtonBox->addButton(&Cancel, QDialogButtonBox::RejectRole);
    #endif

    ui->listWidgetOptions->setCurrentRow(0);
    ui->languageComboBox->insertSeparator(1);

    Load();

    #if defined(Q_OS_ANDROID)
    showMaximized();
    #endif
}

void Options::on_listWidgetOptions_currentRowChanged(int currentRow)
{
    if(currentRow == 0)
    {
        ui->OptionsGroupBox->setTitle(tr("General"));

        ui->messengerWidget->hide();
        ui->languageWidget->hide();

        ui->generalWidget->show();
    }
    else if (currentRow == 1)
    {
        ui->OptionsGroupBox->setTitle(tr("Messenger"));

        ui->generalWidget->hide();
        ui->languageWidget->hide();

        ui->messengerWidget->show();
    }
    else if (currentRow == 2)
    {
        ui->OptionsGroupBox->setTitle(tr("Language"));

        ui->generalWidget->hide();
        ui->messengerWidget->hide();

        ui->languageWidget->show();
    }
}

void Options::Load()
{
    //General
    {
        ui->minimizeToTrayCheckBox->setChecked(SLSettings::MinimizeToTray());
        ui->launchStartupCheckBox->setChecked(SLSettings::RunStartUp());
    }

    //Messenger
    {
        ui->portLineEdit->setText(QString::number(SLSettings::Port()));
    }

    //Language
    {
        if(SLSettings::AutomaticLanguage())
        {
            ui->languageComboBox->setCurrentIndex(0);
        }
        else
        {
            if(SLSettings::Language() == QLocale::English)
            {
                ui->languageComboBox->setCurrentIndex(2);
            }
            else if(SLSettings::Language() == QLocale::Persian)
            {
                ui->languageComboBox->setCurrentIndex(3);
            }
        }
    }
}

void Options::Save()
{
    //General
    {
        SLSettings::setRunStartUp(ui->launchStartupCheckBox->isChecked());

        SLSettings::setMinimizeToTray(ui->minimizeToTrayCheckBox->isChecked());
    }

    //Messenger
    {
        SLSettings::setPort(ui->portLineEdit->text().toInt());
    }

    //Language
    {
        if(ui->languageComboBox->currentIndex() == 0)
        {
            SLSettings::setLanguage(0);
        }
        else if(ui->languageComboBox->currentIndex() == 2)
        {
            SLSettings::setLanguage(QLocale::English);
        }
        else if(ui->languageComboBox->currentIndex() == 3)
        {
            SLSettings::setLanguage(QLocale::Persian);
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

    SLSettings::SaveSettings();
}

void Options::reTranslate()
{
    int cl = ui->languageComboBox->currentIndex();

    ui->retranslateUi(this);
    OK.setText(tr("OK"));
    Cancel.setText(tr("Cancel"));
    Apply.setText(tr("Apply"));
    RestoreDefaults.setText(tr("Restore Defaults"));

    if(ui->listWidgetOptions->currentRow() == 0)
    {
        ui->OptionsGroupBox->setTitle(tr("General"));
    }
    else if (ui->listWidgetOptions->currentRow() == 1)
    {
        ui->OptionsGroupBox->setTitle(tr("Messenger"));
    }
    else if (ui->listWidgetOptions->currentRow() == 2)
    {
        ui->OptionsGroupBox->setTitle(tr("Language"));
    }

    ui->languageComboBox->insertSeparator(1);
    ui->languageComboBox->setCurrentIndex(cl);
}

void Options::OKButton()
{
    Save();
    close();
}

void Options::CancelButton()
{
    close();
}

void Options::ApplyButton()
{
    Save();

    reTranslate();
}

void Options::RestoreDefaultsButton()
{
    SLSettings::setRunStartUp(true);
    SLSettings::setMinimizeToTray(true);
    SLSettings::setPort(4256);
    SLSettings::setAutomaticLanguage(true);
    SLSettings::setLanguage(0);

    Load();
}

void Options::closeEvent (QCloseEvent *)
{
    SLSettings::SaveOptionsWindow(this->geometry().x(), this->geometry().y(), this->geometry().width(), this->geometry().height());

    this->hide();
}
