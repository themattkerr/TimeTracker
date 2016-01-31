#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QTime>
#include <QMainWindow>
#include <QTextStream>
//#include "mattcalculations.h"

enum LoadFileInfo{
    STARTNEW = 0,
    APPEND = 1,
    TRACK = 2,
    IGNORE = 3,
};

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void SetLoadFileInfo(int nLoadFileInfo );

private slots:
    void on_pushButton_clicked();
    void displayEllapsed();


    void on_ignoreButton_clicked();

private:
    Ui::MainWindow *ui;

    QTime t;
    //QTextStream stmLog;

    bool m_bSaved;
    int m_nTotalTime;
    int m_nTotalIgnoredTime;
    int m_nLastRecordedTime;

    int m_nLoadFileInfo;
    QString m_strFileName;


    void setupLog();
    void saveLog(QString strFileName);
    QString getFileName ();
    QString makeNewFileName(QString strFileName);
};

#endif // MAINWINDOW_H
