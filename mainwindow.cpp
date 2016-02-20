#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "calculations.h"
#include "loadfiledialog.h"
#include "changelogtitledialog.h"
#include "filterutilitydialog.h"
#include "inserttimedialog.h"

#include <QDateTime>
#include <QTime>
#include <QTimer>

#include <QString>
#include <qdebug.h>
#include <QTextStream>

#include <QDir>
#include <QFile>

#include <QMessageBox>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    initializeMemberVariables();
    QFile f(getFileName());
    if (f.exists())
    {
        LoadFileDialog *LFI = new LoadFileDialog(this);
        LFI->exec();
    }

    m_strFileName = getFileName();

    ui->setupUi(this);

    Qt::WindowFlags flags = this->windowFlags();
    this->setWindowFlags(flags|Qt::WindowStaysOnTopHint);
    this->setWindowTitle("Time TracKerr");

    setupLog();

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(displayEllapsed()));
    timer->start(1000);

    calculateTotalTime();
    initializeGUI();
//    if(m_bExitNow)
//    {
//        QMessageBox m;
//        m.setText("why this no work");
//        m.exec();

//    }
    t.start();
}

MainWindow::~MainWindow()
{
    if(m_nLoadFileInfo == STARTNEW)
        m_strFileName = getFileName();
    if(m_nLoadFileInfo != EXIT_NO_SAVE)
        saveLog(m_strFileName);
    delete ui;
}
bool MainWindow::getExitNow()
{
    return m_bExitNow;
}

void MainWindow::initializeMemberVariables()
{
    m_nLoadFileInfo = APPEND;//This must be append in order for file name logic to work

    m_bExitNow = false;
    m_bSaved = false;
    m_bFileNameSet= false;
    m_nTotalTrackedTime = 0;
    m_nTotalIgnoredTime = 0;
    m_nTotalTime = 0;
    m_nDeletedTime = 0;
    m_strLogTitle = "Time TracKerr";
}

void MainWindow::initializeGUI()
{
    if(m_nTotalTrackedTime == 0)
        ui->TotalTrackedTime->setText("---");
    else
        ui->TotalTrackedTime->setText(millisecondsToHoursMinsSec(m_nTotalTrackedTime));

    if(m_nTotalIgnoredTime != 0)
        ui->timeIgnored->setText(millisecondsToHoursMinsSec(m_nTotalIgnoredTime));
    if(m_nTotalTime != 0)
        ui->totalTime->setText(millisecondsToHoursMinsSec(m_nTotalTime));

    ui->timeIgnoredLabel->hide();
    ui->timeIgnored->hide();
    ui->CurrentTaskLabel->hide();
    ui->CurrentTaskTime->hide();
    ui->totalTimeLabel->hide();
    ui->totalTime->hide();

    ui->textEdit->append (t.currentTime().toString("h:mm:ss A"));
}

void MainWindow::on_pushButton_clicked()
{
    m_nElapsed = t.elapsed() + m_nDeletedTime;
    m_nDeletedTime = 0;
    m_nTotalTrackedTime = m_nTotalTrackedTime + m_nElapsed;

    ui->textEdit->append(millisecondsToHoursMinsSec(m_nElapsed));
    ui->textEdit->append(SECTION_BREAK);
    ui->textEdit->append (t.currentTime().toString("h:mm:ss A"));
    ui->TotalTrackedTime->setText(millisecondsToHoursMinsSec(m_nTotalTrackedTime)  );
    calculateTotalTime();
    m_nPreviousLogType = TRACK;
    //QTextCursor::End;
    t.restart();
}

void MainWindow::displayEllapsed()
{

    ui->CurrentTaskTime->setText(millisecondsToHoursMinsSec(t.elapsed()+ m_nDeletedTime));

}

void MainWindow::on_ignoreButton_clicked()
{
    m_nElapsed = t.elapsed()+ m_nDeletedTime;
    m_nDeletedTime = 0;
    m_nTotalIgnoredTime = m_nTotalIgnoredTime + m_nElapsed;

    ui->textEdit->append(millisecondsToHoursMinsSec(m_nElapsed));
    ui->textEdit->append(IGNORE_MARKER);
    ui->textEdit->append(SECTION_BREAK);
    ui->textEdit->append (t.currentTime().toString("h:mm:ss A"));

    ui->timeIgnored->setText(millisecondsToHoursMinsSec(m_nTotalIgnoredTime)  );
    calculateTotalTime();
    m_nPreviousLogType = IGNORE;


    t.restart();
}

