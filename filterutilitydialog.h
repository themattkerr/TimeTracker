#ifndef FILTERUTILITYDIALOG_H
#define FILTERUTILITYDIALOG_H

#include <QDialog>

namespace Ui {
class FilterUtilityDialog;
}

class FilterUtilityDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FilterUtilityDialog(QWidget *parent = 0);
    ~FilterUtilityDialog();

private slots:
    void on_trackedButton_clicked();

    void on_ignoredButton_clicked();

private:
    Ui::FilterUtilityDialog *ui;
};

#endif // FILTERUTILITYDIALOG_H
