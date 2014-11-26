#include <QtGui>
#include <QDialog>

#include <string.h>
#include "settings.h"
#include "utils.h"

Settings::Settings()
{
    mainImg = new ImageLabel(":/images/main1.png", ":/images/main2.png", this);
    connect(mainImg, SIGNAL(clicked()), this, SLOT(returnMainWindow()));
    advancedSettings = new QLabel(this);
    advancedSettings->setText(tr("Advanced Settings"));
    showLogButton = new QPushButton(this);
    updateButton = new QPushButton(this);
    
    autoRunBox = new QCheckBox(this);
    autoRunBox->setText(tr("Launch app. when computer starts"));
    autoStartBox = new QCheckBox(this);
    autoStartBox->setText(tr("Auto-connect when app start"));
    reConnBox = new QCheckBox(this);
    reConnBox->setText(tr("Re-connect when connection drop"));
    autoConnBox = new QCheckBox(this);
    autoConnBox->setText(tr("Auto-connect when unsecured WiFi"));
    displayBox = new QCheckBox(this);
    displayBox->setText(tr("Display all individual node servers"));
    killBox = new QCheckBox(this);
    killBox->setText(tr("Kill internet when VPN disconnects"));
    killBox2 = new QLabel(this);
    killBox2->setText(tr("(kill switch)"));
    disableIPv6Box = new QCheckBox(this);
    disableIPv6Box->setText(tr("Disable IPv6 leak"));
    dnsBox = new QCheckBox(this);
    dnsBox->setText(tr("Use proxy.sh DNS (DNS leak fix)"));

    localOpenvpnSettings = new QLabel(this);
    localOpenvpnSettings->setText(tr("Local OpenVPN settings"));
    openvpnPort = new QLineEdit(this);
    openvpn = new QComboBox(this);
    openvpn->addItem("Encryption");
    openvpn->addItem("Test");

    primaryDNSLabel = new QLabel(this);
    primaryDNSLabel->setText(tr("Primary custom DNS"));
    secondaryDNSLabel = new QLabel(this);
    secondaryDNSLabel->setText(tr("Secondary DNS"));
    primaryDNS = new QLineEdit(this);
    primaryDNS->setText("8.8.8.8");
    secondaryDNS = new QLineEdit(this);
    secondaryDNS->setText("8.8.4.4");

    portForwardLabel = new QLabel(this);
    portForwardLabel->setText(tr("Port forwarding (NAT/Firewall)"));
    port = new QLineEdit(this);

    // set UI
    mainImg->setGeometry(QRect(19, 10, 80, 80));
    mainImg->setStyleSheet("border-image: url(:/images/main1.png)");
    advancedSettings->setGeometry(QRect(120, 10, 150, 30));
    showLogButton->setGeometry(QRect(120, 50, 70, 30));
    showLogButton->setStyleSheet("border-image: url(:/images/settings-showlog.jpg)");
    updateButton->setGeometry(QRect(194, 50, 70, 30));
    updateButton->setStyleSheet("border-image: url(:/images/settings-update.jpg)");
    
    autoRunBox->setGeometry(QRect(10, 110, 260, 20));
    autoStartBox->setGeometry(QRect(10, 135, 260, 20));
    reConnBox->setGeometry(QRect(10, 160, 260, 20));
    autoConnBox->setGeometry(QRect(10, 185, 260, 20));
    displayBox->setGeometry(QRect(10, 210, 260, 20));
    killBox->setGeometry(QRect(10, 230, 260, 30));
    killBox2->setGeometry(QRect(10, 250, 260, 30));

    localOpenvpnSettings->setGeometry(QRect(290, 10, 200, 30));
    openvpnPort->setGeometry(QRect(290, 45, 120, 30));
    openvpn->setGeometry(QRect(415, 45, 130, 30));
    primaryDNSLabel->setGeometry(QRect(290, 80, 120, 30));
    secondaryDNSLabel->setGeometry(QRect(415, 80, 130, 30));
    primaryDNS->setGeometry(QRect(290, 113, 120, 30));
    secondaryDNS->setGeometry(QRect(415, 113, 120, 30));
    portForwardLabel->setGeometry(QRect(290, 150, 200, 30));
    port->setGeometry(QRect(290, 183, 255, 30));
    disableIPv6Box->setGeometry(QRect(290, 220, 200, 20));
    dnsBox->setGeometry(QRect(290, 240, 250, 20));
    this->setStyleSheet("QDialog{border-image: url(:/images/settings-bg.png)}"
        "QCheckBox::indicator{width: 15px; height: 15px;}"
        "QCheckBox::indicator:unchecked{image: url(:/images/login-checkbox-unselected.jpg);}"
        "QCheckBox::indicator:checked {image: url(:/images/login-checkbox-selected.jpg);}");

    QFont font;
    font.setPointSize(10);
    font.setFamily(("unifont"));

    autoRunBox->setFont(font);
    autoStartBox->setFont(font);
    reConnBox->setFont(font);
    autoConnBox->setFont(font);
    displayBox->setFont(font);
    killBox->setFont(font);
    killBox2->setFont(font);
    disableIPv6Box->setFont(font);
    dnsBox->setFont(font);

    openvpnPort->setFont(font);
    openvpn->setFont(font);
    primaryDNS->setFont(font);
    secondaryDNS->setFont(font);
    port->setFont(font);

    font.setPointSize(11);
    advancedSettings->setFont(font);
    font.setPointSize(10);
    font.setBold(true);
    localOpenvpnSettings->setFont(font);
    primaryDNSLabel->setFont(font);
    secondaryDNSLabel->setFont(font);
    portForwardLabel->setFont(font);

    connect(autoRunBox, SIGNAL(toggled(bool)), this, SLOT(autoRunBoxChanged(bool)));
    connect(autoStartBox, SIGNAL(toggled(bool)), this, SLOT(autoStartBoxChanged(bool)));
    connect(reConnBox, SIGNAL(toggled(bool)), this, SLOT(reConnBoxChanged(bool)));
    connect(autoConnBox, SIGNAL(toggled(bool)), this, SLOT(autoConnBoxChanged(bool)));
    connect(displayBox, SIGNAL(toggled(bool)), this, SLOT(displayBoxChanged(bool)));
    connect(killBox, SIGNAL(toggled(bool)), this, SLOT(killBoxChanged(bool)));
    connect(disableIPv6Box, SIGNAL(toggled(bool)), this, SLOT(disableIPv6BoxChanged(bool)));
    connect(dnsBox, SIGNAL(toggled(bool)), this, SLOT(dnsBoxChanged(bool)));
    connect(updateButton, SIGNAL(clicked()), this, SLOT(saveSettingsAction()));
    connect(showLogButton, SIGNAL(clicked()), this, SLOT(viewLogDialogAction()));

    icon = new QIcon(":/images/vc.ico");
    setWindowIcon(*icon);
    resize(550, 300);
    setMinimumSize(QSize(550, 300));
    setMaximumSize(QSize(550, 300));
    setWindowTitle(tr("Safejumper"));

    char homeDir[64];
    (void) memset(settingsFile, '\0', sizeof(settingsFile));
    (void) memset(homeDir, '\0', sizeof(homeDir));

#if defined(Q_OS_WIN)
	(void) strcpy(homeDir, getenv("LOCALAPPDATA"));
    (void) sprintf(settingsFile, "%s\\safejumper\\settings.xml", homeDir);
#else
	(void) strcpy(homeDir, getenv("HOME"));
    (void) sprintf(settingsFile, "%s/.safejumper/settings.xml", homeDir);
#endif

    // load preivous saved settings from settings file
    if (getSettings(settingsFile, &s) != 0) {
        (void) strcpy(s.dns1, "8.8.8.8");
        (void) strcpy(s.dns2, "8.8.8.4");
        s.activatePort = 0;
        s.launchOnLogin = false;
        s.connectWhenLaunches = false;
        s.reConnectWhenDrops = false;
        s.wifiAutoConnect = false;
        s.displayFullServerList = false;
        s.killInterfacesWhenEndVPN = false;
        s.disableIPv6 = true;
        s.dns = false;
        saveSettings(settingsFile, s);
    }
    primaryDNS->setText(s.dns1);
    secondaryDNS->setText(s.dns2);
    autoRunBox->setChecked(s.launchOnLogin ? 1 : 0);
    autoStartBox->setChecked(s.connectWhenLaunches ? 1 : 0);
    reConnBox->setChecked(s.reConnectWhenDrops ? 1: 0);
    autoConnBox->setChecked(s.wifiAutoConnect ? 1: 0);
    displayBox->setChecked(s.displayFullServerList ? 1:0);
    killBox->setChecked(s.killInterfacesWhenEndVPN ? 1:0);
    disableIPv6Box->setChecked(s.killInterfacesWhenEndVPN ? 1:0);
    dnsBox->setChecked(s.dns ? 1:0);

    printf("Finish settings dialog init\n");
}

