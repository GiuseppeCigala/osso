////////////////////////////////////////
//  File      : master.h              //
//  Written by: g_cigala@virgilio.it  //
//  Copyright : GPL                   //
////////////////////////////////////////

#ifndef MASTER_H
#define MASTER_H

#include "defs.h"
#include "extension.h"
#include "slider.h"


///////////////////////////////////////

class Master : public QWidget
{
    Q_OBJECT

public:
    Master(Extension *, QWidget *, QWidget * parent = 0);
    ~Master();
    void init();


private slots:
    void set_mono_volume(int);
    void show_full();

private:
    Extension *ext;
    QWidget *mix;
};
#endif