void MainWindow::calculateTotalTime()
{
    m_nTotalTime = m_nTotalTrackedTime + m_nTotalIgnoredTime;
    ui->totalTime->setText(millisecondsToHoursMinsSec(m_nTotalTime));
}

void MainWindow::saveLog(QString strFileName)
{
    QDir mDir;
    if (!mDir.exists("TimeTracKerrFiles"))
    {
        mDir.mkdir("TimeTracKerrFiles");
    }
    QFile Log(strFileName);

    if(Log.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text))
    {
        QTextStream stmLog(&Log);

        if (m_bSaved == false)
        {
            stmLog << "<> " << m_strLogTitle.simplified() << " <>"<<'\n';
            stmLog << ui->textEdit->toPlainText();
            stmLog << '\n';// << t.currentTime().toString("h:mm:ss A") << " <-- Program exit time." << '\n';
            stmLog << SECTION_BREAK;
            stmLog << '\n';
            stmLog << "Total Tracked Time:  " << millisecondsToHoursMinsSec(m_nTotalTrackedTime) << '\n';
            stmLog << "Total Time Ignored:  " << millisecondsToHoursMinsSec(m_nTotalIgnoredTime) << '\n';      // << '\n';
        }
        Log.close();
    }
    else
    {
        QMessageBox msgBox;
        msgBox.setText("Warning! File did not save!");
        msgBox.exec();
    }
}

void MainWindow::setFileName(QString strFileName)
{
    m_strFileName = strFileName;
    m_bFileNameSet = true;
}

QString MainWindow::getFileName()
{
    if(m_bFileNameSet == true)
        return m_strFileName;
    QString strFileName;
    strFileName = "TimeTracKerrFiles/";
    QString strLogTitleSimplified = m_strLogTitle;
    strFileName.append(strLogTitleSimplified.simplified().remove(" "));
    QDateTime current = QDateTime::currentDateTime();
    QString strDateStamp = current.toString("yyyy:MM:dd");
    strDateStamp.replace(':','.');

    strFileName.append(strDateStamp).append(".txt");

    if (m_nLoadFileInfo == STARTNEW)
        strFileName = makeNewFileName(strFileName);

    return strFileName;
}

QString MainWindow::makeNewFileName(QString strFileName)
{
    QFile f(strFileName);
    if(!f.exists())
        return strFileName;
    strFileName.remove(".txt");

    int x = 1;
    QString tempFileName;
    do{
       tempFileName = strFileName;
       tempFileName.append("..");
       if (x < 10)
           tempFileName.append("00");
       if(x >= 10 && x < 100)
           tempFileName.append("0");
       tempFileName.append(QString::number(x,10)).append(".txt");
        x++;

    }while (QFile::exists(tempFileName));

    strFileName = tempFileName;

    return strFileName;
}

void MainWindow::SetLoadFileInfo(int nLoadFileInfo )
{
   m_nLoadFileInfo = nLoadFileInfo;
}

void MainWindow::setupLog()
{
   if (m_nLoadFileInfo == STARTNEW)
       return;

   QFile file(m_strFileName);
   file.open(QIODevice::ReadOnly | QIODevice::Text );
   QTextStream in(&file);

   while (!file.atEnd())
   {
       QString strInFile = in.readAll();
       strInFile = scanForTitle(strInFile);

       if (strInFile.contains("Total Tracked Time:  "))
       {
           readInTrackedTime(strInFile);
       }
       if (strInFile.contains("Total Time Ignored:  "))
       {
           readInIgnoredTime(strInFile);
       }
       if (strInFile.contains("AM") || strInFile.contains("PM") )
       {
           readInLastSavedTime(strInFile);
       }
       ui->textEdit->append(strInFile);
       ui->textEdit->append("");
       ui->textEdit->append(PROGRAM_EXIT_BREAK);
       ui->textEdit->append("");

       if(m_nLoadFileInfo == TRACK || m_nLoadFileInfo == IGNORE)
        {
          logMissingTime();
        }
   }//end while loop

   file.close();

}

