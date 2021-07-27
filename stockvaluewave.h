#ifndef SOTOCKVALUEWAVE_H
#define SOTOCKVALUEWAVE_H

#include <QWidget>

namespace Ui {
class SotockValueWave;
}

class SotockValueWave : public QWidget
{
    Q_OBJECT

public:
    explicit SotockValueWave(QWidget *parent = nullptr);
    ~SotockValueWave();

private:
    Ui::SotockValueWave *ui;
};

#endif // SOTOCKVALUEWAVE_H
