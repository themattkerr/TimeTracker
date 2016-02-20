#include "inserttimedialog.h"
#include "ui_inserttimedialog.h"
#include "mainwindow.h"


InsertTimeDialog::InsertTimeDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InsertTimeDialog)
{
    ui->setupUi(this);
    m_nTimeBeforeAs = TRACK;
    m_nTimeAfterAs = TRACK;
}

InsertTimeDialog::~InsertTimeDialog()
{
    delete ui;
}

void InsertTimeDialog::on_insertButton_clicked()
{
    ((MainWindow*)parentWidget())->insertTime(ui->timeInsertEdit->time()   , m_nTimeBeforeAs, m_nTimeAfterAs);
    this->close();
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
