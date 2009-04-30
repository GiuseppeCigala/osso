////////////////////////////////////////
//  File      : push.h                //
//  Written by: g_cigala@virgilio.it  //
//  Copyright : GPL                   //
////////////////////////////////////////

#ifndef PUSH_H
#define PUSH_H

#include "defs.h"

///////////////////////////////////////

class Push : public QWidget
{
    Q_OBJECT
 
    QFont font() const
    {
        return fontText;
    }
    QColor colorText() const
    {
        return colText;
    }
    QString text() const
    {
        return textBut;
    }

public:
    Push(QWidget *parent = 0);
    ~Push();
    QSize sizeHint() const;
    QSize minimumSizeHint() const;

signals:
    void pressed();
    void released();

public slots:
    void setText(const QString &);
    void setFont(const QFont &);
    void setColorText(const QColor &);

protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void init();
    void paintBorder();
    void paintButton();
    void paintText();

private:
    bool pushed;
    QString textBut;
    QString family;
    QColor colText;
    QFont fontText;
};

#endif
