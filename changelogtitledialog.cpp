#include "changelogtitledialog.h"
#include "ui_changelogtitledialog.h"
#include "mainwindow.h"

ChangeLogTitleDialog::ChangeLogTitleDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChangeLogTitleDialog)
{
    ui->setupUi(this);
}

ChangeLogTitleDialog::~ChangeLogTitleDialog()
{
    delete ui;
}

void ChangeLogTitleDialog::on_buttonBox_accepted()
{
    ((MainWindow*)parent())->SetLogTitle(ui->lineEdit->text().simplified());
    this->close();
}

void ChangeLogTitleDialog::on_buttonBox_rejected()
{
    this->close();
}
