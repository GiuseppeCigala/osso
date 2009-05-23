////////////////////////////////////////
//  File      : mixer.cpp             //
//  Written by: g_cigala@virgilio.it  //
//  Copyright : GPL                   //
////////////////////////////////////////

#include "mixer.h"
#include "info.h"

////////////////////////////////////////

Mixer::Mixer(QWidget * parent) : QMainWindow(parent)
{
    setWindowTitle(tr("Osso - OSS 4.1 Mixer"));
    setWindowIcon(QIcon(":/icons/osso.png"));
    init();
    set_childs();
    create_controls();
    create_docks();
    create_menu();
    merge_childs();
    create_systray_actions();
    create_systray_icon();
    find_master();
    trayIcon->show();
}

Mixer::~Mixer()
{
    delete info_dlg;
}

void Mixer::init()
{
    master_group = new QGroupBox(tr("Master"), this);
    master_group_layout = new QHBoxLayout();
    master_group->setLayout(master_group_layout);
    master_group->setAlignment(Qt::AlignCenter);
    setCentralWidget(master_group);

    // open the mixer device and get the extensions
    dev = new Device(this);
    extension_list = dev->get_extensions();
    // to get system information
    info_dlg = new Info(dev);
}

void Mixer::create_menu()
{
    QMenu *view_menu = menuBar()->addMenu(tr("View"));
    QAction *about_act = menuBar()->addAction(tr("About"));
    connect(about_act, SIGNAL(triggered()), this, SLOT(show_about()));
    view_menu->addAction(sections_dock->toggleViewAction());
    view_menu->addAction(info_dock->toggleViewAction());
}

void Mixer::create_docks()
{
    sections_dock = new QDockWidget(tr("Sections"), this);
    sections_dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    sections_frame = new QFrame(this);
    sections_frame_layout = new QVBoxLayout();
    sections_frame->setLayout(sections_frame_layout);
    sections_dock->setWidget(sections_frame);

    info_dock = new QDockWidget(tr("Info"), this);
    info_dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    QFrame *info_frame = new QFrame(this);
    QVBoxLayout *info_frame_layout = new QVBoxLayout();
    info_frame->setLayout(info_frame_layout);
    info_dock->setWidget(info_frame);
    QPushButton *info_but = new QPushButton(this);
    info_but->setIcon(QIcon(":/icons/card.png"));
    info_but->setText(tr("Info"));
    connect(info_but, SIGNAL(clicked()), info_dlg, SLOT(show()));
    info_frame_layout->addWidget(info_but);

    addDockWidget(Qt::RightDockWidgetArea, sections_dock);
    addDockWidget(Qt::RightDockWidgetArea, info_dock);
}

/// find and create the master slider volume ///
void Mixer::find_master()
{
    foreach(Extension *ex, extension_list)
    {
        if (ex->get_id() == "vmix0-outvol") // hoping that all mixers have it .....
        {
            Slider *master = new Slider(ex, "mono");
             // create the master volume
            master_vol = new Master(master, this);
        }
    }
}

/// check childs for respective parent ///
void Mixer::set_childs()
{
    foreach(Extension *child, extension_list)
    {
        Extension *ex;

        if ((ex = extension_list.value(child->get_parent())) != NULL)
        {
            qDebug() << "Extension:" << ex->get_ctrl();
            qDebug() << "Child:" << child->get_ctrl();
            ex->set_child(child->get_ctrl());
        }
    }

    foreach(Extension *ex, extension_list) // print extension info
    {
        qDebug() << "<b>Extension:</b>" << ex->get_ctrl() << "\nParent:" << ex->get_parent() << "Childs:" << ex->get_childs() <<
        "Id:" << ex->get_id() << "Extname:" << ex->get_extname() << "Min:" << ex->get_min_value() << "Max:" << ex->get_max_value() <<
        "Act:" << ex->get_act_value() << "Flags:" << ex->get_flags() << "Type:" << ex->get_type();
    }
}

/// create the mixer controls ///
void Mixer::create_controls()
{
    foreach(Extension *ex, extension_list)
    {
        if (ex->get_type() == "MIXT_MONOSLIDER16")
        {
            Slider *sld = new Slider(ex, "mono");
            control_list.insert(ex->get_ctrl(), sld);
            qDebug() << "Add ctrl:" << ex->get_ctrl() << "Childs:" << ex->get_childs();
        }

        if (ex->get_type() == "MIXT_STEREOSLIDER16")
        {
            Slider *sld = new Slider(ex, "stereo");
            control_list.insert(ex->get_ctrl(), sld);
            qDebug() << "Add ctrl:" << ex->get_ctrl() << "Childs:" << ex->get_childs();
        }

        if (ex->get_type() == "MIXT_GROUP")
        {
            Group *grp = new Group(ex);
            control_list.insert(ex->get_ctrl(), grp);
            qDebug() << "Add ctrl:" << ex->get_ctrl() << "Childs:" << ex->get_childs();
        }

        if (ex->get_type() == "MIXT_MONOPEAK")
        {
            Peak *mono_vu = new Peak(ex, "mono");
            control_list.insert(ex->get_ctrl(), mono_vu);
            qDebug() << "Add ctrl:" << ex->get_ctrl() << "Childs:" << ex->get_childs();
        }

        if (ex->get_type() == "MIXT_STEREOPEAK")
        {
            Peak *stereo_vu = new Peak(ex, "stereo");
            control_list.insert(ex->get_ctrl(), stereo_vu);
            qDebug() << "Add ctrl:" << ex->get_ctrl() << "Childs:" << ex->get_childs();
        }

        if (ex->get_type() == "MIXT_MUTE")
        {
            Mute *mute = new Mute(ex);
            control_list.insert(ex->get_ctrl(), mute);
            qDebug() << "Add ctrl:" << ex->get_ctrl() << "Childs:" << ex->get_childs();
        }

        if (ex->get_type() == "MIXT_ONOFF")
        {
            Onoff *onoff = new Onoff(ex);
            control_list.insert(ex->get_ctrl(), onoff);
            qDebug() << "Add ctrl:" << ex->get_ctrl() << "Childs:" << ex->get_childs();
        }

        if (ex->get_type() == "MIXT_ENUM")
        {
            Enum *en = new Enum(ex);
            control_list.insert(ex->get_ctrl(), en);
            qDebug() << "Add ctrl:" << ex->get_ctrl() << "Childs:" << ex->get_childs();
        }
    }
}

