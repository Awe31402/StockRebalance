#include "loadportfoliowidget.h"
#include "ui_loadportfoliowidget.h"
#include <iostream>
using namespace std;

LoadPortfolioWidget::LoadPortfolioWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoadPortfolioWidget)
{
    ui->setupUi(this);
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(onAddStockClicked()));
    connect(ui->pushButton_2, SIGNAL(clicked()), this, SLOT(onLoadDataClicked()));
    ui->pushButton_2->setEnabled(false);
    m_vStockInfo.clear();
    m_pLoadStockInfoWorker = new LoadSockInfoWorker(this);
    connect(m_pLoadStockInfoWorker, SIGNAL(queryStockInfoDone(int)), this, SLOT(onQueryStockInfoDone(int)));
}

LoadPortfolioWidget::~LoadPortfolioWidget()
{
    for (size_t i = 0; i < m_vStockInfo.size(); i++)
        delete m_vStockInfo[i];

    m_vStockInfo.clear();
    delete ui;
}

void LoadPortfolioWidget::onAddStockClicked()
{
    QString strFileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                       "/home",
                                                       tr("(*csv)"));
    m_vStockInfo.clear();
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
    ui->pushButton_2->setEnabled(true);
}

void LoadPortfolioWidget::onLoadDataClicked()
{
    qDebug() << "Load Data Clicked";
    ui->pushButton_2->setEnabled(false);

    int tableRow = ui->tableWidget->rowCount();
    QDate today = QDate::currentDate();
    QDate halfYearAgo = today.addMonths(-12);

    QString strToday = today.toString("yyyy-MM-dd");
    QString strHalfYear = halfYearAgo.toString("yyyy-MM-dd");

    for (int i = 0; i < tableRow; i++) {
        QTableWidgetItem *nameItem = ui->tableWidget->item(i, 0);
        QTableWidgetItem *holdingItem = ui->tableWidget->item(i, 1);
        QTableWidgetItem *targetRatioItem = ui->tableWidget->item(i, 2);

        QString stockName = nameItem->text();
        Quote *quote = new Quote(stockName.toStdString());
        int holding = holdingItem->text().toInt();
        double targetRatio = targetRatioItem->text().toDouble();
        m_vStockInfo.push_back(new StockInfo(stockName,
                                         strHalfYear,
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
    if (evNum != 0)
        return;

    vector<double> vPrice;
    vector<double> vNetVal;
    vector<double> vReturn;

    ui->tableWidget->setColumnCount(7);
    ui->tableWidget->setHorizontalHeaderItem(3, new QTableWidgetItem("Price"));
    ui->tableWidget->setHorizontalHeaderItem(4, new QTableWidgetItem("Net Value"));
    ui->tableWidget->setHorizontalHeaderItem(5, new QTableWidgetItem("Return(%)"));
    ui->tableWidget->setHorizontalHeaderItem(6, new QTableWidgetItem("Current Ratio(%)"));

    double totalVal = 0.0;
    for (size_t i = 0; i < m_vStockInfo.size(); i++) {
        double price = m_vStockInfo[i]->GetCurrentPrice();
        double netVal = m_vStockInfo[i]->GetNetValue();
        double return_rate = 100.0 * ((m_vStockInfo[i]->GetCurrentPrice() / m_vStockInfo[i]->GetFirstPrice())-1);

        vPrice.push_back(price);
        vNetVal.push_back(netVal);
        vReturn.push_back(return_rate);
        totalVal += netVal;
    }

    for (size_t i = 0; i < m_vStockInfo.size(); i++) {
        ui->tableWidget->setItem(i, static_cast<size_t>(3), new QTableWidgetItem(QString::number(vPrice[i])));
        ui->tableWidget->setItem(i, static_cast<size_t>(4), new QTableWidgetItem(QString::number(vNetVal[i])));
        ui->tableWidget->setItem(i, static_cast<size_t>(5), new QTableWidgetItem(QString::number(vReturn[i])));
        ui->tableWidget->setItem(i, static_cast<size_t>(6), new QTableWidgetItem(QString::number(100.0 * vNetVal[i]/totalVal)));
    }
    ui->pushButton_2->setEnabled(true);
}