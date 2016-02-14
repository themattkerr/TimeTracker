#include "inserttimedialog.h"
#include "ui_inserttimedialog.h"
#include "mainwindow.h"


InsertTimeDialog::InsertTimeDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InsertTimeDialog)
{
    ui->setupUi(this);
}

InsertTimeDialog::~InsertTimeDialog()
{
    delete ui;
}
