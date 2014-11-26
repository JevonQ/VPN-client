#ifndef SYSTRAY_H
#define SYSTRAY_H

#include <QSystemTrayIcon>
#include <QDialog>
#include "statuswindow.h"
#include "settings.h"
#include "login.h"
#include "about.h"
#include "location.h"
#include "logdialog.h"

class QAction;
class QCheckBox;
class QComboBox;
class QGroupBox;
class QLabel;
class QLineEdit;
class QMenu;
class QPushButton;
class QSpinBox;
class QTextEdit;

class Systray : public QObject
{
    Q_OBJECT
public:
    Systray(StatusWindow *w, About *a, Settings *s, LoginDlg *d, Location *l, LogDialog *o);

protected:
    void closeEvent(QCloseEvent *event);

public slots:
    void openvpnStatusChanged(const int &);
    void showBalloonHint(const int &type, const QString &title, const QString &message);

signals:
    void addLog(const QString &e);

private slots:
    void beginLogin();
    void setIcon(int index);
    void iconActivated(QSystemTrayIcon::ActivationReason reason);
    void messageClicked();
    void showAbout();
    void openBugLink();
    void openWebLink();
    void openMoneyLink();
    void openFeedbackLink();
    void openMainStatusWindow();
    void openSettingsDlg();
    void showLogDialogAction();
    void disconnectOpenvpn();

private:
    void createActions();
    void createTrayIcon();

    QAction *minimizeAction;
    QAction *maximizeAction;
    QAction *restoreAction;
    QAction *quitAction;

    QAction *connectAction;
    QAction *disconnectAction;
    QAction *statusAction;
    QAction *jumpAction;
    QAction *settingAction;
    QAction *logAction;
    QAction *webAction;
    QAction *feedbackAction;
    QAction *bugAction;
    QAction *moneyAction;
    QAction *aboutAction;
    QAction *closeAction;

    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;

    StatusWindow *mainWindow;
    About *mainAbout;
    Settings *mainSettings;
    LoginDlg *mainLogin;
    Location *mainLocation;
    LogDialog *mainLog;
};

#endif
