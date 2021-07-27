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
    Spot spot = m_Quote->getSpot(static_cast<size_t>(0));
    return spot.getAdjClose();
}

double StockInfo::GetCurrentNetValue()
{
    return static_cast<double>(GetHolding()) * GetCurrentPrice();
}

vector<double> StockInfo::GetPrices()
{
    vector<double> ret;

    for (size_t i = 0; i < m_Quote->nbSpots(); i++) {
        Spot spot = m_Quote->getSpot(i);
        ret.push_back(spot.getAdjClose());
    }

    return ret;
}

vector<double> StockInfo::GetNetValues()
{
    vector<double> ret;

    for (size_t i = 0; i < m_Quote->nbSpots(); i++) {
        Spot spot = m_Quote->getSpot(i);
        ret.push_back(m_Holding * spot.getAdjClose());
    }

    return ret;
}

vector<double> StockInfo::GetReturnRates()
{
    vector<double> ret;
    double firstPrice = GetFirstPrice();

    for (size_t i = 0; i < m_Quote->nbSpots(); i++) {
        Spot spot = m_Quote->getSpot(i);
        ret.push_back(100.0 * (spot.getAdjClose() / firstPrice - 1));
    }

    return ret;
}

vector<time_t> StockInfo::GetDates()
{
    vector<time_t> ret;

    for (size_t i = 0; i < m_Quote->nbSpots(); i++) {
        Spot spot = m_Quote->getSpot(i);
        ret.push_back(spot.getDate());
    }

    return ret;
}

vector<string> StockInfo::GetDateStrs()
{
    vector<string> ret;

    for (size_t i = 0; i < m_Quote->nbSpots(); i++) {
        Spot spot = m_Quote->getSpot(i);
        ret.push_back(spot.getDateToString());
    }

    return ret;
}
