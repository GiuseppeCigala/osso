////////////////////////////////////////
//  File      : slider.h              //
//  Written by: g_cigala@virgilio.it  //
//  Copyright : GPL                   //
////////////////////////////////////////

#ifndef SLIDER_H
#define SLIDER_H

#include "defs.h"
#include "extension.h"
#include "cursor.h"
#include "jack.h"

///////////////////////////////////////

class Slider : public QFrame
{
    Q_OBJECT

public:
    Slider(Extension *, QString type, QWidget * parent = 0);
    ~Slider();
    void init();

public slots:
    void set_mono_volume(int);
    void set_left_volume(int);
    void set_right_volume(int);
    void update_label();
    void update_cursor();

private:
    Extension *ext;
    QString type;
    QLabel *id_lab;
    Cursor *mono_cursor;
    Cursor *left_cursor;
    Cursor *right_cursor;
};
#endif





