#include "loadportfoliowidget.h"
#include "ui_loadportfoliowidget.h"
#include <iostream>
#include <math.h>

using namespace std;

LoadPortfolioWidget::LoadPortfolioWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoadPortfolioWidget)
{
    ui->setupUi(this);
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(onAddStockClicked()));
    connect(ui->pushButton_2, SIGNAL(clicked()), this, SLOT(onLoadDataClicked()));
    connect(ui->pushButton_3, SIGNAL(clicked()), this, SLOT(onRebalanceClicked()));

    ui->pushButton_2->setEnabled(false);
    ui->progressBar->setVisible(false);
    ui->lineEdit->setText("0");
    ui->label->setVisible(false);
    ui->lineEdit->setVisible(false);
    ui->pushButton_3->setVisible(false);
    ui->label_3->setVisible(false);
    ui->label_4->setVisible(false);

    ui->dateEdit->setDate(QDate::currentDate().addMonths(-6));

    m_vStockInfo.clear();
    m_pLoadStockInfoWorker = new LoadSockInfoWorker(this);

    connect(m_pLoadStockInfoWorker, SIGNAL(queryStockInfoDone(int)), this, SLOT(onQueryStockInfoDone(int)));
    connect(m_pLoadStockInfoWorker, SIGNAL(queryStockProgress(QString, int)),
             this, SLOT(onQueryStockProgress(QString, int)));
}

LoadPortfolioWidget::~LoadPortfolioWidget()
{
    cleanStockInfoList();
    delete ui;
}

void LoadPortfolioWidget::onRebalanceClicked()
{
    double joinDollars = ui->lineEdit->text().toDouble();
    double totalAddJoin = m_TotalVal + joinDollars;
    double totalJoin = 0.0;

    ui->tableWidget->setColumnCount(11);
    ui->tableWidget->setHorizontalHeaderItem(7, new QTableWidgetItem("Add Value"));
    ui->tableWidget->setHorizontalHeaderItem(8, new QTableWidgetItem("Buy Count"));
    ui->tableWidget->setHorizontalHeaderItem(9, new QTableWidgetItem("Net Value after rebalance"));
    ui->tableWidget->resizeColumnToContents(9);
    ui->tableWidget->setHorizontalHeaderItem(10, new QTableWidgetItem("Ratio after rebalance"));
    ui->tableWidget->resizeColumnToContents(10);

    for (int i = 0; i < ui->tableWidget->rowCount(); i++)
    {
        double targetRatio = ui->tableWidget->item(i, 2)->text().toDouble();
        double netAfterRebalance = totalAddJoin * targetRatio / 100.0;

        double addValue = netAfterRebalance - m_vStockInfo[i]->GetCurrentNetValue();
        double price = m_vStockInfo[i]->GetCurrentPrice();
        int buyCount = static_cast<int> (floor( addValue/price));
        double actNetAfterRebalance = m_vStockInfo[i]->GetCurrentNetValue() + (double)buyCount * price;
        double ratioAfterRebalance = (totalAddJoin == 0.0)? 0.0 : 100.0 * actNetAfterRebalance / totalAddJoin;
        addValue = (double) buyCount * price;
        totalJoin += addValue;

        ui->tableWidget->setItem(i, static_cast<size_t>(7), new QTableWidgetItem(QString::number(addValue)));
        ui->tableWidget->setItem(i, static_cast<size_t>(8), new QTableWidgetItem(QString::number(buyCount)));
        ui->tableWidget->setItem(i, static_cast<size_t>(9), new QTableWidgetItem(QString::number(actNetAfterRebalance)));
        ui->tableWidget->setItem(i, static_cast<size_t>(10), new QTableWidgetItem(QString::number(ratioAfterRebalance)));
    }
    ui->label_3->setText("Actual Join Dollars is: " + QString::number(totalJoin)
                         + ", Net Val after rebalalce: " + QString::number(m_TotalVal + totalJoin));
    ui->label_3->setVisible(true);
}

void LoadPortfolioWidget::onAddStockClicked()
{
    QString strFileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                       "/home",
                                                       tr("(*csv)"));
    m_vStockInfo.clear();
    ui->tableWidget->setRowCount(0);
    QFile file(strFileName);
    if (!file.open(QIODevice::ReadOnly))
        return;

    QStringList wordList;
    bool bFirstLine = true;

    while(!file.atEnd()) {
        QString line = file.readLine();
        if (bFirstLine){
            bFirstLine = false;
            continue;
        }

        int currentRow = ui->tableWidget->rowCount();
        ui->tableWidget->insertRow(currentRow);
        wordList = line.split(",", QString::SkipEmptyParts);
        for (int i = 0; i < wordList.size(); i++) {
            QTableWidgetItem *item = new QTableWidgetItem();
            item->setText(wordList[i].trimmed());
            ui->tableWidget->setItem(currentRow, i, item);
        }
    }
    ui->label_4->setVisible(false);
    ui->pushButton_2->setEnabled(true);
}

