/***************************************************************************
 *   Copyright (C) 2006-2008 by Tomasz Ziobrowski                          *
 *   http://www.3electrons.com                                             *
 *   e-mail: t.ziobrowski@3electrons.com                                   *
 *                                                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include <QtGui>
#include <QMouseEvent>
#include <QMenu>
#include <cmath>
#include <assert.h>
#include "thermometer.h"
#include <iostream>

using namespace std;

ThermoMeter::ThermoMeter(QWidget* parent) : AbstractMeter(parent)
{
    m_max = 100.0;
    m_min = 0.0;
    setStepsScale(10); // Amount of main steps on scale
    setStepsSemiScale(5); // Amount of semi-steps on scale
    // Scale
    m_maximum = m_max; // Scale maximum
    setMinimum(m_min); // Scale minimum
    setNominal(40.0); // Nominal start value
    setCritical(80.0); // Critical start value
    calcMaxMin(); // Extend max and min to have nice step values
    // Current
    setValue(42.0);
    // Miscellaneous
    setValueOffset(268); // Current value downshift (from the very top)
    setDigitOffset(12); // Scale values rightshift
    setSuffix(QString(" [C]")); // Measures
    setDigitFont(QFont("RobotoBold", 12, QFont::Normal));
    setValueFont(QFont("RobotoBold", 16, QFont::Normal));
    // QWidget
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setWindowTitle(tr("Analog Thermometer"));
    resize(200, 200);
    assert(m_max - m_min != 0);
}

void ThermoMeter::initCoordinateSystem(QPainter& painter) {
    // inicjalizacja paintera
    painter.setRenderHint(QPainter::Antialiasing);
    //painter.translate(width() / 2, height() / 2);
    painter.translate(width() / 2.0, 0.0);
    painter.scale(height() / 300.0, height() / 307.0);
}

void ThermoMeter::paintBackground(QPainter& painter)
{
    // Prepare background
//    painter.setBrush(QColor(63, 63, 63));
//    painter.drawRect(-1, -1, width() + 2, height() + 2);
    // Without borders
    painter.setPen(Qt::NoPen);
    // Centering
    initCoordinateSystem(painter);

    /*** INITIALIZING ***/
    QColor colorLow(63, 31, 0, 255);
//    QColor colorNominal(3, 169, 249, 255);
//    QColor colorCritical(255, 127, 127, 255);
    QColor colorNominal(0, 0, 191, 255);
    QColor colorCritical(191, 0, 0, 255);
    // Rod bottom color (lower pie part)
    QRadialGradient gradBottom(QPointF(0.0, 255.0), 20.0, QPointF(0.0, 255.0));
    gradBottom.setColorAt(0.0, QColor(255, 255, 255, 0));
    gradBottom.setColorAt(1.0, QColor(63, 31, 0, 191));
    // Color of glass rod
    QLinearGradient gradRod(QPointF(-10.0, 0.0), QPointF(10.0, 0.0));
    gradRod.setColorAt(0.0, QColor(63, 31, 0, 191));
    gradRod.setColorAt(0.5, QColor(255, 255, 255, 0));
    gradRod.setColorAt(1.0, QColor(63, 31, 0, 191));
    // Rod top color (cap)
    QRadialGradient gradRoof(QPointF(0.0, 22.5), 10.0, QPointF(0.0, 22.5));
    gradRoof.setColorAt(0.0, QColor(255, 255, 255, 0));
    gradRoof.setColorAt(1.0, QColor(63, 31, 0, 191));

    // Glass rod path
    QPainterPath pathRod;
    pathRod.moveTo(-10.0, 237.6795);
    pathRod.lineTo(-10.0, 22.5);
    pathRod.lineTo(10.0, 22.5);
    pathRod.lineTo(10.0, 237.6795);
    pathRod.lineTo(0.0, 255.0);

    /*** DRAWING ***/
    // Draw bottom
    painter.setBrush(gradBottom);
    painter.drawPie(-20, 235, 40, 40, -240 * 16, 300 * 16);
    // Draw glass rod
    painter.setBrush(gradRod);
    painter.drawPath(pathRod);
    // Draw roof
    painter.setBrush(gradRoof);
    painter.drawPie(QRectF(-10.0, 12.5, 20.0, 20.0), 0 * 16, 180 * 16);
    // Draw scale values
    if (digitOffset())
    {
        // 29/01/2015 оставляем только "точки перехода", т.е., например граница
        // между отрезком нормальных и критичных значений.
        painter.setFont(digitFont());
        QFontMetrics fm(digitFont());
        // минимум
        painter.setPen(colorLow);
        QString val = QString::number(m_min);
        double y = 233.0;
        painter.drawText(QPointF(digitOffset(), y + fm.height() / 2.0), val);
        // номинал
        painter.setPen(colorNominal);
        val = QString::number(nominal());
        y = 233.0 - (214.0 / (m_max - m_min)) * (nominal() - m_min);
        painter.drawText(QPointF(digitOffset(), y + fm.height() / 2.0), val);
        // критические значения
        painter.setPen(colorCritical);
        val = QString::number(critical());
        y = 233.0 - (214.0 / (m_max - m_min)) * (critical() - m_min);
        painter.drawText(QPointF(digitOffset(), y + fm.height() / 2.0), val);
        // максимум
        val = QString::number(m_max);
        y = 19.0;
        painter.drawText(QPointF(digitOffset(), y + fm.height() / 2.0), val);
    }
} // paintBackground

