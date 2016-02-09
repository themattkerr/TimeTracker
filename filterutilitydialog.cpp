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
    ui->textBrowser->setPlainText(((MainWindow*)parentWidget())->getTrackedText());
}

void FilterUtilityDialog::on_ignoredButton_clicked()
{
    ui->textBrowser->setPlainText(((MainWindow*)parentWidget())->getIgnoredText());
}
