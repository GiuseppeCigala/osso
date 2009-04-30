////////////////////////////////////////
//  File      : vumeter.h             //
//  Written by: g_cigala@virgilio.it  //
//  Copyright : GPL                   //
////////////////////////////////////////

#ifndef VUMETER_H
#define VUMETER_H

#include "defs.h"

////////////////////////////////////////

class VUMeter : public QWidget
{
    Q_OBJECT

    QColor colorBg() const
    {
        return colBack;
    }
    QColor colorValue() const
    {
        return colValue;
    }
    QColor colorLow() const
    {
        return colLow;
    }
    QColor colorHigh() const
    {
        return colHigh;
    }
    int minValue() const
    {
        return min;
    }
    int maxValue() const
    {
        return max;
    }
    int actValue() const
    {
        return val;
    }
    int valueDim() const
    {
        return dimVal;
    }

public:
    VUMeter(QWidget *parent = 0);
    ~VUMeter();
    QSize sizeHint() const;
    QSize minimumSizeHint() const;

signals:

public slots:
    void setColorBg(const QColor &);
    void setColorValue(const QColor &);
    void setColorHigh(const QColor &);
    void setColorLow(const QColor &);
    void setValueDim(int);
    void setLabel(const QString &);
    void setActValue(int);
    void setMinValue(int);
    void setMaxValue(int);

protected:
    void paintEvent(QPaintEvent *);
    void paintBorder();
    void paintBar();
    void paintValue();

private:
    int min;
    int max;
    int val;
    int dimVal;
    QColor colBack;
    QColor colValue;
    QColor colHigh;
    QColor colLow;
    QString label;
};

#endif
