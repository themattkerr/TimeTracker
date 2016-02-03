#include "mainwindow.h"
#include "ui_mainwindow.h"
//#include "mattcalculations.h"
#include "calculations.h"
#include "loadfiledialog.h"
#include "changelogtitledialog.h"

#include <QDateTime>
#include <QTime>
#include <QTimer>

#include <QString>
#include <qdebug.h>
#include <QTextStream>

#include <QDir>
#include <QFile>

#include <QMessageBox>

/*

 -append the file when exiting.

 -save totaltime to this file and read it in.




 */

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    m_nLoadFileInfo = APPEND;//This must be append in order for file name logic to work

    m_bSaved = false;
    m_bFileNameSet= false;
    m_nTotalTime = 0;
    m_nTotalIgnoredTime = 0;
    m_strLogTitle = "LogFor";

    QFile f(getFileName());
    if (f.exists())
    {
        LoadFileDialog *LFI = new LoadFileDialog(this);
        LFI->exec();
    }
    m_strFileName = getFileName();

   // qDebug() << "stored file name" << m_strFileName;
   // qDebug() << "current setting for tracking " << m_nLoadFileInfo;

    ui->setupUi(this);
    Qt::WindowFlags flags = this->windowFlags();
    this->setWindowFlags(flags|Qt::WindowStaysOnTopHint);
    this->setWindowTitle("Time Tracker");
    setupLog();


    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(displayEllapsed()));
    timer->start(1000);


    if(m_nTotalTime == 0)
        ui->TotalTrackedTime->setText("---");
    else
        ui->TotalTrackedTime->setText(millisecondsToHoursMinsSec(m_nTotalTime)  );

    if(m_nTotalIgnoredTime == 0)
    {
        ui->timeIgnored->setText("---");
        ui->timeIgnoredLabel->hide();
        ui->timeIgnored->hide();
    }
    else
    {
        ui->timeIgnored->setText(millisecondsToHoursMinsSec(m_nTotalIgnoredTime)  );
        ui->timeIgnoredLabel->show();
        ui->timeIgnored->show();
    }

    ui->CurrentTaskLabel->hide();
    ui->CurrentTaskTime->hide();

    ui->textEdit->append (t.currentTime().toString("h:mm:ss A"));

    t.start();
}

MainWindow::~MainWindow()
{
    if(m_nLoadFileInfo == STARTNEW)
        m_strFileName = getFileName();
    saveLog(m_strFileName);
    delete ui;
}




void MainWindow::on_pushButton_clicked()
{

    m_nElapsed = t.elapsed();
    m_nTotalTime = m_nTotalTime + m_nElapsed;


    ui->textEdit->append(millisecondsToHoursMinsSec(m_nElapsed));
    ui->textEdit->append("----------------------------------- ");
    ui->textEdit->append (t.currentTime().toString("h:mm:ss A"));
    ui->TotalTrackedTime->setText(millisecondsToHoursMinsSec(m_nTotalTime)  );
    //ui->textEdit->append("");
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
    ui->textEdit->append("----------------------------------- ");
    ui->textEdit->append (t.currentTime().toString("h:mm:ss A"));
    //ui->textEdit->append("");

    ui->timeIgnored->setText(millisecondsToHoursMinsSec(m_nTotalIgnoredTime)  );

//    ui->timeIgnoredLabel->show();
//    ui->timeIgnored->show();

    m_nPreviousLogType = IGNORE;


    t.restart();
}

void MainWindow::saveLog(QString strFileName)
{
    QDir mDir;
    if (!mDir.exists("TimeTrackerFiles"))
    {
        mDir.mkdir("TimeTrackerFiles");
    }
    QFile Log(strFileName);

    if(Log.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text))
    {
        QTextStream stmLog(&Log);



        if (m_bSaved == false)
        {
            stmLog << "<> " << m_strLogTitle << " <>"<<'\n';
            stmLog << ui->textEdit->toPlainText();
            stmLog << '\n' << '\n';
            stmLog << "Total Tracked Time:  " << millisecondsToHoursMinsSec(m_nTotalTime) << '\n';
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
    strFileName = "TimeTrackerFiles/";
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
   //int x = 1;
   while (!file.atEnd())
   {
       QString line = in.readAll();
       line = scanForTitle(line);

       if (line.contains("Total Tracked Time:  "))
       {
           int x = line.lastIndexOf("Total Tracked Time:  ");
           QString TotalTime;
           for(int iii = (x+21); iii<(x+33); iii++)
           {
              if (line.at(iii) == '\n')
                  break;
               TotalTime.append(line.at(iii));
           }
           m_nTotalTime = stringToMilliseconds(TotalTime);
       }
       if (line.contains("Total Time Ignored:  "))
       {
           int x = line.lastIndexOf("Total Time Ignored:  ");
           QString TotalTime;
           for(int iii = (x+21); iii<(x+33); iii++)
           {
              if (line.at(iii) == '\n')
                  break;
               TotalTime.append(line.at(iii));
           }
           m_nTotalIgnoredTime = stringToMilliseconds(TotalTime);
       }
       if (line.contains("AM") || line.contains("PM") )
       {
           QString strLastTime;
           int lastTimeIndex=0;
           int am = line.lastIndexOf("AM");
           int pm = line.lastIndexOf("PM");
           if (am > pm)
               lastTimeIndex = am;
           else
               lastTimeIndex = pm;
           for (int iii = (lastTimeIndex-8); iii <= lastTimeIndex +2 ; iii ++)
           {
               strLastTime.append(line.at(iii));

           }
              //QMessageBox msbx;
              //msbx.setText(strLastTime);
              //msbx.exec();
            m_nLastRecordedTime = stringToMilliseconds(strLastTime);
       }


       //line.remove("Total Tracked Time:  ");
       //line.remove("Total Time Ignored:  ");

       ui->textEdit->append(line);
       ui->textEdit->append("");
       ui->textEdit->append("===================== ");
       ui->textEdit->append("");


       if(m_nLoadFileInfo == TRACK || m_nLoadFileInfo == IGNORE)
       {    
       int nCurrentTime = stringToMilliseconds(t.currentTime().toString("h:mm:ss A"));
       int nTimeDifference = nCurrentTime - m_nLastRecordedTime;
       if (nTimeDifference < 0)
       {
           QMessageBox msbx;
           msbx.setText("Error! Time difference is less than zero.");
           msbx.exec();
           m_nLoadFileInfo = APPEND;
           break;
       }
       else
       {
           QString strMissingTime = "Missing time:  ";
           strMissingTime.append(millisecondsToHoursMinsSec(nTimeDifference));
           ui->textEdit->append(strMissingTime);
            //ui->textEdit->append("Missing time:");
            //ui->textEdit->append(millisecondsToHoursMinsSec(nTimeDifference));
       }
       if (m_nLoadFileInfo == TRACK)
       {
           m_nTotalTime += nTimeDifference;
           ui->textEdit->append("Time has been logged as Tracked.");
       }
       if (m_nLoadFileInfo == IGNORE)
       {
           m_nTotalIgnoredTime += nTimeDifference;
           ui->textEdit->append("Time as been logged as Ignored.");
       }
       }
   }

   file.close();

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
        m_nTotalTime = m_nTotalTime - m_nElapsed;
        ui->TotalTrackedTime->setText(millisecondsToHoursMinsSec(m_nTotalTime));

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
        m_nTotalTime = m_nTotalTime + m_nElapsed;
        ui->TotalTrackedTime->setText(millisecondsToHoursMinsSec(m_nTotalTime));

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
