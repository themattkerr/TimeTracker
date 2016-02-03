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
    void SetLogTitle(QString LogTitle);
    void setFileName(QString strFileName);

private slots:
    void on_pushButton_clicked();

    void displayEllapsed();

    void on_ignoreButton_clicked();

    void on_actionShow_current_task_counter_triggered(bool checked);

    void on_actionUndo_last_log_entry_triggered();

    void on_actionShow_Time_Ignored_triggered(bool checked);

    void on_actionChange_log_title_triggered();

    void on_actionLoad_saved_log_triggered();

private:
    Ui::MainWindow *ui;

    QTime t;
    //QTextStream stmLog;

    bool m_bFileNameSet;
    bool m_bSaved;
    int m_nTotalTime;
    int m_nTotalIgnoredTime;
    int m_nLastRecordedTime;

    int m_nLoadFileInfo;
    QString m_strFileName;
    QString m_strLogTitle;
    int m_nPreviousLogType;
    int m_nElapsed;

    void setupLog();
    void saveLog(QString strFileName);
    QString scanForTitle(QString strInput);

    QString getFileName ();
    QString makeNewFileName(QString strFileName);
};

#endif // MAINWINDOW_H
