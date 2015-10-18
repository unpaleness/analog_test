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
#include <QAction>
#include <cmath>
#include <assert.h>
#include <iostream>

#include "manometer.h"
#define PI 3.141592653589793238512808959406186204433

using namespace Qt;

ManoMeter::ManoMeter(QWidget *parent) : AbstractMeter(parent)
{
    m_max = 100.0; // Maximum (actual)
    m_min = 0.0; // Minimum (actual)
    setStepsScale(10); // Amount of main steps on scale
    setStepsSemiScale(5); // Amount of semi-steps on scale
    // Scale
    m_maximum = m_max; // Scale maximum
    setMaximumOutputValue(m_maximum); // Maximum value to show as current
    setMinimum(m_min); // Scale minimum
    setNominal(40.0); // Nominal start value
    setCritical(80.0); // Critical start value
    calcMaxMin(); // Extend max and min to have nice step values
    // Current
    setValue(42.0); // Current value
    // Miscellaneous
    setValueOffset(-100.0); // Current value downshift
    setDigitOffset(105.0); // Scale values radius
    setSuffix(QString(" [bar]")); // Measures
    setNameMeterOffset(20.0); // Device name downshift (now commented)
    setDigitFont(QFont("RobotoBold", 12, QFont::Normal));
    setValueFont(QFont("RobotoBold", 20, QFont::Normal));
    // QWidget
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setWindowTitle(tr("Analog Barmeter"));
    resize(311, 311);
    assert(m_max - m_min != 0);
}

void ManoMeter::setValue(double val)
{
    if(val < m_minimum)
        val = m_minimum;
    else if(val > m_maxOutputVal)
        val = m_maxOutputVal;
    AbstractMeter::setValue(val);
}

void ManoMeter::setValue(int val)
{
    if(val < m_minimum)
        val = m_minimum;
    else if (val > m_maxOutputVal)
        val = m_maxOutputVal;
    if ( m_value != val)
        AbstractMeter::setValue(val);
}

void ManoMeter::initCoordinateSystem(QPainter& painter)
{
    int side = qMin(width(), height());
    // painter initialization
    painter.setRenderHint(QPainter::Antialiasing);
    painter.translate(width() / 2, height() / 2);
    painter.scale(side / 335.0, side / 335.0);
}

