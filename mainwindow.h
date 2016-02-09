#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QTime>
#include <QMainWindow>
#include <QTextStream>

#define SECTION_BREAK "----------------------------------- "
#define PROGRAM_EXIT_BREAK "===================== "

enum LoadFileInfo{
    STARTNEW = 0,
    APPEND = 1,
    TRACK = 2,
    IGNORE = 3,
    EXIT_NO_SAVE = 4
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
    void exitWithoutSave();
    void filterText();
    QString getIgnoredText();
    QString getTrackedText();
    int getTrackedTime();
    int getIgnoredTime();

private slots:
    void on_pushButton_clicked();

    void displayEllapsed();

    void on_ignoreButton_clicked();

    void on_actionShow_current_task_counter_triggered(bool checked);

    void on_actionUndo_last_log_entry_triggered();

    void on_actionShow_Time_Ignored_triggered(bool checked);

    void on_actionChange_log_title_triggered();

    void on_actionLoad_saved_log_triggered();

    void on_actionShow_total_time_triggered(bool checked);

    void on_actionUndo_last_time_logging_triggered();

    void on_actionFilter_Utility_triggered();

private:
    Ui::MainWindow *ui;

    QTime t;
    //QTextStream stmLog;

    bool m_bFileNameSet;
    bool m_bSaved;
    int m_nTotalTrackedTime;
    int m_nTotalIgnoredTime;
    int m_nTotalTime;
    int m_nLastRecordedTime;

    int m_nLoadFileInfo;
    QString m_strFileName;
    QString m_strLogTitle;
    QString m_strFilteredIgnored;
    QString m_strFilteredTracked;
    int m_nPreviousLogType;
    int m_nElapsed;

    void initializeMemberVariables();
    void initializeGUI();
    void calculateTotalTime();
    void setupLog();
    void saveLog(QString strFileName);
    void readInTrackedTime(QString &strInFile);
    void readInIgnoredTime(QString &strInFile);
    void readInLastSavedTime(QString &strInFile);
    void logMissingTime();

    QString scanForTitle(QString strInput);
    QString getFileName ();
    QString makeNewFileName(QString strFileName);


};

#endif // MAINWINDOW_H
