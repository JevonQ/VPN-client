#include <QtGui>
#include "systray.h"
#include "statuswindow.h"
#include "settings.h"
#include "login.h"
#include "about.h"
#include "location.h"
#include "logdialog.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QApplication::setQuitOnLastWindowClosed(false);

    About *a = new About;
    Settings *s = new Settings;
    LoginDlg *d = new LoginDlg;
    Location *l = new Location;
    LogDialog *o = new LogDialog;
    StatusWindow *mainWindow = new StatusWindow(s, d, l, o);
    Systray *tray = new Systray(mainWindow, a, s, d, l, o);
    (void) tray;
    return app.exec();
}
