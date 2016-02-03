#ifndef CHANGELOGTITLEDIALOG_H
#define CHANGELOGTITLEDIALOG_H

#include <QDialog>

namespace Ui {
class ChangeLogTitleDialog;
}

class ChangeLogTitleDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ChangeLogTitleDialog(QWidget *parent = 0);
    ~ChangeLogTitleDialog();

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::ChangeLogTitleDialog *ui;
};

#endif // CHANGELOGTITLEDIALOG_H
