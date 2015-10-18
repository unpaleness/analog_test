#include "runner.h"

Runner::Runner() {}

Runner::Runner(ThermoMeter *t, ManoMeter* m, int nSteps, int vDelay)
{
    thermometer = t;
    manometer = m;
    steps = nSteps;
    delay = vDelay;
    inc = 1;
    step = 0;
    connect(&timer, SIGNAL(timeout()), this, SLOT(update()));
    timer.start(delay);
}

Runner::~Runner() {}

void Runner::update()
{
    if(inc)
        step++;
    else
        step--;
    if(step >= steps)
    {
        inc = !inc;
        step = steps;
    }
    if(step < 0)
    {
        inc = !inc;
        step = 0;
    }
    thermometer->setValue((thermometer->maximum() - thermometer->minimum()) * step / steps + thermometer->minimum());
    manometer->setValue((manometer->maximum() - manometer->minimum()) * step / steps + manometer->minimum());
}
