////////////////////////////////////////
//  File      : info.cpp              //
//  Written by: g_cigala@virgilio.it  //
//  Copyright : GPL                   //
////////////////////////////////////////

#include "info.h"

////////////////////////////////////////

Info::Info(Device *d, QWidget * parent) : QGroupBox(parent)
{
    dev = d;
    setWindowTitle(tr("Info"));
    setWindowIcon(QIcon(":/icons/card.png"));
    init();
}

Info::~Info()
{
    delete dev;
}

void Info::init()
{
    QGridLayout *layout = new QGridLayout();
    layout->setColumnMinimumWidth(1, 200),
    setLayout(layout);
    QMap <QString, QString> sys_info = dev->get_system_info();
    QMapIterator <QString, QString> iter(sys_info);

    int i = 0;

    while (iter.hasNext())
    {
        iter.next();

        QLabel *lab = new QLabel(this);
        lab->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
        QLineEdit *info = new QLineEdit(this);
        info->setReadOnly(true);

        lab->setText(iter.key());
        info->setText(iter.value());
        layout->addWidget(lab, i, 0);
        layout->addWidget(info, i, 1);
        i++;
    }
}

