#include "loadfiledialog.h"
#include "ui_loadfiledialog.h"
#include <qdialog.h>
#include <qfiledialog.h>
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
    showImportOptions();

    //setLoadFileInfo(STARTNEW);

}

void LoadFileDialog::showImportOptions()
{
    ui->logTimeCheckBox->show();
    ui->logTimeCheckBox->setEnabled(true);
    ui->trackMissingTimeRadio->show();
    ui->logTimeAsIgnoredRadio->show();
    ui->continueButton->show();
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
        LoadFileInfo(TRACK);
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



void LoadFileDialog::on_browseForFileName_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Select a file"), "Files", tr("Time Tracker log files (*.txt)") );
    ((MainWindow*)parentWidget())->setFileName(fileName);
    showImportOptions();
    ((MainWindow*)parentWidget())->SetLogTitle("Loaded Saved Log");
}
