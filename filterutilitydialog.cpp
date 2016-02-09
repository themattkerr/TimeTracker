#include "filterutilitydialog.h"
#include "ui_filterutilitydialog.h"
#include "mainwindow.h"
#include "calculations.h"

#include <qdebug.h>

FilterUtilityDialog::FilterUtilityDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FilterUtilityDialog)
{
    ui->setupUi(this);
}

FilterUtilityDialog::~FilterUtilityDialog()
{
    delete ui;
}

void FilterUtilityDialog::on_trackedButton_clicked()
{
    QString TrackedText = ((MainWindow*)parentWidget())->getTrackedText();
    qDebug() << TrackedText;
    //ui->textBrowser->setText( TrackedText );
}

void FilterUtilityDialog::on_ignoredButton_clicked()
{
    ui->textBrowser->setText( ((MainWindow*)parentWidget())->getIgnoredText());
}
