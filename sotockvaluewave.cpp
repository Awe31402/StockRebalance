#include "stockvaluewave.h"
#include "ui_sotockvaluewave.h"

SotockValueWave::SotockValueWave(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SotockValueWave)
{
    ui->setupUi(this);
}

SotockValueWave::~SotockValueWave()
{
    delete ui;
}