Settings::~Settings()
{
    delete mainImg;
    delete advancedSettings;
    delete localOpenvpnSettings;
    delete primaryDNSLabel;
    delete secondaryDNSLabel;
    delete portForwardLabel;

    delete autoRunBox;
    delete autoStartBox;
    delete reConnBox;
    delete autoConnBox;
    delete displayBox;
    delete killBox;
    delete disableIPv6Box;
    delete dnsBox;

    delete openvpnPort;
    delete openvpn;
    delete primaryDNS;
    delete secondaryDNS;
    delete portForwardLabel;
    delete port;
    delete showLogButton;
    delete updateButton;
    delete icon; 
}

void Settings::autoRunBoxChanged(bool state)
{
    (void) state;
    QPalette pal;

    if (autoRunBox->isChecked() == true) {
        pal.setColor(QPalette::Text, QColor(0,0,0));
        autoRunBox->setPalette(pal);
    } else {
        pal.setColor(QPalette::Text, QColor(192,192,192));
        autoRunBox->setPalette(pal);
    }
}

void Settings::autoStartBoxChanged(bool state)
{
    (void) state;
    QPalette pal;

    if (autoStartBox->isChecked() == true) {
        pal.setColor(QPalette::Text, QColor(0,0,0));
        autoStartBox->setPalette(pal);
    } else {
        pal.setColor(QPalette::Text, QColor(192,192,192));
        autoStartBox->setPalette(pal);
    }
}