void ManoMeter::paintBackground(QPainter& painter)
{
    // Prepare background
//    painter.setBrush(QColor(63, 0, 0));
//    painter.drawRect(-1, -1, width() + 2, height() + 2);
    // Without borders
    painter.setPen(Qt::NoPen);
    // Centering
    initCoordinateSystem(painter);

    /*** INITIALIZING ***/
    QColor colorBack(63, 31, 0, 63); // Obvious background (before nominal) color
    QColor colorHalo(0x81D4F9); // Halo color O:)
    QColor colorMarks(0, 0, 0); // Marks' on scale color
//    QColor colorNominal(3, 169, 244); // Nominal color
//    QColor colorCritical(255, 127, 127); // Critical color
    QColor colorNominal(0, 95, 191); // Nominal color
    QColor colorCritical(191, 0, 0); // Critical color
    QColor colorErase(255, 255, 255); // Technical color for removing unwanted parts
    QRadialGradient gradHalo(QPointF(0.0, 0.0), 162.0, QPointF(0.0, 0.0));
    gradHalo.setColorAt(0.0, colorHalo);
    gradHalo.setColorAt(0.8951, colorHalo);
    gradHalo.setColorAt(1.0, QColor(0x81, 0xD4, 0xF9, 0));
    // Shield gradient (making no color to bottom part of middle shield)
    QConicalGradient gradShieldMiddle(QPointF(0.0, 0.0), -90.0);
    gradShieldMiddle.setColorAt(0.0, QColor(0, 0, 0, 0));
    gradShieldMiddle.setColorAt(0.1, QColor(0, 0, 0, 0));
    gradShieldMiddle.setColorAt(0.5, QColor(63, 31, 0, 191));
    gradShieldMiddle.setColorAt(0.9, QColor(0, 0, 0, 0));
    gradShieldMiddle.setColorAt(1.0, QColor(0, 0, 0, 0));
    // Inner border gradient color (thin brown border in the middle [as complex as I could =)])
    QRadialGradient gradInnerBorder(QPointF(0.0, 0.0), 70.0, QPointF(0.0, 0.0));
    gradInnerBorder.setColorAt(0.0, QColor(0, 0, 0, 0));
    gradInnerBorder.setColorAt(0.98, QColor(0, 0, 0, 0));
    gradInnerBorder.setColorAt(1.0, QColor(63, 31, 0, 255));
    // Outter border gradient (almost all white places in the device)
    QRadialGradient gradOutterBorder(QPointF(0.0, 0.0), 162.0, QPointF(0.0, 0.0));
    gradOutterBorder.setColorAt(0.0, QColor(63, 31, 0, 255));
    gradOutterBorder.setColorAt(0.1543, QColor(63, 31, 0, 127));
    gradOutterBorder.setColorAt(0.2469, QColor(255, 255, 255, 255));
    gradOutterBorder.setColorAt(0.3396, QColor(255, 255, 255, 255));
    gradOutterBorder.setColorAt(0.4321, QColor(255, 255, 255, 0));
    gradOutterBorder.setColorAt(0.5247, QColor(255, 255, 255, 0));
    gradOutterBorder.setColorAt(0.7840, QColor(255, 255, 255, 127));
    gradOutterBorder.setColorAt(0.8580, QColor(255, 255, 255, 0));
    gradOutterBorder.setColorAt(0.8951, QColor(255, 255, 255, 0));
    gradOutterBorder.setColorAt(1.0, QColor(255, 255, 255, 0));

    /*** DROWING ***/
    // Outter halo
    painter.setBrush(gradHalo);
    painter.drawPie(-162, -162, 324, 324, -30 * 16, 240 * 16);
    // Scale bar
    painter.setBrush(colorMarks);
    painter.drawPie(-145, -145, 290, 290, -30 * 16, 240 * 16);
    // Inner background
    painter.setBrush(colorErase);
    painter.drawPie(-139, -139, 278, 278, -30 * 16, 240 * 16);
    painter.setBrush(colorBack);
    painter.drawPie(-139, -139, 278, 278, -30 * 16, 240 * 16);

    painter.save();
    painter.rotate(-210.0);
    // Nominal background
    assert(m_max - m_min != 0);
    int angleStart = static_cast<int>((240 * (m_nominal - m_min)) / (m_max - m_min));
    int angleEnd = static_cast<int>((240 * (m_critical - m_min)) / (m_max - m_min));
    painter.setBrush(colorNominal);
    if (m_min <= m_nominal && m_nominal < m_max)
        painter.drawPie(QRect(-139, -139, 278, 278), -angleStart % 360 * 16, -(angleEnd % 360 - angleStart % 360) * 16);
    // Critical backgound
    painter.setBrush(colorCritical);
    angleStart = static_cast<int>((240 * (m_critical - m_min)) / (m_max - m_min));
    angleEnd = 240;
    if (m_min <= m_critical && m_critical < m_max)
        painter.drawPie(QRect(-139, -139, 278, 278), -angleStart % 360 * 16, -(angleEnd % 360 - angleStart % 360) * 16);
    // 2nd Critical background (optional)
    if (is2redSector())
    {
        angleStart = 0;
        angleEnd = static_cast<int>((240 * (m_nominal - m_min)) / (m_max - m_min));
        if (m_nominal <= m_max && m_nominal <= m_critical)
            painter.drawPie(QRect(-139, -139, 278, 278), -angleStart % 360 * 16, -(angleEnd % 360 - angleStart % 360) * 16);
    }
    painter.restore();

    // Erase middle
    painter.setBrush(colorErase);
    painter.drawEllipse(-70, -70, 140, 140);
    // Shield middle
    painter.setBrush(gradShieldMiddle);
    painter.drawEllipse(-85, -85, 170, 170);
    // Inner border
    painter.setBrush(gradInnerBorder);
    painter.drawPie(-70, -70, 140, 140, -30 * 16, 240 * 16);
    // Erase center
    painter.setBrush(colorErase);
    painter.drawEllipse(-55, -55, 110, 110);
    // Draw nice white places
    painter.setBrush(QBrush(gradOutterBorder));
    painter.drawEllipse(-163, -163, 326, 326);
    // Erase center (for arrow)
    painter.setBrush(colorErase);
    painter.drawEllipse(-25, -25, 50, 50);

    /*** Scale values ***/
    painter.rotate(60.0);
    if (true || digitOffset())
    {
        painter.setPen(QColor(0, 0, 0, 127));
        painter.rotate(-60.0);
        painter.setFont(digitFont());
        double v = 0.0;
        for (int i = 0; i < m_stepsScale + 1; i++)
        {
            v = m_min + i * (m_max - m_min) / m_stepsScale;
            if (fabs(v) < 0.000001 ) v = 0.0;
            QString val = QString("%1").arg(v);
            QSize Size = painter.fontMetrics().size(Qt::TextSingleLine, val);
            painter.save();
            painter.translate(digitOffset() * cos(PI / 3.0 * (2.5 + 4.0 * i / m_stepsScale)),
                              digitOffset() * sin(PI / 3.0 * (2.5 + 4.0 * i / m_stepsScale)));
            painter.drawText(QPointF(Size.width() / -2.0,  Size.height() / 4.0), val);
            painter.restore();
        }
        setMaximumOutputValue(v);
    }
    // Device name
//    painter.setPen(QColor(0, 0, 0, 127));
//    painter.setFont(nameMeterFont());
//    QString outputStr = nameMeter();
//    QSize sizeStr = painter.fontMetrics().size(Qt::TextSingleLine, outputStr);
//    painter.drawText(QPointF(sizeStr.width() / -2.0, static_cast<int>(0 - valueOffset() - nameMeterOffset())), outputStr);
} // paintBackground

