#include "loadsockinfoworker.h"

LoadSockInfoWorker::LoadSockInfoWorker(QObject* parent)
{
}

void LoadSockInfoWorker::SetStockInfoVector(vector<StockInfo*> &vStockInfo)
{
    m_vStockInfo = vStockInfo;
}

void LoadSockInfoWorker::run()
{
    bool result = true;
    for (size_t i = 0; i < m_vStockInfo.size(); i++) {
        result &= m_vStockInfo[i]->Init();

        if (!result) {
            emit queryStockInfoDone(1);
        }
    }
    emit queryStockInfoDone(0);
}
