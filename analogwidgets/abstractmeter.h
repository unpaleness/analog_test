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

#ifndef ABSTRACTMETER_H
#define ABSTRACTMETER_H

#include "widgetwithbackground.h"

/**
   * Abstract class for Meter/Gauge classes
   */
class AbstractMeter : public WidgetWithBackground
{
    Q_OBJECT
    Q_PROPERTY (double minimum READ minimum WRITE setMinimum )
    Q_PROPERTY (double maximum READ maximum WRITE setMaximum )
    Q_PROPERTY (double value   READ value   WRITE setValue )
    Q_PROPERTY (double nominal READ nominal WRITE setNominal)
    Q_PROPERTY (double critical READ critical WRITE setCritical)

    Q_PROPERTY (QString prefix READ prefix WRITE setPrefix)
    Q_PROPERTY (QString suffix READ suffix WRITE setSuffix)
    Q_PROPERTY (QFont valueFont READ valueFont   WRITE setValueFont)
    Q_PROPERTY (double valueOffset READ valueOffset WRITE setValueOffset)
    Q_PROPERTY (QFont digitFont READ digitFont   WRITE setDigitFont)
    Q_PROPERTY (double digitOffset READ digitOffset WRITE setDigitOffset)
    Q_PROPERTY (int stepsScale READ stepsScale WRITE setStepsScale)
    Q_PROPERTY (int stepsSemiScale READ stepsSemiScale WRITE setStepsSemiScale)

    /* 25/01/2015 им€ отображаемого на виджете параметра.*/
    Q_PROPERTY (QString nameMeter READ nameMeter WRITE setNameMeter)
    Q_PROPERTY (QFont nameMeterFont READ nameMeterFont WRITE setNameMeterFont)
    Q_PROPERTY (double nameMeterOffset READ nameMeterOffset WRITE setNameMeterOffset)
    Q_PROPERTY (bool is2redSector READ is2redSector WRITE setIs2redSector)

public:
    AbstractMeter(QWidget *parent = 0);
    virtual ~AbstractMeter() {}

    double  minimum() const   { return m_minimum; }
    void setMinimum(double i);
    double  maximum() const   { return m_maximum; }
    void setMaximum(double i);
    double value() const         { return m_value;}

    double nominal() const	  { return m_nominal; 		}
    void setNominal(double i)    { m_nominal = i; updateWithBackground();}
    double critical() const	  { return m_critical;		}
    void setCritical(double i)   { m_critical = i; updateWithBackground();}

    QFont valueFont() const   { return m_valueFont;        }
    void setValueFont(QFont f){ m_valueFont = f; updateWithBackground(); }

    double valueOffset() const   { return m_valueOffset;       }
    void setValueOffset(double v){ m_valueOffset = v; updateWithBackground();}

    double digitOffset() const   { return m_digitOffset;       }
    void setDigitOffset(double v){ m_digitOffset = v; updateWithBackground();}

    QFont digitFont() const   { return m_digitFont;         }
    void setDigitFont(QFont f){ m_digitFont = f; updateWithBackground();  }

    QString prefix() const    { return m_prefix;  }
    void setPrefix(QString s) { m_prefix = s; updateWithBackground(); }

    QString suffix() const    { return m_suffix;  }
    void setSuffix(QString s) { m_suffix = s; updateWithBackground(); }

    int stepsScale() const    { return m_stepsScale; }
    void setStepsScale(int v) { m_stepsScale = v; }
    int stepsSemiScale() const    { return m_stepsSemiScale; }
    void setStepsSemiScale(int v) { m_stepsSemiScale = v; }

    /*  23/01/2015 ¬се "сеттеры" и "геттеры" описаные отсюда и до конца секции,
     *  написаны дл€ обеспечени€ отображени€ имени измер€емого параметра*/
    QString nameMeter() {return m_nameMeter;}
    void setNameMeter(QString name) {
        m_nameMeter = name;
        updateWithBackground();
    }
    QFont nameMeterFont() const {return m_nameMeterFont;}
    void setNameMeterFont(QFont f) {
        m_nameMeterFont = f;
        updateWithBackground();
    }
    double nameMeterOffset() const {return m_nameMeterOffset;}
    void setNameMeterOffset(double offset) {
        m_nameMeterOffset = offset;
        updateWithBackground();
    }
    bool is2redSector() const {return m_is2redSector;}
    void setIs2redSector(bool v) {
        m_is2redSector = v;
        updateWithBackground();
    }

public slots:
    void setValue(int val);
    void setValue(double val);

signals:
    void valueChanged(int val);
    void valueChanged(double val);

protected:
    void mousePressEvent(QMouseEvent *event);
    virtual void contexMenuPrefix(QMouseEvent *event) = 0;

    /**
     * Calculate m_max and m_min values shown on scale
     * @return true if m_max or m_min has been changed
     */

    bool calcMaxMin();

    /** Starting value on meter  this value is less than m_minimum */
    double m_min;
    /** Endgind value on meter this value is more than m_maximum */
    double m_max;

    /** Minimum that has to be on scale */
    double m_minimum;
    /** Maximum that has to be on scale */
    double m_maximum;

    /** Current value */
    double m_value;

    /** Nominal value (allowed value) by convention mark by green look to manometer and thermometer widgets */
    double m_nominal;
    /** Critical value (maximum allowed value) by convention mark by red */
    double m_critical;

    /** Used to place value string on component */
    double m_valueOffset;
    /** Used to place scale digits offset. On manometer distance from the center on thermometer distance form left */
    double m_digitOffset;

    /** Font used to display value */
    QFont m_valueFont;
    /** Font used to display scale digit/numbers */
    QFont m_digitFont;

    /** Prefix added to value string  like Speed  */
    QString m_prefix;
    /** Postfix added to value string like km/h or mph */
    QString m_suffix;

    /** Amount of main scale steps */
    int m_stepsScale;
    /** Amount of semiscale steps */
    int m_stepsSemiScale;

/*  23/01/2015 Ётот код добавил Ѕанчекно ј.ј.
 *  Ётот класс сделан дл€ отображени€ давлени€, а мы используем его дл€
 *  отображени€ различных параметров. ќтсюда возникает необходимость отображать
 *  на циферблате название измер€емой величине, причем есть дикое желание
 *  задавать это название в дизайнере, т.е. дл€ этого параметра будут прописаны
 *  еще и свойства*/
    QString m_nameMeter;
    QFont m_nameMeterFont;
    double m_nameMeterOffset;
    bool m_is2redSector;

protected slots:
    void setSuffixOnMenuAction(QAction *action);

};// AbstractMeter

#endif // ABSTRACTMETER_H_