void ManoMeter::paintEvent(QPaintEvent * )
{
    drawBackground();
    QPainter painter(this);
    painter.setPen(Qt::NoPen);
    initCoordinateSystem(painter);
    /*** COLORS INITIALIZING ***/
    QColor colorMarks(0, 0, 0); // Marks' on scale color
    QColor colorLow(63, 31, 0); // Low color)
    QColor colorNominal(0, 95, 191); // Nominal color
    QColor colorCritical(191, 0, 0); // Critical color
    // Arrow gradient color (and central circle too)
    QConicalGradient gradArrow(QPointF(0.0, 124.0), 90.0);
    gradArrow.setColorAt(0.0, QColor(255, 255, 255, 191));
    gradArrow.setColorAt(0.01, QColor(63, 31, 0, 191));
    gradArrow.setColorAt(0.02, QColor(255, 255, 255, 0));
    gradArrow.setColorAt(0.98, QColor(255, 255, 255, 0));
    gradArrow.setColorAt(0.99, QColor(63, 31, 0, 191));
    gradArrow.setColorAt(1.0, QColor(255, 255, 255, 191));
    // Marks' gradient color (for unreached marks)
    QRadialGradient gradMarks(QPointF(0.0, 0.0), 141.0, QPointF(0.0, 0.0));
    gradMarks.setColorAt(0.0, QColor(0, 0, 0, 0));
    gradMarks.setColorAt(0.8865, QColor(0, 0, 0, 0));
    gradMarks.setColorAt(1.0, QColor(0, 0, 0, 255));
    // Marks' gradient color (for reached marks)
    QRadialGradient gradMarksPassed(QPointF(0.0, 0.0), 141.0, QPointF(0.0, 0.0));
    gradMarksPassed.setColorAt(0.0, QColor(0, 0, 0, 0));
    gradMarksPassed.setColorAt(0.8298, QColor(0, 0, 0, 0));
    gradMarksPassed.setColorAt(0.8865, QColor(191, 0, 0, 255));
    gradMarksPassed.setColorAt(1.0, QColor(0, 0, 0, 255));
    // Arrow path
    QPainterPath handPath;
    handPath.moveTo(-1.0, 120.0);
    handPath.lineTo(-10.0, -17.3205);
    handPath.quadTo(0.0, -25.0, 10.0, -17.3205);
    handPath.lineTo(1.0, 120.0);


    /*** SCALE SEGMENTS DRAWING ***/
    painter.save();
    painter.rotate(60.0);
    // First mark should be half-wide
    painter.setBrush(gradMarksPassed);
    int lineLength = 16;
    painter.drawRect(-4, 117, 4, lineLength + 8);
    // Border
    painter.setBrush(colorMarks);
    painter.drawRect(-1, 70, 1, 73);
    //
    qreal angleSemiStep = 240.0 / m_stepsScale / m_stepsSemiScale;
    qreal valueSemiStep = (m_max - m_min) / m_stepsScale / m_stepsSemiScale;
    painter.rotate(angleSemiStep);
    for (int i = 1; i < m_stepsScale * m_stepsSemiScale; i++)
    {
        if (i % m_stepsSemiScale)
            lineLength = 8;
        else
            lineLength = 16;
        if(m_value - m_min >= valueSemiStep * i)
        {
            painter.setBrush(gradMarksPassed);
            painter.drawRect(-3, 117, 7, lineLength + 8);
        }
        else
        {
            painter.setBrush(gradMarks);
            painter.drawRect(-3, 125, 7, lineLength);
        }
        painter.rotate(angleSemiStep);
    }
    lineLength = 16;
    // Last mark should be half-wide
    if(m_value >= m_max)
    {
        painter.setBrush(gradMarksPassed);
        painter.drawRect(0, 117, 4, lineLength + 8);
    }
    else
    {
        painter.setBrush(gradMarks);
        painter.drawRect(0, 125, 4, lineLength);
    }
    // Border
    painter.setBrush(colorMarks);
    painter.drawRect(0, 70, 1, 73);
    //
    painter.restore();

    /*** ARROW DRAWING ***/
    painter.save();
    painter.rotate(60.0);
    painter.setBrush(gradArrow);
    painter.rotate(((value() - m_min) * 240.0) / static_cast<double> (m_max - m_min));
    painter.drawPath(handPath);
    painter.drawEllipse(-15.0, -15.0, 30.0, 30.0);
    painter.restore(); // Restore from rotating on 60 degrees
    // Actual value of device
    if (valueOffset())
    {
        painter.setPen(colorLow);
        if(value() >= critical())
            painter.setPen(colorCritical);
        else if(value() < critical() && value() >= nominal())
            painter.setPen(colorNominal);
        else if(is2redSector())
            painter.setPen(colorCritical);
        painter.setFont(valueFont());
        QString Str = prefix() + QString("%1").arg(value()) + suffix();
        QSize Size = painter.fontMetrics().size(Qt::TextSingleLine, Str);
        painter.drawText(QPointF(Size.width() / -2.0, static_cast<int>(0 - valueOffset())), Str);
    }
} // paintEvent

void ManoMeter::contexMenuPrefix(QMouseEvent *event)
{
    QMenu menu(this);
    menu.addAction(" [atm]");
    menu.addAction(" [bar]");
    menu.addAction(" [Pa]");
    menu.connect(&menu, SIGNAL(triggered(QAction*)), this, SLOT(setSuffixOnMenuAction(QAction*)));
    menu.exec(event->globalPos());
}

