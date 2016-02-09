#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "calculations.h"
#include "loadfiledialog.h"
#include "changelogtitledialog.h"
#include "filterutilitydialog.h"

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

void MainWindow::initializeMemberVariables()
{
    m_nLoadFileInfo = APPEND;//This must be append in order for file name logic to work

    m_bSaved = false;
    m_bFileNameSet= false;
    m_nTotalTrackedTime = 0;
    m_nTotalIgnoredTime = 0;
    m_nTotalTime = 0;
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
    m_nElapsed = t.elapsed();
    m_nTotalTrackedTime = m_nTotalTrackedTime + m_nElapsed;

    ui->textEdit->append(millisecondsToHoursMinsSec(m_nElapsed));
    ui->textEdit->append(SECTION_BREAK);
    ui->textEdit->append (t.currentTime().toString("h:mm:ss A"));
    ui->TotalTrackedTime->setText(millisecondsToHoursMinsSec(m_nTotalTrackedTime)  );
    calculateTotalTime();
    m_nPreviousLogType = TRACK;

    t.restart();
}

void MainWindow::displayEllapsed()
{
    ui->CurrentTaskTime->setText(millisecondsToHoursMinsSec(t.elapsed()));
}

void MainWindow::on_ignoreButton_clicked()
{
    m_nElapsed = t.elapsed();
    m_nTotalIgnoredTime = m_nTotalIgnoredTime + m_nElapsed;

    ui->textEdit->append(millisecondsToHoursMinsSec(m_nElapsed));
    ui->textEdit->append("> Ignored <");
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
            stmLog << '\n' << t.currentTime().toString("h:mm:ss A") << " <-- Program exit time." << '\n';
            stmLog << SECTION_BREAK;
            stmLog << '\n' << '\n';
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
       ui->textEdit->append("===================== ");
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
    TotalTime.prepend("Total Tracked Time:  ");
    TotalTime.append('\n');
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
    TotalTime.append('\n');
    strInFile.remove(TotalTime);
}

void MainWindow::readInLastSavedTime(QString &strInFile)
{
    QString strLastTime;
    int lastTimeIndex=0;
    int am = strInFile.lastIndexOf("AM");
    int pm = strInFile.lastIndexOf("PM");
    if (am > pm)
        lastTimeIndex = am;
    else
        lastTimeIndex = pm;
    for (int iii = (lastTimeIndex-9); iii < lastTimeIndex + 2 ; iii ++)
    {
        strLastTime.append(strInFile.at(iii));

    }
//qDebug() << "strLastTime = " << strLastTime;
     m_nLastRecordedTime = stringToMilliseconds(strLastTime);

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
    }
    if (m_nLoadFileInfo == IGNORE)
    {
        m_nTotalIgnoredTime += nTimeDifference;
        ui->textEdit->append("Time as been logged as > Ignored <.");
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
   // m_nLoadFileInfo = EXIT_NO_SAVE;
    this->close();

}

void MainWindow::on_actionUndo_last_time_logging_triggered()
{

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
            if (strSection.contains("> Ignored <"))
                m_strFilteredIgnored.append(strSection);
            else
               m_strFilteredTracked.append(strSection);

            nCurrentStartIndex = strUnFiltered.indexOf(SECTION_BREAK, nNextIndex);
            nNextIndex = strUnFiltered.indexOf(SECTION_BREAK, nCurrentStartIndex+1);

            if(nNextIndex < 0)
            {
                nNextIndex = strUnFiltered.length();

            }


            qDebug() <<"\nCurrent index = " << nCurrentStartIndex;
            qDebug() << "Next Index   = " << nNextIndex;

        }
            qDebug() << "length of file = " << strUnFiltered.length();
    }

    return;

}



void MainWindow::on_actionFilter_Utility_triggered()
{
    FilterUtilityDialog *Filter = new FilterUtilityDialog(this);
    Filter->show();
    //getIgnoredText();
}