void MainWindow::readInTrackedTime(QString &strInFile)
{
    int x = strInFile.lastIndexOf("Total Tracked Time:  ");
    QString TotalTime;
    for(int iii = (x+21); iii<(x+33); iii++)
    {
       if (strInFile.at(iii) == '\n')
           break;
        TotalTime.append(strInFile.at(iii));
    }
    m_nTotalTrackedTime = stringToMilliseconds(TotalTime);
    TotalTime.prepend("\nTotal Tracked Time:  ");
    TotalTime.append("\n");
    qDebug() << TotalTime;
    strInFile.remove(TotalTime);
}

void MainWindow::readInIgnoredTime(QString &strInFile)
{
    int x = strInFile.lastIndexOf("Total Time Ignored:  ");
    QString TotalTime;
    for(int iii = (x+21); iii<(x+33); iii++)
    {
       if (strInFile.at(iii) == '\n')
           break;
        TotalTime.append(strInFile.at(iii));
    }
    m_nTotalIgnoredTime = stringToMilliseconds(TotalTime);
    TotalTime.prepend("Total Time Ignored:  ");
    TotalTime.append("\n");
    strInFile.remove(TotalTime);
}

QString MainWindow::readInLastSavedTime(QString &strInFile)
{
    QString strLastTime;
    int lastTimeIndex=0;
    int am = strInFile.lastIndexOf("AM");
    int pm = strInFile.lastIndexOf("PM");
    if (am == pm)
        return"";
    if (am > pm)
        lastTimeIndex = am;
    else
        lastTimeIndex = pm;
    for (int iii = (lastTimeIndex-9); iii < lastTimeIndex + 2 ; iii ++)
    {
        strLastTime.append(strInFile.at(iii));

    }

     strLastTime.remove('\n');
     m_nLastRecordedTime = stringToMilliseconds(strLastTime);
    return strLastTime;
}

void MainWindow::logMissingTime()
{
    int nCurrentTime = stringToMilliseconds(t.currentTime().toString("h:mm:ss A"));
    int nTimeDifference = nCurrentTime - m_nLastRecordedTime;
    if (nTimeDifference < 0)
    {
        QMessageBox msbx;
        msbx.setText("Error! Time difference is less than zero.");
        msbx.exec();
        m_nLoadFileInfo = APPEND;
        return;
    }
    else
    {
        QString strMissingTime = "Missing time:  ";
        strMissingTime.append(millisecondsToHoursMinsSec(nTimeDifference));
        ui->textEdit->append(strMissingTime);
    }
    if (m_nLoadFileInfo == TRACK)
    {
        m_nTotalTrackedTime += nTimeDifference;
        ui->textEdit->append("Time has been logged as Tracked.");
        ui->textEdit->append(SECTION_BREAK);
    }
    if (m_nLoadFileInfo == IGNORE)
    {
        m_nTotalIgnoredTime += nTimeDifference;
        ui->textEdit->append("Time as been logged as > Ignored <.");
        ui->textEdit->append(SECTION_BREAK);
    }
}

void MainWindow::on_actionShow_current_task_counter_triggered(bool checked)
{
    if (checked)
    {
        ui->CurrentTaskLabel->show();
        ui->CurrentTaskTime->show();
    }
    else
    {
        ui->CurrentTaskLabel->hide();
        ui->CurrentTaskTime->hide();
    }
}

void MainWindow::on_actionUndo_last_log_entry_triggered()
{
    if(m_nPreviousLogType == TRACK)
    {
        m_nTotalTrackedTime = m_nTotalTrackedTime - m_nElapsed;
        ui->TotalTrackedTime->setText(millisecondsToHoursMinsSec(m_nTotalTrackedTime));

        m_nTotalIgnoredTime = m_nTotalIgnoredTime + m_nElapsed;
        ui->timeIgnored->setText(millisecondsToHoursMinsSec(m_nTotalIgnoredTime));
        QString out;
        out = "==> "; out.append(millisecondsToHoursMinsSec(m_nElapsed)).append(" swapped");
        ui->textEdit->append(out);
        ui->textEdit->append("  from Tracked to Ignored");

        m_nPreviousLogType = IGNORE;

    }
    else
    {
        m_nTotalTrackedTime = m_nTotalTrackedTime + m_nElapsed;
        ui->TotalTrackedTime->setText(millisecondsToHoursMinsSec(m_nTotalTrackedTime));

        m_nTotalIgnoredTime = m_nTotalIgnoredTime - m_nElapsed;
        ui->timeIgnored->setText(millisecondsToHoursMinsSec(m_nTotalIgnoredTime));

        QString out;
        out = "<== "; out.append(millisecondsToHoursMinsSec(m_nElapsed)).append(" swapped");
        ui->textEdit->append(out);
        ui->textEdit->append("  from Ignored to Tracked");

        m_nPreviousLogType = TRACK;
    }


}



