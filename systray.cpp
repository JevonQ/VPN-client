#include <QtGui>
#include <QDesktopServices>
#include "systray.h"

Systray::Systray(StatusWindow *w, About *a, Settings *s, LoginDlg *d, Location *l, LogDialog *o)
{
    mainWindow = w;
    mainAbout = a;
    mainSettings = s;
    mainLogin = d;
    mainLocation = l;
    mainLog = o;

    createActions();
    createTrayIcon();

    connect(trayIcon, SIGNAL(messageClicked()), this, SLOT(messageClicked()));
    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));
    connect(mainLocation, SIGNAL(settingsDialgShow()), this, SLOT(openSettingsDlg()));

    connect(mainSettings, SIGNAL(showLogDialogSignals()), this, SLOT(showLogDialogAction()));
    connect(mainLogin, SIGNAL(addLog(const QString &)), mainLog, SLOT(addLogEntry(const QString &)));
    connect(mainLocation, SIGNAL(addLog(const QString &)), mainLog, SLOT(addLogEntry(const QString &)));
    connect(mainLog, SIGNAL(addLog(const QString &)), mainLog, SLOT(addLogEntry(const QString &)));
    connect(mainWindow, SIGNAL(addLog(const QString &)), mainLog, SLOT(addLogEntry(const QString &)));
    connect(mainSettings, SIGNAL(addLog(const QString &)), mainLog, SLOT(addLogEntry(const QString &)));
    connect(this, SIGNAL(addLog(const QString &)), mainLog, SLOT(addLogEntry(const QString &)));
    connect(mainWindow, SIGNAL(openvpnStatusChangedSignal(const int &)), this, SLOT(openvpnStatusChanged(const int &)));

    connect(mainSettings, SIGNAL(showBalloon(const int &, const QString &, const QString &)), this, SLOT(showBalloonHint(const int &, const QString &, const QString &)));
    connect(mainLogin, SIGNAL(showBalloon(const int &, const QString &, const QString &)), this, SLOT(showBalloonHint(const int &, const QString &, const QString &)));
    connect(mainLocation, SIGNAL(showBalloon(const int &, const QString &, const QString &)), this, SLOT(showBalloonHint(const int &, const QString &, const QString &)));
    connect(mainLog, SIGNAL(showBalloon(const int &, const QString &, const QString &)), this, SLOT(showBalloonHint(const int &, const QString &, const QString &)));
    connect(mainWindow, SIGNAL(showBalloon(const int &, const QString &, const QString &)), this, SLOT(showBalloonHint(const int &, const QString &, const QString &)));

    trayIcon->show();
    showBalloonHint(1, "Welcome to Safejumper, by proxy.sh", "Use the tray icon to manage the software options.");
}

void Systray::openvpnStatusChanged(const int &s)
{
    if (s == VPN_CONNECTED) {
        connectAction->setEnabled(false);
        disconnectAction->setEnabled(true);
    } else if (s == VPN_DISCONNECT) {
        connectAction->setEnabled(true);
        disconnectAction->setEnabled(false);
    } else {
    }
}

void Systray::closeEvent(QCloseEvent *event)
{
    if (trayIcon->isVisible()) {
        event->ignore();
    }
}

void Systray::beginLogin()
{
    if (mainLogin->getLoginStatus()) {
        mainWindow->mainWindowShow();
        mainWindow->openvpnConnect();
    } else {
        mainLogin->openDialog();
    }
}

void Systray::setIcon(int index)
{
    (void) index;
    /*
    QIcon icon = iconComboBox->itemIcon(index);
    trayIcon->setIcon(icon);
    setWindowIcon(icon);

    trayIcon->setToolTip(iconComboBox->itemText(index));
    */
}

void Systray::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason) {
    case QSystemTrayIcon::Trigger:
    case QSystemTrayIcon::DoubleClick:
        break;
    case QSystemTrayIcon::MiddleClick:
        break;
    default:
        ;
    }
}

void Systray::showBalloonHint(const int &type, const QString &title, const QString &message)
{
    QSystemTrayIcon::MessageIcon icon;
    if (type == 1) {
        icon = QSystemTrayIcon::Information;
    } else if (type ==2) {
        icon = QSystemTrayIcon::Warning;
    } else {
        icon = QSystemTrayIcon::Critical;
    }

    trayIcon->showMessage(title, message, icon, 1 * 1000);
}

void Systray::messageClicked()
{
}

void Systray::showAbout()
{
    printf("Begin to make 'about' Dialog and show it\n");
    mainAbout->show();
}

void Systray::openBugLink()
{
    QDesktopServices::openUrl(QUrl(QLatin1String("http://blog.const.net.cn")));
}

void Systray::openMoneyLink()
{
    QDesktopServices::openUrl(QUrl(QLatin1String("https://proxy.sh/panel/affiliates.php")));
}

void Systray::openWebLink()
{
    QDesktopServices::openUrl(QUrl(QLatin1String("https://proxy.sh/panel/clientarea.php")));
}

