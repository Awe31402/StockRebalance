#ifndef LOADPORTFOLIOWIDGET_H
#define LOADPORTFOLIOWIDGET_H

#include <QWidget>
#include <QFileDialog>
#include <QStringList>
#include <QFile>
#include <QDebug>
#include "quote.hpp"
#include "curl_utils.hpp"
#include "time_utils.hpp"
#include <QDate>
#include <QMessageBox>
#include <vector>
#include <stockinfo.h>
#include "loadsockinfoworker.h"

namespace Ui {
class LoadPortfolioWidget;
}

class LoadPortfolioWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LoadPortfolioWidget(QWidget *parent = nullptr);
    ~LoadPortfolioWidget();
private slots:
    void onLoadDataClicked();
    void onAddStockClicked();
    void onRebalanceClicked();
    void onQueryStockInfoDone(int);
    void onQueryStockProgress(QString, int);
private:
    void cleanStockInfoList();
    Ui::LoadPortfolioWidget *ui;
    vector<StockInfo*> m_vStockInfo;
    LoadSockInfoWorker *m_pLoadStockInfoWorker;
    double m_TotalVal;
    double m_PortfolioReturn;
};

#endif // LOADPORTFOLIOWIDGET_H