void MainWindow::on_actionShow_Time_Ignored_triggered(bool checked)
{
    if(checked)
    {
        ui->timeIgnored->show();
        ui->timeIgnoredLabel->show();
    }
    else
    {
        ui->timeIgnored->hide();
        ui->timeIgnoredLabel->hide();
    }
}

void MainWindow::on_actionChange_log_title_triggered()
{
    ChangeLogTitleDialog *ChangeTitle = new ChangeLogTitleDialog(this);
    ChangeTitle->exec();
}

void MainWindow::SetLogTitle(QString LogTitle)
{
    m_strLogTitle = LogTitle;
    this->setWindowTitle(m_strLogTitle);
}

QString MainWindow::getLogTitle()
{
    return m_strLogTitle;
}

void MainWindow::on_actionLoad_saved_log_triggered()
{
    LoadFileDialog *LoadFile = new LoadFileDialog(this);
    LoadFile->exec();
}

QString MainWindow::scanForTitle(QString strInput)
{
    int nTitleIndexStart =  (strInput.indexOf("<>") + 2);
    int nTitleIndexEnd =    (strInput.indexOf("<>", nTitleIndexStart));
    if (nTitleIndexStart < 0 || nTitleIndexEnd > 100)
        return "Loaded saved log";
    QString strWindowTitle;
    for (int III = nTitleIndexStart; III < nTitleIndexEnd; III++)
        strWindowTitle.append(strInput[III]);
    SetLogTitle(strWindowTitle);
    strWindowTitle.prepend("<>").append("<>").append('\n');
    (strInput.remove(strWindowTitle));
    return strInput;
}

void MainWindow::on_actionShow_total_time_triggered(bool checked)
{
    if(checked)
    {
        ui->totalTimeLabel->show();
        ui->totalTime->show();
    }
    else
    {
        ui->totalTimeLabel->hide();
        ui->totalTime->hide();
    }
}

void MainWindow::exitWithoutSave()
{
   m_bExitNow = true;
    //this->close();

}

void MainWindow::on_actionUndo_last_time_logging_triggered()
{
    removeLastTimeEntry();
}
QString MainWindow::getIgnoredText()
{
    filterText();
    return m_strFilteredIgnored;
}

QString MainWindow::getTrackedText()
{
    filterText();
    return m_strFilteredTracked;
}

int MainWindow::getTrackedTime()
{
    return m_nTotalTrackedTime;
}

int MainWindow::getIgnoredTime()
{
    return m_nTotalIgnoredTime;
}

void MainWindow::filterText()
{
    m_strFilteredIgnored = "";
    m_strFilteredTracked = "";
    QString strSection;

    QString strUnFiltered = ui->textEdit->toPlainText();
    int nCurrentStartIndex = 0;
    int nNextIndex = 0;

    if (strUnFiltered.contains(SECTION_BREAK))
    {

        nNextIndex = strUnFiltered.indexOf(SECTION_BREAK);

        bool bKeepGoing = true;
        while(bKeepGoing)
        {
            if (nNextIndex == strUnFiltered.length()) bKeepGoing = false;

            strSection = "";
            for (int iii = nCurrentStartIndex; iii < nNextIndex; iii++)
            {
                 strSection.append(strUnFiltered[iii]);
            }
            if (strSection.contains(IGNORE_MARKER))
                m_strFilteredIgnored.append(strSection);
            else
               m_strFilteredTracked.append(strSection);

            nCurrentStartIndex = strUnFiltered.indexOf(SECTION_BREAK, nNextIndex);
            nNextIndex = strUnFiltered.indexOf(SECTION_BREAK, nCurrentStartIndex+1);

            if(nNextIndex < 0)
            {
                nNextIndex = strUnFiltered.length();

            }

           //m_strFilteredIgnored.remove("\n\n\n");

          // m_strFilteredTracked.remove("\n\n\n");

        }

    }

    return;

}


