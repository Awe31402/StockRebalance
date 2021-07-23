#include "stockinfo.h"
#include "spot.hpp"
#include <QDebug>

using namespace std;

int StockInfo::sRetryShot = 3;

StockInfo::StockInfo(QString name, QString startDate, QString endDate,
                     Quote *quote, int holding, double targetRatio)
{
    this->m_Name = name;
    this->m_Quote = quote;
    this->m_Holding = holding;
    this->m_TargetRatio = targetRatio;
    this->m_StartDate = startDate;
    this->m_EndDate = endDate;
}

StockInfo::~StockInfo()
{
    qDebug() << "Delete " << m_Name;
    if (m_Quote != nullptr) {
        delete m_Quote;
        m_Quote = nullptr;
    }
}

bool StockInfo::Init()
{
    int retryShot = 0;
    size_t spotSize = 0;

    while (retryShot < StockInfo::sRetryShot && spotSize == 0) {
        retryShot++;
        string startDate = m_StartDate.toStdString();
        string endDate = m_EndDate.toStdString();

        m_Quote->getHistoricalSpots(startDate.c_str(), endDate.c_str(), "1d");
        spotSize = m_Quote->nbSpots();
    }
    return (spotSize > 0);
}


QString StockInfo::GetName()
{
    return m_Name;
}

int    StockInfo::GetHolding()
{
    return m_Holding;
}

double StockInfo::GetTargetRatio()
{
    return m_TargetRatio;
}

double StockInfo::GetCurrentPrice()
{
    size_t spotSize = m_Quote->nbSpots();
    Spot spot = m_Quote->getSpot(spotSize - 1);

    return spot.getAdjClose();
}

double StockInfo::GetFirstPrice()
{
    Spot spot = m_Quote->getSpot((size_t) 0);
    return spot.getAdjClose();
}

double StockInfo::GetNetValue()
{
    return GetHolding() * GetCurrentPrice();
}