void Systray::openFeedbackLink()
{
    QDesktopServices::openUrl(QUrl(QLatin1String("ttps://proxy.sh/panel/submitticket.php'")));
}

void Systray::openMainStatusWindow()
{
    if (mainLogin->getLoginStatus()) {
        mainWindow->show(); 
    } else {
        mainLogin->openDialog();
    }
}

void Systray::openSettingsDlg()
{
    mainWindow->hide();
    mainAbout->hide();
    mainSettings->show();
}


void Systray::showLogDialogAction()
{
    mainLog->showLogDialog();
}

void Systray::createActions()
{
    connectAction = new QAction(tr("&Connect"), this);
    connectAction->setIcon(QIcon(":/images/tray-plug-red.jpg"));
    connectAction->setIconVisibleInMenu(true);
    connect(connectAction, SIGNAL(triggered()), this, SLOT(beginLogin()));
    disconnectAction = new QAction(tr("&Disconnect"), this);
    disconnectAction->setIcon(QIcon(":/images/tray-dis-red.jpg"));
    disconnectAction->setIconVisibleInMenu(true);
    connect(disconnectAction, SIGNAL(triggered()), this, SLOT(disconnectOpenvpn()));
    statusAction = new QAction(tr("&Status"), this);
    statusAction->setIcon(QIcon(":/images/tray-graph2-red.jpg"));
    statusAction->setIconVisibleInMenu(true);
    connect(statusAction, SIGNAL(triggered()), this, SLOT(openMainStatusWindow()));
    jumpAction = new QAction(tr("&Jump to Faster"), this);
    jumpAction->setIcon(QIcon(":/images/tray-rocket-red.jpg"));
    jumpAction->setIconVisibleInMenu(true);
    connect(jumpAction, SIGNAL(triggered()), qApp, SLOT(quit()));
    settingAction = new QAction(tr("&Settings"), this);
    settingAction->setIcon(QIcon(":/images/tray-config-red.jpg"));
    settingAction->setIconVisibleInMenu(true);
    connect(settingAction, SIGNAL(triggered()), this, SLOT(openSettingsDlg()));
    logAction = new QAction(tr("&Logs"), this);
    logAction->setIcon(QIcon(":/images/tray-note-red.jpg"));
    logAction->setIconVisibleInMenu(true);
    connect(logAction, SIGNAL(triggered()), this, SLOT(showLogDialogAction()));
    webAction = new QAction(tr("&Web Management"), this);
    webAction->setIcon(QIcon(":/images/tray-mouse-red.jpg"));
    webAction->setIconVisibleInMenu(true);
    connect(webAction, SIGNAL(triggered()), this, SLOT(openWebLink()));
    feedbackAction = new QAction(tr("&Feedback/Support"), this);
    feedbackAction->setIcon(QIcon(":/images/tray-help-red.jpg"));
    feedbackAction->setIconVisibleInMenu(true);
    connect(feedbackAction, SIGNAL(triggered()), this, SLOT(openFeedbackLink()));
    bugAction = new QAction(tr("&Report Bug"), this);
    bugAction->setIcon(QIcon(":/images/tray-attention-red.jpg"));
    bugAction->setIconVisibleInMenu(true);
    connect(bugAction, SIGNAL(triggered()), this, SLOT(openBugLink()));
    moneyAction = new QAction(tr("&Earn Money"), this);
    moneyAction->setIcon(QIcon(":/images/tray-icon-red.jpg"));
    moneyAction->setIconVisibleInMenu(true);
    connect(moneyAction, SIGNAL(triggered()), this, SLOT(openMoneyLink()));

    aboutAction = new QAction(tr("&About"), this);
    aboutAction->setIcon(QIcon(":/images/tray-info-red.jpg"));
    aboutAction->setIconVisibleInMenu(true);
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(showAbout()));
    closeAction = new QAction(tr("&Close"), this);
    closeAction->setIcon(QIcon(":/images/tray-close-red.jpg"));
    closeAction->setIconVisibleInMenu(true);
    connect(closeAction, SIGNAL(triggered()), qApp, SLOT(quit()));
}

void Systray::createTrayIcon()
{
    trayIconMenu = new QMenu();
    trayIconMenu->addAction(connectAction);
    trayIconMenu->addAction(disconnectAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(statusAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(jumpAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(settingAction);
    trayIconMenu->addAction(logAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(webAction);
    trayIconMenu->addAction(feedbackAction);
    trayIconMenu->addAction(bugAction);
    trayIconMenu->addAction(moneyAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(aboutAction);
    trayIconMenu->addAction(closeAction);

    QIcon *icon = new QIcon(":/images/vc.ico");
    trayIcon = new QSystemTrayIcon();
    trayIcon->setContextMenu(trayIconMenu);
    trayIcon->setIcon(*icon);
}

void Systray::disconnectOpenvpn()
{
    mainWindow->sendCloseOpenvpnSignal();
}

