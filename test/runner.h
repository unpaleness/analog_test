#ifndef _RUNNER_H_
#define _RUNNER_H_

#include <QTimer>
#include "thermometer.h"
#include "manometer.h"

class Runner : public QObject
{
    Q_OBJECT

public:
    Runner();
    Runner(ThermoMeter *t, ManoMeter *m, int nSteps, int vDelay);
    ~Runner();

public slots:
    void update();

private:
    ThermoMeter *thermometer;
    ManoMeter *manometer;
    QTimer timer;
    int steps;
    int step;
    int delay; //ms
    bool inc; // 1 - increase, 0 - decrease

};

#endif // _RUNNER_H_
