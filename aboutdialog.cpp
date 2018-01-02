#include "aboutdialog.h"
#include "ui_aboutdialog.h"

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);
    ui->ProgramVerlabel->setText(TIME_TRACKERR_VERSION);
    ui->CalculationsVerlabel->setText(VERSION_OF_MATTCALCULATIONS);

    Qt::WindowFlags flags = this->windowFlags();
    this->setWindowFlags(flags|Qt::WindowStaysOnTopHint);
}

AboutDialog::~AboutDialog()
{
    delete ui;
}
