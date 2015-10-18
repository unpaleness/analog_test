#include <QApplication>
#include <QWidget>

#include "thermometer.h"
#include "manometer.h"
#include "runner.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    ThermoMeter widgetThermoMeter;
    ManoMeter widgetManoMeter;
    Runner runner(&widgetThermoMeter, &widgetManoMeter, 400, 10);

    widgetThermoMeter.show();
    widgetManoMeter.show();

    return a.exec();
}
