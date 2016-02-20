#ifndef INSERTTIMEDIALOG_H
#define INSERTTIMEDIALOG_H

#include <QDialog>

namespace Ui {
class InsertTimeDialog;
}

class InsertTimeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit InsertTimeDialog(QWidget *parent = 0);
    ~InsertTimeDialog();

private slots:
    void on_insertButton_clicked();

    void on_beforeAsTrackedRadio_clicked();

    void on_beforeAsIgnoredRadio_clicked();

    void on_afterAsTrackedRadio_clicked();

    void on_afterAsIgnoredRadio_clicked();

private:
    Ui::InsertTimeDialog *ui;
    int m_nTimeBeforeAs;
    int m_nTimeAfterAs;

};

#endif // INSERTTIMEDIALOG_H
