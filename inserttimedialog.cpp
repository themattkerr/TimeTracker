#include "inserttimedialog.h"
#include "ui_inserttimedialog.h"
#include "mainwindow.h"
#include <QMessageBox>


InsertTimeDialog::InsertTimeDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InsertTimeDialog)
{
    ui->setupUi(this);
    m_nTimeBeforeAs = TRACK;
    m_nTimeAfterAs = TRACK;
    ui->timeInsertEdit->setTime(QTime::currentTime());
}

InsertTimeDialog::~InsertTimeDialog()
{
    delete ui;
}

void InsertTimeDialog::on_insertButton_clicked()
{
    if(ui->timeInsertEdit->time() < QTime::currentTime())
    {
    ((MainWindow*)parentWidget())->insertTime(ui->timeInsertEdit->time()   , m_nTimeBeforeAs, m_nTimeAfterAs);
    this->close();
    }
    else
    {
        QMessageBox MB;
        MB.setText("Inserted time must be earlier than current time.");
        MB.setWindowTitle("Insert error");
        MB.exec();
    }

}

void InsertTimeDialog::on_beforeAsTrackedRadio_clicked()
{
    m_nTimeBeforeAs = TRACK;
}

void InsertTimeDialog::on_beforeAsIgnoredRadio_clicked()
{
    m_nTimeBeforeAs = IGNORE;
}

void InsertTimeDialog::on_afterAsTrackedRadio_clicked()
{
    m_nTimeBeforeAs = TRACK;
}

void InsertTimeDialog::on_afterAsIgnoredRadio_clicked()
{
    m_nTimeAfterAs = IGNORE;
}
