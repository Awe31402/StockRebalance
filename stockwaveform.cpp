#include "stockwaveform.h"
#include "ui_stockwaveform.h"

StockWaveForm::StockWaveForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StockWaveForm)
{
    ui->setupUi(this);
    ui->widget->addGraph();
}
StockWaveForm::~StockWaveForm()
{
    delete ui;
}

void StockWaveForm::SetStockInfo(vector<StockInfo*> vStockInfo)
{
    m_vStockInfo = vStockInfo;
    m_vPortfolioNetVal.clear();
    m_vTimeData.clear();
    ui->comboBox->clear();

    vector<time_t> vTimeData = m_vStockInfo[0]->GetDates();

    for (size_t i = 0; i < m_vStockInfo.size(); i++) {
        vector<double> vNetVal = m_vStockInfo[i]->GetNetValues();

        for (size_t j = 0; j < vNetVal.size(); j++) {
            if (m_vPortfolioNetVal.size() <= j) {
                m_vPortfolioNetVal.push_back(0.0);
            }

            m_vPortfolioNetVal[j] += vNetVal[j];
        }

        ui->comboBox->addItem(m_vStockInfo[i]->GetName());
    }

    ui->comboBox->addItem("Portfolio");
    for (size_t i = 0; i < m_vPortfolioNetVal.size(); i++) {
        double dReturn = 100.0 * (m_vPortfolioNetVal[i] / m_vPortfolioNetVal[0] - 1);
        m_vPortfolioReturn.push_back(dReturn);
        m_vTimeData.push_back(vTimeData[i]);
    }

    PlotData("Net Value", 0);

    connect(ui->comboBox, SIGNAL(currentTextChanged(QString)), this, SLOT(onStockNameChanged(QString)));
    connect(ui->comboBox_2, SIGNAL(currentTextChanged(QString)), this, SLOT(onDataTypeChanged(QString)));
}

void StockWaveForm::PlotData(QString type, int index)
{
    qDebug() << "Plot Data " + type + " " + ui->comboBox->itemText(index);

    vector<double> vData;
    if (type == "Return") {
        vData = (ui->comboBox->currentText() == "Portfolio")? m_vPortfolioReturn : m_vStockInfo[index]->GetReturnRates();
    } else {
        vData = (ui->comboBox->currentText() == "Portfolio")? m_vPortfolioNetVal : m_vStockInfo[index]->GetNetValues();
    }

    QVector<QCPGraphData> timeData(vData.size());

    for (int i = 0; i < vData.size(); i++) {
        timeData[i].key = m_vTimeData[i];
        timeData[i].value = vData[i];
    }

    ui->widget->graph()->data()->set(timeData);
    QSharedPointer<QCPAxisTickerDateTime> dateTicker(new QCPAxisTickerDateTime);
    dateTicker->setDateTimeFormat("d. MMMM\nyyyy");
    ui->widget->xAxis->setTicker(dateTicker);

    ui->widget->rescaleAxes();
    ui->widget->replot();
}

void StockWaveForm::onDataTypeChanged(QString)
{
    m_DataType = ui->comboBox_2->currentText();
    PlotData(m_DataType, ui->comboBox->currentIndex());
}

void StockWaveForm::onStockNameChanged(QString name)
{
    m_StockName = ui->comboBox->currentText();
    PlotData(m_DataType, ui->comboBox->currentIndex());
}
