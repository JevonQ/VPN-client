#include <QtGui>
#include "location.h"

Location::Location()
{
    label1 = new QLabel(this);
    label2 = new QLabel(this);
    mainImg = new ImageLabel(":/images/main1.png", ":/images/main2.png", this);
    connect(mainImg, SIGNAL(clicked()), this, SLOT(returnMainWindow()));
    connectButton = new QPushButton(this);
    settingButton= new QPushButton(this);
    protocolCom = new QComboBox(this);
    protocolCom->addItem("-- Select Protocol --");
    protocolCom->addItem("TCP");
    protocolCom->addItem("UDP");
    protocolCom->setCurrentIndex(0);
    country = new QComboBox(this);
    // test item
    country->addItem("-- Select Location --");
    country->setCurrentIndex(0);

    // set UI
    mainImg->setGeometry(QRect(19, 10, 80, 80));
    label1->setGeometry(QRect(155, 20, 25, 25));
    label1->setStyleSheet("border-image: url(:/images/location-11.jpg)");
    label2->setGeometry(QRect(155, 55, 25, 25));
    label2->setStyleSheet("border-image: url(:/images/location-21.jpg)");
    protocolCom->setGeometry(QRect(190, 20, 200, 30));
    country->setGeometry(QRect(190, 55, 200, 30));
    connectButton->setGeometry(QRect(90, 305, 100, 40));
    connectButton->setStyleSheet("border-image: url(:/images/location-connect.jpg)");
    settingButton->setGeometry(QRect(210, 305, 100, 40));
    settingButton->setStyleSheet("border-image: url(:/images/location-settings.jpg)");
    this->setStyleSheet("QDialog{border-image: url(:/images/location-bg.png);}");

    connect(protocolCom, SIGNAL(currentIndexChanged(const QString &)), this,
        SLOT(protocolChanged(const QString &)));
    connect(country, SIGNAL(currentIndexChanged(const QString &)), this,
        SLOT(countryChanged(const QString &)));
    connect(connectButton, SIGNAL(clicked()), this, SLOT(connectClickAction()));
    connect(settingButton, SIGNAL(clicked()), this, SLOT(settingClickAction()));

    locationReady = false;
    protocolReady = false;

    icon = new QIcon(":/images/vc.ico");
    setWindowIcon(*icon);
    resize(400, 355);
    setMinimumSize(QSize(400, 355));
    setMaximumSize(QSize(400, 355));
    setWindowTitle(tr("Safejumper"));

    char homeDir[32];
    (void) memset(serverListFile, '\0', sizeof(serverListFile));
    (void) memset(homeDir, '\0', sizeof(homeDir));
#if defined(Q_OS_WIN)
    (void) strcpy(homeDir, getenv("LOCALAPPDATA"));
    (void) sprintf(serverListFile, "%s\\safejumper\\server.xml", homeDir);
#else
    (void) strcpy(homeDir, getenv("HOME"));
    (void) sprintf(serverListFile, "%s/.safejumper/server.xml", homeDir);
#endif
    printf("Finish location dialog init\n");
}

Location::~Location()
{
    delete mainImg;
    delete label1;
    delete label2;
    delete mapLabel;
    delete protocolCom;
    delete country;
    delete connectButton;
    delete settingButton;
    delete icon;
}

void Location::protocolChanged(const QString &text)
{
    if (text == "-- Select Protocol--"){
        country->setEnabled(false);
        country->setCurrentIndex(0);
        protocolReady = false;
    } else {
        country->setEnabled(true);
        protocolReady = true;
        protocol = text;
        port = text;
    }
}

void Location::countryChanged(const QString &text)
{
    if (text == "-- Select Location--"){
        locationReady = false;
    } else {
        locationReady = true;
        location = text;
        label2->setStyleSheet("border-image: url(:/images/location-22.jpg)");
    }
}

void Location::returnMainWindow()
{
    emit returnMain();
}

void Location::connectClickAction()
{
    if (locationReady && protocolReady) {
        protocol = protocolCom->currentText();
        if (protocol == "TCP") {
            protocol = "tcp";
            port = "80";
        } else {
            protocol = "udp";
            port = "443";
        }
        location = QString(serverList[countryList[country->currentIndex()-1].res].location);
        emit showBalloon(1, "Safejumper, by proxy.sh", "Connecting to " + location + ". Please hold on...");
        emit returnMainWithLocation(protocol, port, location);
    }
}

void Location::settingClickAction()
{
    this->hide();
    emit settingsDialgShow();
}

/*
 * must test the login status before calling this function
 */
void Location::showLocationAfterLogin()
{
    int i = 0;
    // initiate the server list via server.xml
    int serverCount = getXmlNodeCount(serverListFile);
    serverList = new Server[serverCount];
    countryList = new Server[serverCount];
    (void) memset(serverList, 0, sizeof (Server) * serverCount);
    (void) memset(countryList, 0, sizeof (Server) * serverCount);
    (void) getServerList(serverListFile, serverList);
    int countryCount = getCountryList(serverList, serverCount, countryList);
    for (; i < countryCount; i ++) {
        //printf("%s\n", serverList[i].location);
        country->addItem(countryList[i].location);
    }
    this->show();
}
