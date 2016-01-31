#include "loadfiledialog.h"
#include "ui_loadfiledialog.h"
#include <qdialog.h>
#include "mainwindow.h"

LoadFileDialog::LoadFileDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoadFileDialog)
{
    ui->setupUi(this);
    ui->continueButton->hide();
    ui->logTimeCheckBox->hide();
    ui->trackMissingTimeRadio->hide();
    ui->logTimeAsIgnoredRadio->hide();

    adjustSize();
}

LoadFileDialog::~LoadFileDialog()
{
    delete ui;
}

void LoadFileDialog::on_addToExistingButton_clicked()
{
    //ui->logTimeCheckBox->show();
    //ui->logTimeCheckBox->setEnabled(true);
    //ui->trackMissingTimeRadio->show();
    //ui->logTimeAsIgnoredRadio->show();
    ui->continueButton->show();

    //setLoadFileInfo(STARTNEW);

}

void LoadFileDialog::on_logTimeCheckBox_clicked()
{

}

void LoadFileDialog::on_trackMissingTimeRadio_clicked()
{
    LoadFileInfo(TRACK);
    ui->continueButton->show();
}

void LoadFileDialog::on_logTimeAsIgnoredRadio_clicked()
{
    LoadFileInfo(IGNORE);
    ui->continueButton->show();
}

void LoadFileDialog::on_continueButton_clicked()
{

    this->close();
}

bool LoadFileDialog::LoadFileInfo(int nLoadFileInfo)
{
    ((MainWindow*)parentWidget())->SetLoadFileInfo(nLoadFileInfo);

}



void LoadFileDialog::on_logTimeCheckBox_toggled(bool checked)
{
    if (checked)
    {
        ui->trackMissingTimeRadio->setEnabled(true);
        ui->logTimeAsIgnoredRadio->setEnabled(true);

    }
    else
    {
        ui->trackMissingTimeRadio->setEnabled(false);
        ui->logTimeAsIgnoredRadio->setEnabled(false);
        LoadFileInfo(APPEND);
    }

}

void LoadFileDialog::on_startNewButton_clicked()
{
    LoadFileInfo(STARTNEW);
    this->close();
}
