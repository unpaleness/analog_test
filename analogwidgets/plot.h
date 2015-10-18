/*! 27/01/2015 Банченко А.А © ОАО "Завод "Агрохимпродукт".
 *  Объяыление класса, реализующий виджет-график.*/

#ifndef __PLOT_H__
#define __PLOT_H__

#include "widgetwithbackground.h"

#include <QtCore/QDateTime>
#include <QtCore/QMap>

class Plot : public WidgetWithBackground {
    Q_OBJECT

    Q_PROPERTY(double maximum READ maximum WRITE setMaximum)
    Q_PROPERTY(double minimum READ minimum WRITE setMinimum)
    Q_PROPERTY(QString parameterName READ parameterName WRITE setParameterName)

public:
    Plot(QWidget *parent = 0);

    QDateTime addValue(QDateTime, double);
    QMap<QDateTime, double> replaceValuesMap(QMap<QDateTime, double>);
    void clearValues();
    double maximum() const {
        return m_max;
    }
    double minimum() const {
        return m_min;
    }
    QString parameterName() const {
        return m_parameterName;
    }
    void setMaximum(double);
    void setMinimum(double);
    void setParameterName(QString name) {
        m_parameterName = name;
        updateWithBackground();
    }

public slots:
//    void init();
    void repaint();

protected:
    void initCoordinateSystem(QPainter & painter);
    void paintEvent(QPaintEvent *event);
    void paintBackground(QPainter & painter);

private:
    double m_max;
    double m_min;
    QString m_parameterName;
    double m_side;
    QDateTime m_start;
    bool m_isTimeShift;
    QDateTime m_finish;
    QMap<QDateTime, double> m_map;

};
#endif // __PLOT_H__
