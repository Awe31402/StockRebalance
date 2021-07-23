#ifndef STOCKINFO_H
#define STOCKINFO_H
#include <iostream>
#include "quote.hpp"
#include <QString>

using namespace std;

class StockInfo
{
public:
    StockInfo(QString name, QString startDate, QString endDate,
              Quote *m_Quote, int holding, double targetRatio);
    ~StockInfo();
    bool Init();
    QString GetName();
    int    GetHolding();
    double GetTargetRatio();
    double GetCurrentPrice();
    double GetFirstPrice();
    double GetNetValue();
private:
    QString m_Name;
    QString m_StartDate;
    QString m_EndDate;
    Quote  *m_Quote;
    double m_TargetRatio;
    int m_Holding;
    double m_CurrentPrice;
    double m_FirstPrice;
    double m_NetValue;
    static int sRetryShot;
};

#endif // STOCKINFO_H