void ThermoMeter::paintEvent(QPaintEvent* )
{
    // Draw empty thermometer
    drawBackground();
    // Prepare painter without borders
    QPainter painter(this);
    painter.setPen(Qt::NoPen);
    // Centering
    initCoordinateSystem(painter);

    /*** INITIALIZING ***/
    QColor colorLow(63, 31, 0, 255);
    QColor colorNominal(0, 0, 191, 255);
    QColor colorCritical(191, 0, 0, 255);
    // Setting up mercury color
    QColor colorMercury(colorLow);
    if (m_value >= m_nominal)
        colorMercury = colorNominal;
    if (m_value >= m_critical)
        colorMercury = colorCritical;
    // Mercury bottom color
    QRadialGradient gradMercuryBottom(QPointF(0.0, 255.0), 20.0, QPointF(0.0, 255.0));
    gradMercuryBottom.setColorAt(0.0, QColor(255, 255, 255, 0));
    gradMercuryBottom.setColorAt(0.6, colorMercury);
    gradMercuryBottom.setColorAt(1.0, QColor(255, 255, 255, 0));
    // Mercury rod gradient
    QLinearGradient gradMercuryRod(QPointF(-10.0, 0.0), QPointF(10.0, 0.0));
    gradMercuryRod.setColorAt(0.0, QColor(255, 255, 255, 0));
    gradMercuryRod.setColorAt(0.2, colorMercury);
    gradMercuryRod.setColorAt(0.5, QColor(255, 255, 255, 0));
    gradMercuryRod.setColorAt(0.8, colorMercury);
    gradMercuryRod.setColorAt(1.0, QColor(255, 255, 255, 0));
    // Mercury rod path
    QPainterPath pathMercuryRod;
    pathMercuryRod.moveTo(10.0, 237.6795);
    pathMercuryRod.lineTo(0.0, 255.0);
    pathMercuryRod.lineTo(-10.0, 237.6795);
    pathMercuryRod.lineTo(-10.0, (m_max - m_value) / (m_max - m_min) * 214 + 24.0);
    pathMercuryRod.lineTo(10.0, (m_max - m_value) / (m_max - m_min) * 214 + 24.0);
    // Unreached marks
    QLinearGradient gradMarks(QPointF(-8.0, 0.0), QPointF(8.0, 0.0));
    gradMarks.setColorAt(0.0, QColor(0, 0, 0, 127));
    gradMarks.setColorAt(0.5, QColor(0, 0, 0, 0));
    gradMarks.setColorAt(1.0, QColor(0, 0, 0, 127));
    // Reached marks
    QLinearGradient gradMarksPassed(QPointF(-8.0, 0.0), QPointF(8.0, 0.0));
    gradMarksPassed.setColorAt(0.0, QColor(128, 0, 0, 255));
    gradMarksPassed.setColorAt(0.5, QColor(128, 0, 0, 0));
    gradMarksPassed.setColorAt(1.0, QColor(128, 0, 0, 255));

    /*** DRAWING ***/
    // Draw scale
    // Total height of scale is 224
    qreal length;
    qreal valueSemiStep = (m_max - m_min) / m_stepsScale / m_stepsSemiScale;
    qreal heightSemiStep = valueSemiStep * 214.0 / (m_max - m_min);
    for (int i = 0; i <= stepsScale() * stepsSemiScale(); i++)
    {
        if (i % stepsSemiScale())
            length = 8.0;
        else
            length = 16.0;
        if (m_value - m_min >= valueSemiStep * i)
            painter.setBrush(gradMarksPassed);
        else
            painter.setBrush(gradMarks);
        painter.drawRect(QRectF(-length / 2.0, 235.0 - heightSemiStep * i, length, 3.0));
    }
    // Draw mercury bottom
    painter.setBrush(gradMercuryBottom);
    painter.drawPie(-20, 235, 40, 40, -240.0 * 16, 300.0 * 16);
    // Draw mercury rod
    painter.setBrush(gradMercuryRod);
    painter.drawPath(pathMercuryRod);
    // Draw value
    if (valueOffset())
    {
        painter.setFont(valueFont());
        //  29/01/2015 Разукрасим текущее значение под цвет столбика в термометре
        painter.setPen(colorMercury);
        QString Str = prefix() + QString("%1").arg(value()) + suffix();
        QSize Size = painter.fontMetrics().size(Qt::TextSingleLine, Str);
        painter.drawText(QPointF(Size.width() / -2, valueOffset() + Size.height()) , Str);
    }
} // paintEvent

void ThermoMeter::contexMenuPrefix(QMouseEvent *event)
{
    QMenu menu(this);
    menu.addAction(" [C]");
    menu.addAction(" [grad] ");
    menu.connect(&menu, SIGNAL(triggered(QAction*)), this, SLOT(setSuffixOnMenuAction(QAction*)));
    menu.exec(event->globalPos());
}