/// merge the childs to respective parent ///
void Mixer::merge_childs()
{
    QHashIterator<int, QWidget *> iter(control_list);

    while (iter.hasNext())
    {
        iter.next();

        QList <int> childs = extension_list.value(iter.key())->get_childs();
        int parent = extension_list.value(iter.key())->get_parent();

        if (childs.size() != 0 && parent == 0 && extension_list.value(iter.key())->get_type() == "MIXT_GROUP") // is top level group box
        {
            qDebug() << "Top Level Group:" << iter.key();

            Group *parent = dynamic_cast <Group *>(iter.value());
            QGridLayout *group_layout = parent->get_layout();
            QPushButton *section_but = new QPushButton(this);
            section_but->setIcon(QIcon(":/icons/audio.png"));
            section_but->setText(extension_list.value(iter.key())->get_id());
            connect(section_but, SIGNAL(clicked()), parent, SLOT(show()));
            sections_frame_layout->addWidget(section_but);

            for (int i = 0; i < childs.size(); ++i)
            {
                group_layout->addWidget(control_list.value(childs.at(i)), i/8, i%8);
                qDebug() << "Child Control:" << childs.at(i) << "add to Group:" << iter.key();
            }
        }
        if (childs.size() != 0 && parent != 0 && extension_list.value(iter.key())->get_type() == "MIXT_GROUP") // is a nested group box
        {
            qDebug() << "Child Group:" << iter.key();

            Group *parent = dynamic_cast <Group *>(iter.value());
            QGridLayout *group_layout = parent->get_layout();

            for (int i = 0; i < childs.size(); ++i)
            {
                group_layout->addWidget(control_list.value(childs.at(i)), i/4, i%4);
                qDebug() << "Child Control:" << childs.at(i) << "add to Group:" << iter.key();
            }
        }
        if (childs.size() == 0 && parent == 0
                && extension_list.value(iter.key())->get_type() != "MIXT_GROUP") // is a top level control (bypass the empty group)
        {
            qDebug() << "Top Level Control:" << iter.key();
            master_group_layout->addWidget(iter.value(), Qt::AlignCenter);
        }
    }
}

void Mixer::create_systray_icon()
{
    trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(minimizeAction);
    trayIconMenu->addAction(restoreAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(quitAction);

    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setContextMenu(trayIconMenu);
    trayIcon->setIcon(QIcon(":/icons/osso.png"));
    trayIcon->setToolTip(tr("Osso - OSS 4.1 Mixer"));
    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(icon_activated(QSystemTrayIcon::ActivationReason)));
}

void Mixer::create_systray_actions()
{
    minimizeAction = new QAction(QIcon(":/icons/minimize.png"), tr("Minimize"), this);
    connect(minimizeAction, SIGNAL(triggered()), this, SLOT(hide()));

    restoreAction = new QAction(QIcon(":/icons/restore.png"), tr("Show"), this);
    connect(restoreAction, SIGNAL(triggered()), this, SLOT(showNormal()));

    quitAction = new QAction(QIcon(":/icons/quit.png"), tr("Quit"), this);
    connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
}

void Mixer::closeEvent(QCloseEvent *event)
{
    if (trayIcon->isVisible())
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle(tr("System Tray"));
        msgBox.setWindowIcon(QIcon(":/icons/osso.png"));
        QPushButton *ok_but = new QPushButton(QIcon(":/icons/ok.png"), tr("Ok"), this);
        QPushButton *cancel_but = new QPushButton(QIcon(":/icons/cancel.png"), tr("Cancel"), this);
        msgBox.addButton(ok_but, QMessageBox::AcceptRole);
        msgBox.addButton(cancel_but, QMessageBox::RejectRole);
        msgBox.setText(tr("Do you want quit from <b>Osso</b>?"));
        msgBox.setIconPixmap(QPixmap(":/icons/question.png"));

        int ret = msgBox.exec();

        if (ret == QMessageBox::AcceptRole) // ok
        {
            qApp->quit();
        }
        else // cancel
        {
            hide();
            event->ignore();
        }
    }
}

void Mixer::icon_activated(QSystemTrayIcon::ActivationReason reason)
{
    if (reason == QSystemTrayIcon::Trigger) // single click
    {
        QPoint mouse_pos = QCursor::pos();
        mouse_pos.setY(mouse_pos.y() - master_vol->sizeHint().height());
        master_vol->move(mouse_pos); // move above the mouse cursor
        master_vol->show();
    }
}

void Mixer::show_about()
{
    QMessageBox msgBox;
    msgBox.setWindowTitle(tr("About Osso"));
    msgBox.setWindowIcon(QIcon(":/icons/osso.png"));
    msgBox.setIconPixmap(QPixmap(":/icons/osso.png"));
    msgBox.setText(tr("<b>Osso - OSS 4.1 Mixer</b>"));
    msgBox.setInformativeText(tr("Version 0.3"));
    msgBox.setDetailedText(tr("Author: Giuseppe Cigala ").append("\nContact: g_cigala@virgilio.it"));
    //msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.exec();
}
