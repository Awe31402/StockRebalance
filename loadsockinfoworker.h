#ifndef LOADSOCKINFOWORKER_H
#define LOADSOCKINFOWORKER_H

#include <QObject>
#include <QThread>
#include <vector>
#include <stockinfo.h>
#include <vector>

class LoadSockInfoWorker : public QThread
{
    Q_OBJECT
public:
    explicit LoadSockInfoWorker(QObject* parent=0);
    void SetStockInfoVector(vector<StockInfo*> &vStockInfo);
    void run();
signals:
    void queryStockInfoDone(int);
    void queryStockProgress(QString, int);
private:
    vector<StockInfo*> m_vStockInfo;
};

#endif // LOADSOCKINFOWORKER_H
