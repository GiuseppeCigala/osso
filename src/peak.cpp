////////////////////////////////////////
//  File      : peak.cpp              //
//  Written by: g_cigala@virgilio.it  //
//  Copyright : GPL                   //
////////////////////////////////////////

#include "peak.h"

////////////////////////////////////////

Peak::Peak(Extension *ex, QString ty, QWidget * parent) : QFrame(parent)
{
    ext = ex;
    type = ty;
    setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    setToolTip(ext->get_id());
    init();
}

Peak::~Peak()
{
    delete ext;
}

void Peak::init()
{
    timer = new QTimer(this); // to update the value
    QGridLayout *layout = new QGridLayout();
    layout->setAlignment(Qt::AlignCenter);
    layout->setSpacing(5);
    setLayout(layout);
    QFrame *lab_frame = new QFrame(this);
    QVBoxLayout *lab_layout = new QVBoxLayout();
    lab_frame->setLayout(lab_layout);
    QLabel *id_lab = new QLabel(ext->get_id(), this);
    lab_layout->insertSpacing(0, 10);
    lab_layout->addWidget(id_lab, Qt::AlignCenter);

    if (type == "mono")
    {
        mono_vu = new VUMeter(this);
        mono_vu->setMinValue(ext->get_min_value());
        mono_vu->setMaxValue(ext->get_max_value());
        mono_vu->setLabel("M");
        layout->addWidget(lab_frame, 0, 0, Qt::AlignCenter);
        layout->addWidget(mono_vu, 1, 0, Qt::AlignCenter);
        connect(timer, SIGNAL(timeout()), SLOT(mono_timeout()));
        timer->start(50);  // signal every 50 ms
    }
    if (type == "stereo")
    {
        left_vu = new VUMeter(this);
        left_vu->setMinValue(ext->get_min_value());
        left_vu->setMaxValue(ext->get_max_value());
        right_vu = new VUMeter(this);
        right_vu->setMinValue(ext->get_min_value());
        right_vu->setMaxValue(ext->get_max_value());
        left_vu->setLabel("L");
        right_vu->setLabel("R");
        layout->addWidget(lab_frame, 0, 0, 1, 0, Qt::AlignCenter);
        layout->addWidget(left_vu, 1, 0, Qt::AlignCenter);
        layout->addWidget(right_vu, 1, 1, Qt::AlignCenter);
        connect(timer, SIGNAL(timeout()), SLOT(stereo_timeout()));
        timer->start(50);  // signal every 50 ms
    }
}

void Peak::mono_timeout()
{
    int value = ext->get_mono_value();
    
    if (value != 0) // update only if there's signal
    {
        mono_vu->setActValue(ext->get_mono_value());
    }
}

void Peak::stereo_timeout()
{
    int value = ext->get_act_value();

    if (value != 0) // update only if there's signal
    {
        left_vu->setActValue(value & 0xff);
        right_vu->setActValue((value >> 8) & 0xff);
    }
}