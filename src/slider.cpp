////////////////////////////////////////
//  File      : slider.cpp            //
//  Written by: g_cigala@virgilio.it  //
//  Copyright : GPL                   //
////////////////////////////////////////

#include "slider.h"

////////////////////////////////////////

Slider::Slider(Extension *ex, QString ty, QWidget * parent) : QFrame(parent)
{
    ext = ex;
    type = ty;
    setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    setToolTip(ext->get_id());
    init();
}

Slider::~Slider()
{
    delete ext;
}

void Slider::init()
{
    if (ext->get_id().startsWith('@')) // to update the label
    {
        timer = new QTimer(this);
        connect(timer, SIGNAL(timeout()), SLOT(update_label()));
        timer->start(2000);  // signal every 2000 ms
    }
    QGridLayout *layout = new QGridLayout();
    layout->setAlignment(Qt::AlignCenter);
    layout->setSpacing(5);
    setLayout(layout);
    id_lab = new QLabel(ext->get_id(), this);
    Jack *jack = new Jack();  // 3.5 mm jacks for audio input and outputs (pink=mic, etc)
    jack->setColor(ext->get_color());

    if (type == "mono")
    {
        Cursor *mono_cursor = new Cursor("mono", this);
        mono_cursor->setMinValue(ext->get_min_value());
        mono_cursor->setMaxValue(ext->get_max_value());
        mono_cursor->setActValue(ext->get_mono_value());
        layout->addWidget(jack, 0, 0, Qt::AlignCenter);
        layout->addWidget(id_lab, 1, 0, Qt::AlignCenter);
        layout->addWidget(mono_cursor, 2, 0, Qt::AlignCenter);
        connect(mono_cursor, SIGNAL(valueChanged(int)), this, SLOT(set_mono_volume(int)));
    }
    if (type == "stereo")
    {
        Cursor *left_cursor = new Cursor("left", this);
        Cursor *right_cursor = new Cursor("right", this);
        left_cursor->setMinValue(ext->get_min_value());
        left_cursor->setMaxValue(ext->get_max_value());
        left_cursor->setActValue(ext->get_left_value());
        right_cursor->setMinValue(ext->get_min_value());
        right_cursor->setMaxValue(ext->get_max_value());
        right_cursor->setActValue(ext->get_right_value());
        layout->addWidget(jack, 0, 0, 1, 0, Qt::AlignCenter);
        layout->addWidget(id_lab, 1, 0, 1, 0, Qt::AlignCenter);
        layout->addWidget(left_cursor, 2, 0, Qt::AlignCenter);
        layout->addWidget(right_cursor, 2, 1, Qt::AlignCenter);
        connect(left_cursor, SIGNAL(valueChanged(int)), this, SLOT(set_left_volume(int)));
        connect(right_cursor, SIGNAL(valueChanged(int)), this, SLOT(set_right_volume(int)));
    }
}

void Slider::set_mono_volume(int volume)
{
    ext->set_mono_value(volume);
}

void Slider::set_left_volume(int volume)
{
    ext->set_left_value(volume);
}

void Slider::set_right_volume(int volume)
{
    ext->set_right_value(volume);
}

void Slider::update_label()
{
    oss_audioinfo ainfo;
    ainfo.dev = QString(ext->get_id().mid(4)).toInt(); // get the audio device num. (i.e. @pcm9 ->9)

    if (ioctl (ext->get_mixer_fd(), SNDCTL_ENGINEINFO, &ainfo) == -1)
    {
        qCritical() << "SNDCTL_ENGINEINFO error.\n";
        return;
    }
    if (*ainfo.label != 0)
    {
        id_lab->setText(QString(ainfo.label)); // update label name (es. xmms, firefox)
    }
    else
    {
        id_lab->setText(ext->get_id()); // restore the label
    }
    qDebug() << "label:" << ainfo.label;
}
