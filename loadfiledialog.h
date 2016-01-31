#ifndef LOADFILEDIALOG_H
#define LOADFILEDIALOG_H

#include <QDialog>

namespace Ui {
class LoadFileDialog;
}

class LoadFileDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoadFileDialog(QWidget *parent = 0);
    ~LoadFileDialog();

private slots:
    void on_addToExistingButton_clicked();

    void on_trackMissingTimeRadio_clicked();

    void on_logTimeCheckBox_clicked();

    void on_logTimeAsIgnoredRadio_clicked();

    void on_continueButton_clicked();

    void on_logTimeCheckBox_toggled(bool checked);

    void on_startNewButton_clicked();

private:
    Ui::LoadFileDialog *ui;

    bool LoadFileInfo(int nLoadFileInfo);
};

#endif // LOADFILEDIALOG_H
