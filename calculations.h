#ifndef CALCULATIONS
#define CALCULATIONS

#ifndef QSTRING_H
#include <qstring.h>
#endif

enum curency{
    US_DOLLARS = 0,
};

bool doubleIsEqual (double x, double y, unsigned int nNumberOfDecimalPlaces);

double usDollarsStringToDouble(QString qstrDollars);
QString doubleToCurrency (double dInput, unsigned int nCurrency);
QString millisecondsToHoursMinsSec (int nMilliseconds);
int stringToMilliseconds (QString strTime);
int TimeDifferencInMilliseconds (QString strFirstTime, QString strSecondTime);
QString removeAllLettersFromString(QString strInputString);
#endif // CALCULATIONS

