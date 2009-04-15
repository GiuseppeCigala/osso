////////////////////////////////////////
//  File      : main.cpp              //
//  Written by: g_cigala@virgilio.it  //
//  Copyright : GPL                   //
////////////////////////////////////////

#include "defs.h"
#include "mixer.h"

////////////////////////////////////////

int main(int argc, char ** argv)
{
    QApplication app(argc, argv);
    
    if (!QSystemTrayIcon::isSystemTrayAvailable())
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle("System Tray");
        msgBox.setWindowIcon(QIcon(":/icons/osso.png"));
        msgBox.setText("No System Tray detect on this system...");
        msgBox.setInformativeText("Osso not started...");
        msgBox.setIconPixmap(QPixmap(":/icons/quit.png"));
        msgBox.exec();
        
        return 1;
    }
    
    QApplication::setQuitOnLastWindowClosed(false);
    Mixer mix;
    app.connect(&app, SIGNAL(lastWindowClosed()), &app, SLOT(quit()));
    return app.exec();
}
