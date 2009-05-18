////////////////////////////////////////
//  File      : mixer.cpp             //
//  Written by: g_cigala@virgilio.it  //
//  Copyright : GPL                   //
////////////////////////////////////////

#ifndef MIXER_H
#define MIXER_H

#include "defs.h"
#include "device.h"
#include "slider.h"
#include "group.h"
#include "peak.h"
#include "mute.h"
#include "onoff.h"
#include "enum.h"
#include "info.h"
#include "extension.h"
#include "master.h"

////////////////////////////////////////

class Mixer : public QMainWindow
{
    Q_OBJECT

public:
    Mixer(QWidget * parent = 0);
    ~Mixer();
    void init();
    void create_docks();
    void create_menu();
    void create_systray_actions();
    void create_systray_icon();
    void find_master();
    void set_childs();
    void create_controls();
    void merge_childs();

private slots:
    void closeEvent(QCloseEvent *);
    void icon_activated(QSystemTrayIcon::ActivationReason);
    void show_about();

private:
    Device *dev;
    QGroupBox *master_group;
    QFrame *sections_frame;
    QHBoxLayout *master_group_layout;
    QVBoxLayout *sections_frame_layout;
    QHash <int, QWidget *> control_list;
    QHash <int, Extension *> extension_list;
    QAction *minimizeAction;
    QAction *restoreAction;
    QAction *quitAction;
    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;
    QDockWidget *sections_dock;
    QDockWidget *info_dock;
    Info *info_dlg;
    Master *master_vol;
};

#endif
