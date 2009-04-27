////////////////////////////////////////
//  File      : jack.cpp              //
//  Written by: g_cigala@virgilio.it  //
//  Copyright : GPL                   //
////////////////////////////////////////

#include "jack.h"

////////////////////////////////////////

Jack::Jack(QWidget *parent) : QWidget(parent)
{
    init();
    setFixedSize(20, 20);
}

Jack::~Jack()
{}

void Jack::init()
{
    col = Qt::darkGray;
}

void Jack::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setWindow(0, 0, 100, 100);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(QPen(Qt::black, 3));
    QRect rect_border(0, 0, 100, 100);
    painter.drawRoundRect(rect_border, 25, 25);
    QLinearGradient linGrad(0, 75, 50, 25);
    linGrad.setColorAt(0, Qt::gray);
    linGrad.setColorAt(1, QColor(200, 200, 200));
    linGrad.setSpread(QGradient::ReflectSpread);
    painter.setBrush(linGrad);
    QRect spread(2, 2, 96, 96);
    painter.drawRoundRect(spread, 25, 25);
    
    painter.setPen(QPen(Qt::black, 2));
    QRect out_border(12, 12, 76, 76);
    painter.drawEllipse(out_border);
    painter.setPen(QPen(Qt::white, 2));
    QRect in_border(30, 30, 40, 40);
    painter.drawEllipse(in_border);
    
    painter.setBrush(col);
    QRect border(15, 15, 70, 70);
    painter.drawEllipse(border);
    painter.setBrush(QColor(125, 125, 125));
    QRect pin(32, 32, 36, 36);
    painter.drawEllipse(pin);
}

void Jack::setColor(QColor color)
{
    col = color;
    update();
}

QSize Jack::sizeHint() const
{
    return QSize(20, 20);
}

QSize Jack::minimumSizeHint() const
{
    return QSize(10, 10);
}

