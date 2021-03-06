#include "calculations.h"

#include <qDebug>
#include <qstring.h>
#include <qlocale.h>
#include <qmessagebox.h>
#include "constantants.h"



bool doubleIsEqual (double x, double y, unsigned int nNumberOfDecimalPlaces)
{
    int nMultiplyer = 1;
    for(int iii = 1; iii <= nNumberOfDecimalPlaces; iii++)
        nMultiplyer *= 10;
    return (static_cast<int>(x*nMultiplyer + 0.5) == static_cast<int>(y*nMultiplyer + 0.5));
}

double usDollarsStringToDouble(QString qstrDollars)
{
    bool ok;
    qstrDollars.remove('$').remove(',');
    double dValue = qstrDollars.toDouble(&ok);
    if (ok)
        return dValue;
    else
        return 0;
}

QString doubleToCurrency (double dInput, unsigned int nCurrency = US_DOLLARS)
{
    QString qstrCurrency;
    switch (nCurrency)
    {
        case US_DOLLARS:     {qstrCurrency = "$ "; break;}
    }

    qstrCurrency.append(QLocale (QLocale::English).toString(dInput));
    return qstrCurrency;
}

QString millisecondsToHoursMinsSec (int nMilliseconds)
{
    int hours;
    int mins;
    int sec;

    hours = nMilliseconds / 3600000;
    nMilliseconds = nMilliseconds % 3600000;

    mins = nMilliseconds / 60000;
    nMilliseconds = nMilliseconds % 60000;

    sec = nMilliseconds / 1000;

    QString strE;
    if (hours > 0 || mins > 0)
    {
        if (hours > 0)
        {
            if (hours < 10)
                strE.append("0");
            strE = strE.number(hours, 10); strE.append("h:");
        }
        if (mins < 10)
            strE.append("0");
        strE.append(QString::number(mins,10)); strE.append("m:");
    }
    if (sec < 10)
        strE.append("0");
    strE.append(QString::number(sec, 10)); strE.append("s");

    return strE;
}

int stringToMilliseconds (QString strTime)
{
    qDebug() << "string fed to stringToMilliseconds = " << strTime;
    bool bIsPMTime = false;
    strTime.remove(" ");
    strTime.remove("h").remove("m").remove("s").remove(":").remove('\n');
    int nLength = strTime.length();

    QString strReversed = strTime;

    for (int iii = 0, jjj = nLength-1; iii < nLength; iii++, jjj--)
    {
        strReversed[iii] = strTime[jjj];
    }

    int nTotalMilliseconds = 0;
    if (strReversed[1] == 'P')//this would indicate that it is PM
        {
            //nTotalMilliseconds = MILLISECONDS_PER_12HOUR;
            bIsPMTime = true;
        }
            strReversed.remove("A").remove("P").remove("M");
            nLength = strReversed.length();

    bool ok;
    for (int n = 0; n <nLength; n++)
    {
        QString strTempMultiplier("");
        switch (n){
            case 0:     strTempMultiplier.append(strReversed[0]);
                        nTotalMilliseconds += MILLISECONDS_PER_SEC*strTempMultiplier.toInt(&ok,10);
                        break;
            case 1:     strTempMultiplier.append(strReversed[1]);
                        nTotalMilliseconds += (MILLISECONDS_PER_10SEC)*strTempMultiplier.toInt(&ok,10);
                        break;
            case 2:     strTempMultiplier.append(strReversed[2]);
                        nTotalMilliseconds += (MILLISECONDS_PER_MIN)*strTempMultiplier.toInt(&ok,10);
                        break;
            case 3:     strTempMultiplier.append(strReversed[3]);
                        nTotalMilliseconds += (MILLISECONDS_PER_10MIN)*strTempMultiplier.toInt(&ok,10);
                        break;
            case 4:     strTempMultiplier.append(strReversed[4]);
                        nTotalMilliseconds += (MILLISECONDS_PER_HOUR)*strTempMultiplier.toInt(&ok,10);
                        break;
            case 5:     strTempMultiplier.append(strReversed[5]);
                        nTotalMilliseconds += (MILLISECONDS_PER_10HOUR)*strTempMultiplier.toInt(&ok,10);
                        break;
            return -1;
            }
    }
    if (nTotalMilliseconds < MILLISECONDS_PER_12HOUR && bIsPMTime)
        nTotalMilliseconds += MILLISECONDS_PER_12HOUR;
    if (nTotalMilliseconds > MILLISECONDS_PER_12HOUR && !bIsPMTime)
        nTotalMilliseconds -= MILLISECONDS_PER_12HOUR;

    if (!ok)
    {
        QMessageBox ConversionFail;
        ConversionFail.setText("Error! Time conversion failure. Time will not be logged");
        ConversionFail.exec();
        return 0;
    }
    if (nTotalMilliseconds < 0)
        return 0;
    qDebug() << "Output of stringToMilliseconds = " << nTotalMilliseconds;
    return nTotalMilliseconds;
}

int TimeDifferencInMilliseconds (QString strFirstTime, QString strSecondTime)
{
    int nT1 = stringToMilliseconds(strFirstTime);
    int nT2 = stringToMilliseconds(strSecondTime);
    int nTimeDifferrence = abs(nT2-nT1);
    return nTimeDifferrence;
}

QString removeAllLettersFromString(QString strInputString)
{
    QString strOutput = strInputString;

    strOutput
            .remove('A').remove('a')
            .remove('B').remove('b')
            .remove('C').remove('c')
            .remove('D').remove('d')
            .remove('E').remove('e')
            .remove('F').remove('f')
            .remove('G').remove('g')
            .remove('H').remove('h')
            .remove('I').remove('i')
            .remove('J').remove('j')
            .remove('K').remove('k')
            .remove('L').remove('l')
            .remove('M').remove('m')
            .remove('N').remove('n')
            .remove('O').remove('o')
            .remove('P').remove('p')
            .remove('Q').remove('q')
            .remove('R').remove('r')
            .remove('S').remove('s')
            .remove('T').remove('t')
            .remove('U').remove('u')
            .remove('V').remove('v')
            .remove('W').remove('w')
            .remove('X').remove('x')
            .remove('Y').remove('y')
            .remove('Z').remove('z')
            ;
    return strOutput;
}
