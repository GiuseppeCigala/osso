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
    setLayout(layout);
    QMap <QString, QStringList> sys_info = dev->get_system_info();
    QMapIterator <QString, QStringList> iter(sys_info);

    int i = 0;

    while (iter.hasNext())
    {
        iter.next();

        QLabel *lab = new QLabel(this);
        lab->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);

        if (iter.key() == "6. Audio Device files:" || iter.key() == "7. Audio Engines:" || iter.key() == "8. Sound Cards:")
        {
            lab->setText(iter.key());

            QTableWidget *info_table = new QTableWidget(this);
            info_table->setColumnCount(2);
            info_table->setRowCount(iter.value().size());
            QStringList header;
            header << tr("Name") << tr("Device");
            info_table->setHorizontalHeaderLabels(header);
            info_table->setAlternatingRowColors(true);
            info_table->setMaximumHeight(iter.value().size() * (fontMetrics().height()) + 60);

            for (int l = 0; l < iter.value().size(); ++l)
            {
                QStringList elements = iter.value().at(l).split(",");
                QTableWidgetItem *name = new QTableWidgetItem(elements.at(0));
                QTableWidgetItem *node = new QTableWidgetItem(elements.at(1));
                name->setFlags(Qt::ItemIsEnabled);
                node->setFlags(Qt::ItemIsEnabled);
                info_table->setItem(l, 0, name);
                info_table->setItem(l, 1, node);
            }
            info_table->resizeColumnsToContents();
            info_table->resizeRowsToContents();
            layout->addWidget(lab, i, 0);
            layout->addWidget(info_table, i, 1);
        }
        else
        {
            QLineEdit *info = new QLineEdit(this);
            info->setReadOnly(true);
            lab->setText(iter.key());

            for (int l = 0; l < (iter.value()).size(); ++l)
            {
                info->setText((iter.value()).at(l));
            }
            layout->addWidget(lab, i, 0);
            layout->addWidget(info, i, 1);
        }
        i++;
    }
    
    QDialogButtonBox *close_box = new QDialogButtonBox(this);
    QPushButton *close = new QPushButton(QIcon(":/icons/quit.png"), tr("Close"), this);
    close_box->addButton(close, QDialogButtonBox::AcceptRole);
    connect(close_box, SIGNAL(accepted()), this, SLOT(close()));
    layout->addWidget(close_box, i, 1);
}

