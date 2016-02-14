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

private:
    Ui::InsertTimeDialog *ui;
};

#endif // INSERTTIMEDIALOG_H