void MainWindow::on_actionFilter_Utility_triggered()
{
    FilterUtilityDialog *Filter = new FilterUtilityDialog(this);
    Filter->show();
}

void MainWindow::removeLastTimeEntry()
{
    QString strCurrentText = ui->textEdit->toPlainText();
    QString strSectionBreak = SECTION_BREAK;
    QString strIgnoreMarker = IGNORE_MARKER;
    QString strSection ="";
    QString strTempTimeReversed = "";
    int nSectionBreakLength = strSectionBreak.length();
    int nCurrentTimeBreak = strCurrentText.lastIndexOf(SECTION_BREAK);
    if(nCurrentTimeBreak < 0)
        nCurrentTimeBreak = 0;
    int nPreviousTimeBreak = strCurrentText.lastIndexOf(SECTION_BREAK, nCurrentTimeBreak-1);
    if(nPreviousTimeBreak < 0 )
        nPreviousTimeBreak = 0;
    for (int eee = nPreviousTimeBreak; eee < nCurrentTimeBreak; eee++)
    {
        strSection.append(strCurrentText[eee]);
    }
    QString strLastTimeEntered = readInLastSavedTime(strCurrentText);
    int nLastTime = strCurrentText.lastIndexOf(QRegExp ("[1234567890][1234567890]s"));

    for (int iii = nLastTime+2; iii >=0; iii-- )
    {
        if(strCurrentText[iii] != '\n')
            strTempTimeReversed.append(strCurrentText[iii]);
        else
            break;
    }

    QString strTemp = strTempTimeReversed;
    for (int jjj = 0, lll = strTempTimeReversed.length()-1;jjj < (strTempTimeReversed.length()); jjj++, lll-- )
    {
        strTemp[lll] = strTempTimeReversed[jjj];
    }

    int nTimeToSubtract = stringToMilliseconds(strTemp);
    //int nTimeToSubtract = stringWithTimeEnteredToMilliseconds(strCurrentText);
    strCurrentText.remove(strLastTimeEntered);
    strCurrentText.remove(nCurrentTimeBreak,nSectionBreakLength+1);

    if (strSection.contains(IGNORE_MARKER))
    {
        m_nTotalIgnoredTime -= nTimeToSubtract;

        strCurrentText.remove((strCurrentText.lastIndexOf(IGNORE_MARKER)),(strIgnoreMarker.length()+1));


    }
    else
        m_nTotalTrackedTime -= nTimeToSubtract;

    strCurrentText.remove((strCurrentText.lastIndexOf(strTemp)),(strTemp.length()+1));

    m_nTotalTime -= nTimeToSubtract;
    m_nDeletedTime += nTimeToSubtract;


    ui->textEdit->setText(strCurrentText);
    refreshTimeTotals();

}

void MainWindow::refreshTimeTotals()
{
    if (m_nTotalIgnoredTime < 0)
        m_nTotalIgnoredTime = 0;
    if (m_nTotalTrackedTime < 0)
        m_nTotalTrackedTime = 0;
    m_nTotalTime = m_nTotalIgnoredTime + m_nTotalTrackedTime;

    ui->TotalTrackedTime->setText(millisecondsToHoursMinsSec(m_nTotalTrackedTime)  );
    ui->timeIgnored->setText(millisecondsToHoursMinsSec(m_nTotalIgnoredTime)  );
    ui->totalTime->setText(millisecondsToHoursMinsSec(m_nTotalTime));

}

int MainWindow::stringWithTimeEnteredToMilliseconds(QString strStringWithSavedTime, QString &strSavedTime)
{
    QString strTempTimeReversed = "";
    int nLastTime = strStringWithSavedTime.lastIndexOf(QRegExp ("[1234567890][1234567890]s"));

    for (int iii = nLastTime+2; iii >=0; iii-- )
    {
        if(strStringWithSavedTime[iii] != '\n')
            strTempTimeReversed.append(strStringWithSavedTime[iii]);
        else
            break;
    }

    QString strTemp = strTempTimeReversed;
    for (int jjj = 0, lll = strTempTimeReversed.length()-1;jjj < (strTempTimeReversed.length()); jjj++, lll-- )
    {
        strTemp[lll] = strTempTimeReversed[jjj];
    }

    strSavedTime = strTemp;
    return stringToMilliseconds(strTemp);
}

