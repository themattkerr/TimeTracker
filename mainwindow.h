#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#define TIME_TRACKERR_VERSION "1.1.1 Beta Build 3"

#include <QMainWindow>

#include "C://Users/thema/OneDrive/Documents/Matts_Software/General_Library/mattcalculations.h"
#include "loadfiledialog.h"
#include "changelogtitledialog.h"
#include "filterutilitydialog.h"
#include "inserttimedialog.h"
#include "aboutdialog.h"

#include <QDateTime>
#include <QTime>
#include <QTimer>

#include <QString>
#include <qdebug.h>
#include <QTextStream>
#include <qtextcursor.h>

#include <QDir>
#include <QFile>

#define SECTION_BREAK "----------------------------------- "
#define PROGRAM_EXIT_BREAK "===================== "
#define IGNORE_MARKER "> Ignored <"
#define INSERT_MARKER "+++ Time inserted +++\n"
#define ADD_TO_BEGINING "Add to begining"
#define ADD_TO_END "Add to end"
#define ONLY_INSERTED_TIME "No other time found"
#define MISSING_TIME_MARKER "Missing time:  "
#define LOGGED_AS_TRACKED_MARKER "Time has been logged as Tracked."
#define LOGGED_AS_IGNORED_MARKER "Time as been logged as > Ignored <."

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
    void insertTime(QTime tInsertTime, int nLogBeforeAs, int nLogAfterAs);

    QString getLogTitle();
    QString getIgnoredText();
    QString getTrackedText();
    int getLoadFileInfo();
    int getTrackedTime();
    int getIgnoredTime();
    bool getExitNow();

    void setPrivousLogType(int nPreviousLogType);

private slots:

    void on_actionLoad_saved_log_triggered();//May not actually be used?
    void displayEllapsed();

    // File Menu ---
    void on_actionChange_log_title_triggered();
    void on_actionFilter_Utility_triggered();

    //Edit Menu ---
    void on_actionUndo_last_log_entry_triggered();//This is actually Swap Tracked and Ignored
    void on_actionUndo_last_time_logging_triggered();
    void on_actionInsert_time_break_triggered();

    //Options Menu ---
    void on_actionShow_current_task_counter_triggered(bool checked);
    void on_actionShow_Time_Ignored_triggered(bool checked);
    void on_actionShow_total_time_triggered(bool checked);


    void on_fontComboBox_activated();
    void on_fontSize_spinBox_valueChanged(int arg1);

    void on_pushButton_clicked();//Track Time
    void on_ignoreButton_clicked();



    void on_actionAbout_triggered();

private:
    Ui::MainWindow *ui;

    QTime t;
    //QTextStream stmLog;

    bool m_bExitNow;
    bool m_bFileNameSet;
    bool m_bSaved;
    int m_nTotalTrackedTime;
    int m_nTotalIgnoredTime;
    int m_nTotalTime;
    int m_nLastRecordedTime;
    int m_nDeletedTime;

    int m_nLoadFileInfo;
    QString m_strFileName;
    QString m_strLogTitle;
    QString m_strFilteredIgnored;
    QString m_strFilteredTracked;
    int m_nPreviousLogType;
    int m_nElapsed;

    void hideAllInfo();
    void initializeMemberVariables();
    void initializeGUI();
    void calculateTotalTime();
    void setupLog();
    void saveLog(QString strFileName);
    void readInTrackedTime(QString &strInFile);
    void readInIgnoredTime(QString &strInFile);
    QString readInLastSavedTime(QString &strInFile);
    void removeLastMarker(QString strLastMarkerToRemove);
    int stringWithTimeEnteredToMilliseconds(QString strStringWithSavedTime, QString &strSavedTime);
    void logMissingTime();
    void removeLastTimeEntry();
    void refreshTimeTotals();
    void refreshTextEdit();

    QString scanForTitle(QString strInput);
    QString getFileName ();
    QString makeNewFileName(QString strFileName);

    QString findInsertSection(QString &strCurrentText, QTime &tInsertTime , int &nBeforeTime, int &nAfterTime, int &nStartIndex, int &nEndIndex);
    int findAmountTimeSavedInSection(QString &strSectionText, int &nStoredAs, int &nIndexOfTimeStoredInSection, QString &strSavedTime);
    void     setAndRemoveTimesForInsertTime(QTime &tInsertTime, QString &strCurrentText,
                                            int &nLogBeforeAs,   int &nLogAfterAs,
                                            int &nStartIndex,    int &nEndIndex, int &nBeforeTime, int &nAfterTime,
                                            int &nIndexOfTimeStoredInSection,
                                            QString &strSavedTime, int &nStoredAs, int &nAmountTimeSavedInSection, QString &strSectionText);

};


#endif // MAINWINDOW_H
