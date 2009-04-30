////////////////////////////////////////
//  File      : jack.h                //
//  Written by: g_cigala@virgilio.it  //
//  Copyright : GPL                   //
////////////////////////////////////////

#ifndef JACK_H
#define JACK_H

#include "defs.h"

///////////////////////////////////////

class Jack : public QWidget
{
    Q_OBJECT
 
public:
    Jack(QWidget *parent = 0);
    ~Jack();
    QSize sizeHint() const;
    QSize minimumSizeHint() const;

signals:

public slots:
    void setColor(const QColor &);

protected:
    void paintEvent(QPaintEvent *);
    void init();

private:
    QColor col;

};

#endif
