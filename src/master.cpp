////////////////////////////////////////
//  File      : master.cpp            //
//  Written by: g_cigala@virgilio.it  //
//  Copyright : GPL                   //
////////////////////////////////////////

#include "master.h"

////////////////////////////////////////

Master::Master(Extension *ex, QWidget *mx, QWidget * parent) : QWidget(parent)
{
    ext = ex;
    mix = mx;
    setWindowFlags(Qt::Popup);
    init();
}

Master::~Master()
{
}

void Master::init()
{
    QVBoxLayout *master_group_layout = new QVBoxLayout();
    master_group_layout->setMargin(5);
    setLayout(master_group_layout);
    Slider *mono_cursor = new Slider(ext, "mono");
    QPushButton *full_but = new QPushButton(tr("Full view"), this);
    master_group_layout->addWidget(mono_cursor, Qt::AlignCenter);
    master_group_layout->addWidget(full_but, Qt::AlignCenter);
    connect(mono_cursor, SIGNAL(valueChanged(int)), this, SLOT(set_mono_volume(int)));
    connect(full_but, SIGNAL(clicked()), this, SLOT(show_full()));
}

void Master::set_mono_volume(int volume)
{
    ext->set_mono_value(volume);
}

// show the full mixer
void Master::show_full()
{
    mix->showNormal();
}