void Settings::reConnBoxChanged(bool state)
{
    (void) state;
    QPalette pal;

    if (reConnBox->isChecked() == true) {
        pal.setColor(QPalette::Text, QColor(0,0,0));
        reConnBox->setPalette(pal);
    } else {
        pal.setColor(QPalette::Text, QColor(192,192,192));
        reConnBox->setPalette(pal);
    }
}

void Settings::autoConnBoxChanged(bool state)
{
    (void) state;
    QPalette pal;

    if (autoConnBox->isChecked() == true) {
        pal.setColor(QPalette::Text, QColor(0,0,0));
        autoConnBox->setPalette(pal);
    } else {
        pal.setColor(QPalette::Text, QColor(192,192,192));
        autoConnBox->setPalette(pal);
    }
}

void Settings::displayBoxChanged(bool state)
{
    (void) state;
    QPalette pal;

    if (displayBox->isChecked() == true) {
        pal.setColor(QPalette::Text, QColor(0,0,0));
        displayBox->setPalette(pal);
    } else {
        pal.setColor(QPalette::Text, QColor(192,192,192));
        displayBox->setPalette(pal);
    }
}

void Settings::killBoxChanged(bool state)
{
    (void) state;
    QPalette pal;

    if (killBox->isChecked() == true) {
        pal.setColor(QPalette::Text, QColor(0,0,0));
        killBox->setPalette(pal);
    } else {
        pal.setColor(QPalette::Text, QColor(192,192,192));
        killBox->setPalette(pal);
    }
}

void Settings::disableIPv6BoxChanged(bool state)
{
    (void) state;
    QPalette pal;

    if (disableIPv6Box->isChecked() == true) {
        pal.setColor(QPalette::Text, QColor(0,0,0));
        disableIPv6Box->setPalette(pal);
    } else {
        pal.setColor(QPalette::Text, QColor(192,192,192));
        disableIPv6Box->setPalette(pal);
    }
}

void Settings::dnsBoxChanged(bool state)
{
    (void) state;
    QPalette pal;

    if (dnsBox->isChecked() == true) {
        pal.setColor(QPalette::Text, QColor(0,0,0));
        dnsBox->setPalette(pal);
    } else {
        pal.setColor(QPalette::Text, QColor(192,192,192));
        dnsBox->setPalette(pal);
    }
}

void Settings::returnMainWindow()
{
    emit returnMain();
}

char *qstring2char(QString &s)
{
    QByteArray t = s.toLatin1();
    return (char *)(s.data());
}

void Settings::saveSettingsAction()
{
    bool save = false;
    QString tempPort = openvpnPort->text();
    QString tempdns1 = primaryDNS->text();
    QString tempdns2 = secondaryDNS->text();
    int portUpdate = atoi(qstring2char(tempPort));

    if (portUpdate != s.activatePort) {
        s.activatePort = portUpdate;
        save = true;
    }
    if (strcmp(qstring2char(tempdns1), s.dns1)) {
        (void) strcpy(s.dns1, qstring2char(tempdns1));
        save = true;
    }
    if (strcmp(qstring2char(tempdns2), s.dns2)) {
        (void) strcpy(s.dns2, qstring2char(tempdns2));
        save = true;
    }
    if (autoRunBox->isChecked() != s.launchOnLogin) {
        s.launchOnLogin = autoRunBox->isChecked();
        save = true;
    }
    if (autoStartBox->isChecked() != s.connectWhenLaunches) {
        s.connectWhenLaunches = autoStartBox->isChecked();
        save = true;
    }
    if (reConnBox->isChecked() != s.reConnectWhenDrops) {
        s.reConnectWhenDrops = reConnBox->isChecked();
        save = true;
    }
    if (autoConnBox->isChecked() != s.wifiAutoConnect) {
        s.wifiAutoConnect = autoConnBox->isChecked();
        save = true;
    }
    if (displayBox->isChecked() != s.displayFullServerList) {
        s.displayFullServerList = displayBox->isChecked();
        save = true;
    }
    if (killBox->isChecked() != s.killInterfacesWhenEndVPN) {
        s.killInterfacesWhenEndVPN = killBox->isChecked();
        save = true;
    }
    if (disableIPv6Box->isChecked() != s.disableIPv6) {
        s.disableIPv6 = disableIPv6Box->isChecked();
        save = true;
    }
    if (dnsBox->isChecked() != s.dns) {
        s.dns = dnsBox->isChecked();
        save = true;
    }

    if (save == true) {
        saveSettings(settingsFile, s);
    }
}

void Settings::viewLogDialogAction()
{
    emit showLogDialogSignals();
}

