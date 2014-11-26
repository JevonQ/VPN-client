#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDialog>
#include "imagelabel.h"

class QLabel;
class QPushButton;
class QIcon;
class QComboBox;
class QCheckBox;
class QLineEdit;

typedef struct settings_s
{
    char dns1[16];
    char dns2[16];
    int activatePort;
    bool launchOnLogin;
    bool connectWhenLaunches;
    bool reConnectWhenDrops;
    bool wifiAutoConnect;
    bool displayFullServerList;
    bool killInterfacesWhenEndVPN;
    bool disableIPv6;
    bool dns;
} settings;

class Settings: public QDialog
{
    Q_OBJECT
public:
    Settings();
    virtual ~Settings();

signals:
    void addLog(const QString &l);
    void showBalloon(const int &type, const QString &t, const QString &m);

public slots:

signals:
    void returnMain();
    void showLogDialogSignals();
    void addLog(QString &l);

protected:

private slots:
    void autoRunBoxChanged(bool on);
    void autoStartBoxChanged(bool on);
    void reConnBoxChanged(bool on);
    void autoConnBoxChanged(bool on);
    void displayBoxChanged(bool on);
    void killBoxChanged(bool on);
    void disableIPv6BoxChanged(bool on);
    void dnsBoxChanged(bool on);
    void returnMainWindow();
    void saveSettingsAction();
    void viewLogDialogAction();

private:
    ImageLabel *mainImg;
    QLabel *advancedSettings;
    QLabel *localOpenvpnSettings;
    QLabel *primaryDNSLabel;
    QLabel *secondaryDNSLabel;
    QLabel *portForwardLabel;

    QCheckBox *autoRunBox;
    QCheckBox *autoStartBox;
    QCheckBox *reConnBox;
    QCheckBox *autoConnBox;
    QCheckBox *displayBox;
    QCheckBox *killBox;
    QLabel    *killBox2;
    QCheckBox *disableIPv6Box;
    QCheckBox *dnsBox;

    QLineEdit *openvpnPort;
    QComboBox *openvpn;
    QLineEdit *primaryDNS;
    QLineEdit *secondaryDNS;
    QLineEdit *port;
    QPushButton *showLogButton;
    QPushButton *updateButton;
    QIcon *icon; 

    settings s;
    char settingsFile[255];
};

#endif
