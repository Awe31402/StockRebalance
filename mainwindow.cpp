#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>

using  namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_LoadPortfolioWidget = new LoadPortfolioWidget(this);
    setCentralWidget(m_LoadPortfolioWidget);
}

MainWindow::~MainWindow()
{
    delete m_LoadPortfolioWidget;
    delete ui;
}



