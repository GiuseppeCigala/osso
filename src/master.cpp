////////////////////////////////////////
//  File      : master.cpp            //
//  Written by: g_cigala@virgilio.it  //
//  Copyright : GPL                   //
////////////////////////////////////////

#include "master.h"

////////////////////////////////////////

Master::Master(Slider *slid, QWidget *mx, QWidget * parent) : QWidget(parent)
{
    master_slider = slid; // is the master volume slider
    mix = mx;
    setWindowFlags(Qt::Popup);
    init();
}

Master::~Master()
{
    delete master_slider;
    delete mix;
}

void Master::init()
{
    QVBoxLayout *master_group_layout = new QVBoxLayout();
    master_group_layout->setMargin(5);
    setLayout(master_group_layout);
    QPushButton *full_but = new QPushButton(tr("Full view"), this);
    master_group_layout->addWidget(master_slider, Qt::AlignCenter);
    master_group_layout->addWidget(full_but, Qt::AlignCenter);
    connect(full_but, SIGNAL(clicked()), this, SLOT(show_full()));
}

void Master::set_volume(int volume)
{
    master_slider->set_mono_volume(volume);
}

// show the full mixer
void Master::show_full()
{
    mix->showNormal();
}
