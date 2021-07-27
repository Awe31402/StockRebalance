#include "stockwaveform.h"
#include "ui_stockwaveform.h"

StockWaveForm::StockWaveForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StockWaveForm)
{
    ui->setupUi(this);
}
StockWaveForm::~StockWaveForm()
{
    delete ui;
}

void StockWaveForm::SetStockInfo(vector<StockInfo*> vStockInfo)
{
    m_vStockInfo = vStockInfo;
    m_vPortfolioNetVal.clear();

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
    }

    PlotData(ui->comboBox_2->currentText(), ui->comboBox->currentIndex());

    connect(ui->comboBox, SIGNAL(currentTextChanged(QString)), this, SLOT(onStockNameChanged(QString)));
    connect(ui->comboBox_2, SIGNAL(currentTextChanged(QString)), this, SLOT(onDataTypeChanged(QString)));
}

void StockWaveForm::PlotData(QString type, int index)
{
    qDebug() << "Plot Data " + type + " " + ui->comboBox->itemText(index);
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
