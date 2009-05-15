////////////////////////////////////////
//  File      : group.cpp             //
//  Written by: g_cigala@virgilio.it  //
//  Copyright : GPL                   //
////////////////////////////////////////

#include "group.h"

////////////////////////////////////////

Group::Group(Extension *ex, QWidget * parent) : QGroupBox(parent)
{
    ext = ex;
    setWindowTitle(ext->get_id());
    setWindowIcon(QIcon(":/icons/osso.png"));
    init();
}

Group::~Group()
{
    delete layout;
    delete ext;
}

void Group::init()
{
    layout = new QGridLayout();
    setLayout(layout);
    layout->setAlignment(Qt::AlignCenter);
    setTitle(ext->get_id());
}

QGridLayout * Group::get_layout()
{
    return layout;
}

QStringList Group::get_flags() const
{
    return ext->get_flags();
}