void LoadPortfolioWidget::onLoadDataClicked()
{
    qDebug() << "Load Data Clicked";
    ui->pushButton_2->setEnabled(false);

    cleanStockInfoList();

    int tableRow = ui->tableWidget->rowCount();
    QDate today = QDate::currentDate();
    QDate lastRebalaceDate = ui->dateEdit->date();

    QString strToday = today.toString("yyyy-MM-dd");
    QString strLastRebalanceDate = lastRebalaceDate.toString("yyyy-MM-dd");

    for (int i = 0; i < tableRow; i++) {
        QTableWidgetItem *nameItem = ui->tableWidget->item(i, 0);
        QTableWidgetItem *holdingItem = ui->tableWidget->item(i, 1);
        QTableWidgetItem *targetRatioItem = ui->tableWidget->item(i, 2);

        QString stockName = nameItem->text();
        Quote *quote = new Quote(stockName.toStdString());
        int holding = holdingItem->text().toInt();
        double targetRatio = targetRatioItem->text().toDouble();
        m_vStockInfo.push_back(new StockInfo(stockName,
                                         strLastRebalanceDate,
                                         strToday,
                                         quote,
                                         holding,
                                         targetRatio));
    }
    m_pLoadStockInfoWorker->SetStockInfoVector(m_vStockInfo);
    m_pLoadStockInfoWorker->run();
}

void LoadPortfolioWidget::onQueryStockInfoDone(int evNum)
{
    ui->progressBar->setVisible(false);
    if (evNum != 0) {
        QMessageBox warningBox;
        warningBox.setText("Error while query information");
        warningBox.setStandardButtons(QMessageBox::Close);
        ui->pushButton_2->setEnabled(true);
        cleanStockInfoList();
        return;
    }

    vector<double> vPrice;
    vector<double> vNetVal;
    vector<double> vReturn;

    ui->tableWidget->setColumnCount(7);
    ui->tableWidget->setHorizontalHeaderItem(3, new QTableWidgetItem("Price"));
    ui->tableWidget->setHorizontalHeaderItem(4, new QTableWidgetItem("Net Value"));
    ui->tableWidget->setHorizontalHeaderItem(5, new QTableWidgetItem("Return(%)"));
    ui->tableWidget->setHorizontalHeaderItem(6, new QTableWidgetItem("Current Ratio(%)"));
    ui->tableWidget->resizeColumnToContents(6);

    double beforeVal = 0.0;
    m_TotalVal = 0.0;
    m_PortfolioReturn = 0.0;

    for (size_t i = 0; i < m_vStockInfo.size(); i++) {
        beforeVal += m_vStockInfo[i]->GetFirstPrice() * m_vStockInfo[i]->GetHolding();
        double price = m_vStockInfo[i]->GetCurrentPrice();
        double netVal = m_vStockInfo[i]->GetCurrentNetValue();
        double return_rate = 100.0 * ((m_vStockInfo[i]->GetCurrentPrice() / m_vStockInfo[i]->GetFirstPrice())-1.0);
        vPrice.push_back(price);
        vNetVal.push_back(netVal);
        vReturn.push_back(return_rate);
        m_TotalVal += netVal;
    }
    m_PortfolioReturn =(beforeVal == 0.0)? 0.0 : 100.0 * ((m_TotalVal / beforeVal) - 1.0);

    for (size_t i = 0; i < m_vStockInfo.size(); i++) {
        double ratio = (m_TotalVal == 0.0)? 0.0 : 100.0 * vNetVal[i]/m_TotalVal;


        ui->tableWidget->setItem(i, static_cast<size_t>(3), new QTableWidgetItem(QString::number(vPrice[i])));
        ui->tableWidget->setItem(i, static_cast<size_t>(4), new QTableWidgetItem(QString::number(vNetVal[i])));
        ui->tableWidget->setItem(i, static_cast<size_t>(5), new QTableWidgetItem(QString::number(vReturn[i])));
        ui->tableWidget->setItem(i, static_cast<size_t>(6), new QTableWidgetItem(QString::number(ratio)));
    }

    QWidget* widget = ui->horizontalTabWidget->widget(1);

    if (widget == nullptr) {
        StockWaveForm *newForm = new StockWaveForm(this);
        newForm->SetStockInfo(m_vStockInfo);
        ui->horizontalTabWidget->addTab(newForm, "Plots");
    } else {
        StockWaveForm* waveformWidget = dynamic_cast<StockWaveForm*>(widget);
        if (waveformWidget != nullptr)
            waveformWidget->SetStockInfo(m_vStockInfo);
    }

    ui->pushButton_2->setEnabled(true);
    ui->label->setVisible(true);
    ui->lineEdit->setVisible(true);
    ui->pushButton_3->setVisible(true);
    ui->label_4->setVisible(true);
    ui->label_4->setText("Portfoio Return is: " + QString::number(m_PortfolioReturn)
                         + ",Net Value Before Rebalance " + QString::number(m_TotalVal));

}

void LoadPortfolioWidget::onQueryStockProgress(QString stockName, int progress)
{
    ui->progressBar->setVisible(true);
    ui->progressBar->setValue(progress);
}

void LoadPortfolioWidget::cleanStockInfoList()
{
    for (size_t i = 0; i < m_vStockInfo.size(); i++)
        delete m_vStockInfo[i];

    m_vStockInfo.clear();
}
