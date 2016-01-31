#include "calculations.h"


#include <qstring.h>
#include <qlocale.h>
#include <qmessagebox.h>



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
    strTime.remove("h").remove("m").remove("s").remove(":");
    int nLength = strTime.length();

    QString strReversed = strTime;
    QString Temp;
    for (int iii = 0, jjj = nLength-1; iii < nLength; iii++, jjj--)
    {

        strReversed[iii] = strTime[jjj];
    }

    int nTotalMilliseconds = 0;
    bool ok;
    for (int n = 0; n <nLength; n++)
    {
        QString strTempMultiplier;
        switch (n){
            case 0:     strTempMultiplier.append(strReversed[0]);
                        nTotalMilliseconds = 1000*strTempMultiplier.toInt(&ok,10);
                        break;
            case 1:     strTempMultiplier.append(strReversed[1]);
                        nTotalMilliseconds += (10000)*strTempMultiplier.toInt(&ok,10);
                        break;
            case 2:     strTempMultiplier.append(strReversed[2]);
                        nTotalMilliseconds += (60000)*strTempMultiplier.toInt(&ok,10);
                        break;
            case 3:     strTempMultiplier.append(strReversed[3]);
                        nTotalMilliseconds += (600000)*strTempMultiplier.toInt(&ok,10);
                        break;
            case 4:     strTempMultiplier.append(strReversed[4]);
                        nTotalMilliseconds += (3600000)*strTempMultiplier.toInt(&ok,10);
                        break;
            case 5:     strTempMultiplier.append(strReversed[5]);
                        nTotalMilliseconds += (36000000)*strTempMultiplier.toInt(&ok,10);
                        break;
            return -1;
            }
    }
    return nTotalMilliseconds;
}
