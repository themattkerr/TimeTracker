#include "mainwindow.h"
#include "ui_mainwindow.h"
//#include "mattcalculations.h"
#include "calculations.h"
#include "loadfiledialog.h"

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
    m_nTotalTime = 0;
    m_nTotalIgnoredTime = 0;

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

    saveLog(m_strFileName);
    delete ui;

}




void MainWindow::on_pushButton_clicked()
{

    int elapsed = t.elapsed();
    m_nTotalTime = m_nTotalTime + elapsed;


    ui->textEdit->append(millisecondsToHoursMinsSec(elapsed));
    ui->textEdit->append("----------------------------------- ");
    ui->textEdit->append (t.currentTime().toString("h:mm:ss A"));
    ui->TotalTrackedTime->setText(millisecondsToHoursMinsSec(m_nTotalTime)  );
    //ui->textEdit->append("");

    t.restart();

}


void MainWindow::displayEllapsed()
{
    ui->CurrentTaskTime->setText(millisecondsToHoursMinsSec(t.elapsed()));
}

void MainWindow::on_ignoreButton_clicked()
{
    int elapsed = t.elapsed();
    m_nTotalIgnoredTime = m_nTotalIgnoredTime + elapsed;


    ui->textEdit->append(millisecondsToHoursMinsSec(elapsed));
    ui->textEdit->append("> Ignored <");
    ui->textEdit->append("----------------------------------- ");
    ui->textEdit->append (t.currentTime().toString("h:mm:ss A"));
    //ui->textEdit->append("");

    ui->timeIgnored->setText(millisecondsToHoursMinsSec(m_nTotalIgnoredTime)  );

    ui->timeIgnoredLabel->show();
    ui->timeIgnored->show();




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
            stmLog << ui->textEdit->toPlainText();
            stmLog << '\n' << '\n';
            stmLog << "Total Tracked Time:  " << millisecondsToHoursMinsSec(m_nTotalTime) << '\n';
            stmLog << "Total Time Ignored:  " << millisecondsToHoursMinsSec(m_nTotalIgnoredTime) << '\n' << '\n';
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

QString MainWindow::getFileName()
{


    QString strFileName;
    strFileName = "TimeTrackerFiles/LogFor";

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
       //line.remove("Total Tracked Time:  ");
       //line.remove("Total Time Ignored:  ");

       ui->textEdit->append(line);
       //x++;
   }

   file.close();

}
