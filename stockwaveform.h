#ifndef STOCKWAVEFORM_H
#define STOCKWAVEFORM_H

#include <QWidget>
#include "stockinfo.h"
#include <vector>

namespace Ui {
class StockWaveForm;
}

class StockWaveForm : public QWidget
{
    Q_OBJECT

public:
    explicit StockWaveForm(QWidget *parent = nullptr);
    ~StockWaveForm();
    void SetStockInfo(vector<StockInfo*> vStockInfo);
    void PlotData(QString type, int index);
private slots:
    void onDataTypeChanged(QString);
    void onStockNameChanged(QString);
private:
    Ui::StockWaveForm *ui;
    vector<StockInfo*> m_vStockInfo;
    vector<double> m_vPortfolioNetVal;
    vector<double> m_vPortfolioReturn;
    vector<double> m_vTimeData;

    QString m_DataType;
    QString m_StockName;
    int m_StockIndex;
};

#endif // STOCKWAVEFORM_H