void MainWindow::insertTime(QTime tInsertTime, int nLogBeforeAs, int LogAfterAs)
{
    int nStoredAs = APPEND;
    int nIndexOfTimeStoredInSection = 0;
    int nStartIndex = 0;
    int nEndIndex = 0;
    QString strSavedTime;
    
    QString strCurrentText = ui->textEdit->toPlainText();
    QString strSectionText = findInsertSection(strCurrentText, tInsertTime , nStartIndex, nEndIndex);

//    QMessageBox h;
//    h.setText(strSectionText);
//    h.exec();

    int nAmountTimeSavedInSection = findAmountTimeSavedInSection(strSectionText, nStoredAs, nIndexOfTimeStoredInSection, strSavedTime);
   
//    qDebug() << "amount of time saved" << millisecondsToHoursMinsSec(nAmountTimeSavedInSection);
//    qDebug() << "Stored as " << nStoredAs;
//    qDebug() << "index of time stored in section = "<< nIndexOfTimeStoredInSection;
//    qDebug() << "time saved string "<< strSavedTime;


    // remove that time from its target (tracked or ignored)
    // remove old time and text for tracked or ignored from string
            //use remove(#,#) to remove text from a specific location

    // Place new time break before end of section break
    // enter new logged time before inserted time and add that time to the right place
    // enter new text that includes info about inserting new time.
    // place new logged time right before section break.

    // set ui text to strCurrentText

}
QString MainWindow::findInsertSection(QString &strCurrentText, QTime &tInsertTime ,int &nStartIndex, int &nEndIndex)
{
    QString strFoundStartTime;
    QString strFoundEndTime;

    while (1){
        strFoundStartTime = "";
        strFoundEndTime = "";


        nStartIndex = strCurrentText.indexOf(QRegExp ("[1234567890]:[1234567890][1234567890]:[1234567890][1234567890] [AP]M"),nStartIndex);

        //qDebug() << "current Start index = " << nStartIndex;

        for (int iii = nStartIndex; iii < nStartIndex+11 ;iii++ )
        {
            if (strCurrentText[iii]== '\n')
                break;
            strFoundStartTime.append(strCurrentText[iii]);
        }
        QTime tStartTime = QTime::fromString(strFoundStartTime, "h:mm:ss AP");

        if (tStartTime > tInsertTime)
        {
            return "Insert time at begining";
        }

        nEndIndex = strCurrentText.indexOf(QRegExp ("[1234567890]:[1234567890][1234567890]:[1234567890][1234567890] [AP]M"),nStartIndex+1);

        for (int iii = nEndIndex; iii < nEndIndex+11 ;iii++ )
        {
            if (strCurrentText[iii]== '\n')
                break;
            strFoundEndTime.append(strCurrentText[iii]);
        }

        QTime tEndTime = QTime::fromString(strFoundEndTime, "h:mm:ss AP");

        if (tEndTime > tInsertTime && tStartTime < tInsertTime )
        {
          break;
        }
        if (nStartIndex < 0 && nEndIndex < 0)
                return "No Time present";
        if (nStartIndex > 0 && nEndIndex < 0)
                return "Insert time at the end";
        nStartIndex = nEndIndex;
    }// end while loop <---

    QString strSectionText;
    for (int III = nStartIndex; III < nEndIndex; III++)
        strSectionText.append(strCurrentText[III]);

    return strSectionText;

}

void MainWindow::on_actionInsert_time_break_triggered()
{
    InsertTimeDialog *InsertTime = new InsertTimeDialog(this);
    InsertTime->exec();
}

int MainWindow::findAmountTimeSavedInSection(QString &strSectionText, int &nStoredAs, int &nIndexOfTimeStoredInSection, QString &strSavedTime)
{
    if (strSectionText.contains(IGNORE_MARKER))
        nStoredAs = IGNORE;
    else if (strSectionText.contains(QRegExp ("[1234567890][1234567890]s")))
        nStoredAs = TRACK;
    else
        nStoredAs = APPEND;

    nIndexOfTimeStoredInSection = strSectionText.indexOf(QRegExp ("[1234567890][1234567890]s"));

    return stringWithTimeEnteredToMilliseconds(strSectionText, strSavedTime);
}
