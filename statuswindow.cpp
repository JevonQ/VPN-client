#include <QMainWindow>
#include <QDebug>
#include <QtGui>
#include <QtCore/QSharedMemory>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#if defined(Q_OS_WIN)
#include <windows.h>
#include <winsock2.h>
#else
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#endif
#include "statuswindow.h"

void LLabel::mouseReleaseEvent(QMouseEvent *e)
{
    emit clicked();
    QLabel::mouseReleaseEvent(e);
}

void LLabel::enterEvent(QEvent *e)
{
    setCursor(Qt::PointingHandCursor);
    QLabel::enterEvent(e);
}

StatusWindow::StatusWindow(Settings *s, LoginDlg *d, Location *l,
        LogDialog *log, QWidget *parent):QMainWindow(parent)
{
    mainSettings = s;
    mainLoginDlg = d;
    mainLocation = l;
    mainLog = log;
    connect(mainSettings, SIGNAL(returnMain()), this, SLOT(sReturnMain()));
    connect(mainLocation, SIGNAL(returnMain()), this, SLOT(lReturnMain()));
    connect(mainLog, SIGNAL(returnMain()), this, SLOT(logReturnMain()));

    titleImg = new QLabel(this);
    cStatus = new QLabel(this);
    username = new QLabel(this);
    email = new QLabel(this);
    package = new QLabel(this);
    activeTime = new QLabel(this);

    locationImg = new LLabel(this);
    countryNode = new QLabel(this);
    nodeStatus = new QLabel(this);
    countryImg = new QLabel(this);

    ipImg = new LLabel(this);
    newIp = new QLabel(this);
    oldIp = new QLabel(this);
    newIpInfo = new QLabel(this);
    oldIpInfo = new QLabel(this);

    protocolImg = new LLabel(this);
    protocolInfo = new QLabel(this);
    protocol = new QLabel(this);
    encryptionInfo = new QLabel(this);
    encryption = new QLabel(this);

    connectBtn = new QPushButton(this);
    jumperBtn = new QPushButton(this);
    settingBtn = new QPushButton(this);
    
    // Initiate
    QPalette pe;
    pe.setColor(QPalette::WindowText,Qt::gray);
    newIpInfo->setText("NEW IP ADDRESS");
    newIpInfo->setPalette(pe);
    oldIpInfo->setText("OLD IP ADDRESS");
    oldIpInfo->setPalette(pe);
    protocolInfo->setText("PROTOCOL");
    protocolInfo->setPalette(pe);
    encryptionInfo->setText("ENCRYPTION");
    encryptionInfo->setPalette(pe);
    email->setPalette(pe);
    activeTime->setPalette(pe);
    QFont font;
    font.setPointSize(10);
    font.setFamily(("helvetica"));
    newIpInfo->setFont(font);
    oldIpInfo->setFont(font);
    protocolInfo->setFont(font);
    encryptionInfo->setFont(font);
    email->setFont(font);
    activeTime->setFont(font);

    font.setBold(true);
    countryNode->setFont(font);
    newIp->setFont(font);
    oldIp->setFont(font);
    protocol->setFont(font);
    encryption->setFont(font);
    username->setFont(font);
    package->setFont(font);

    // test
    username->setText("<b>test</b>");
    email->setText("(a@oracle.com)");
    package->setText("hello");
    activeTime->setText("world!");
    newIp->setText("1.1.1.1");
    oldIp->setText("2.2.2.2");
    protocol->setText("OPENVPN TCP(80)");
    encryption->setText("RSA");

    /*
     * set the UI
     */
    titleImg->setGeometry(QRect(10, 10, 80, 80));
    titleImg->setStyleSheet("border-image: url(:/images/main1.png)");
    cStatus->setGeometry(QRect(130, 20, 180, 20));
    QPixmap cstatus(":/images/img_disconnect.png");
    cStatus->setPixmap(cstatus);
    username->setGeometry(QRect(130, 42, 150, 20));
    username->setAlignment(Qt::AlignCenter);
    email->setGeometry(QRect(130, 65, 150, 20));
    email->setAlignment(Qt::AlignCenter);
    
    package->setGeometry(QRect(340, 20, 50, 30));
    activeTime->setGeometry(QRect(340, 60, 50, 30));
    
    locationImg->setGeometry(QRect(10, 105, 40, 40));
    locationImg->setStyleSheet("border-image: url(:/images/mainlocation.jpg)");
    countryNode->setGeometry(QRect(85, 110, 100, 20));
    countryNode->setText(tr("Australia"));
    nodeStatus->setGeometry(QRect(85, 130, 100, 20));
    nodeStatus->setText(tr("Load: 10%"));
    countryImg->setGeometry(QRect(354, 114, 30, 29));
    countryImg->setStyleSheet("border-image: url(:/images/flag_australia.png)");

    ipImg->setGeometry(QRect(10, 167, 40, 40));
    ipImg->setStyleSheet("border-image: url(:/images/mainip.jpg)");

    newIpInfo->setGeometry(QRect(250, 170, 150, 20));
    oldIpInfo->setGeometry(QRect(85, 168, 150, 20));
    newIp->setGeometry(QRect(250, 184, 150, 30));
    oldIp->setGeometry(QRect(85, 184, 150, 30));

    protocolImg->setGeometry(QRect(10, 235, 40, 40));
    protocolImg->setStyleSheet("border-image: url(:/images/mainstream.jpg)");
    protocolInfo->setGeometry(QRect(85, 230, 150, 20));
    protocol->setGeometry(QRect(85, 244, 150, 30));
    encryptionInfo->setGeometry(QRect(250, 230, 150, 20));
    encryption->setGeometry(QRect(250, 244, 150, 30));

    connectBtn->setGeometry(QRect(15, 300, 105, 40));
    connectBtn->setStyleSheet("border-image: url(:/images/main-connect.jpg)");
    jumperBtn->setGeometry(QRect(140, 300, 105, 40));
    jumperBtn->setStyleSheet("border-image: url(:/images/main-jump.jpg)");
    settingBtn->setGeometry(QRect(266, 300, 105, 40));
    settingBtn->setStyleSheet("border-image: url(:/images/main-settings.jpg)");

    icon = new QIcon(":/images/vc.ico");
    setWindowIcon(*icon);
    setWindowTitle(tr("Safejumper"));
    resize(400, 361);
    setMinimumSize(QSize(400, 361));
    setMaximumSize(QSize(400, 361));
    this->setStyleSheet("QMainWindow{border-image: url(:/images/mainwindow.jpg)}");


    connect(locationImg, SIGNAL(clicked()), this, SLOT(openLocation()));
    connect(ipImg, SIGNAL(clicked()), this, SLOT(openLocation()));
    connect(protocolImg, SIGNAL(clicked()), this, SLOT(openLocation()));
    connect(connectBtn, SIGNAL(clicked()), this, SLOT(openvpnConnect()));
    connect(settingBtn, SIGNAL(clicked()), this, SLOT(openSettings()));
    // Login Dialog trigger the sinal to make the main window show
    connect(mainLoginDlg, SIGNAL(loginSuccess()), this, SLOT(mainWindowShow()));
    connect(mainLocation, SIGNAL(returnMainWithLocation(QString &, QString &, QString &)), this, SLOT(tryConnect(QString &, QString &, QString &)));
    locationReady = false;
    protocolReady = false;

    char homeDir[64];
    (void) memset(serverListFile, '\0', sizeof(serverListFile));
    (void) memset(homeDir, '\0', sizeof(homeDir));
#if defined(Q_OS_WIN)
    (void) strcpy(homeDir, getenv("LOCALAPPDATA"));
#else
    (void) strcpy(homeDir, getenv("HOME"));
#endif
    char mainDir[128];
    (void) memset(mainDir, '\0', sizeof(mainDir));
#if defined(Q_OS_WIN)
    (void) sprintf(mainDir, "mkdir %s\\safejumper\\", homeDir);
    (void) sprintf(serverListFile, "%s\\safejumper\\server.xml", homeDir);
#else
    (void) sprintf(mainDir, "mkdir -p %s/.safejumper/", homeDir);
    (void) sprintf(serverListFile, "%s/.safejumper/server.xml", homeDir);
#endif
    system(mainDir);        // create directory for main directory

    protocolStr = "TCP";
    ipStr = "1.2.3.4";
    locationStr = "HK";
    portStr = "80";

    // VPN connection related
    openvpnStatus = VPN_DISCONNECT;
    openvpnManagementPort = 60000;
    openvpnManagementIP = "127.0.0.1";

#if defined(Q_OS_WIN)
#define WINEPOCH 2208988800
#define WSVERS MAKEWORD(2,0) 
    WSADATA wsadata;
    if(WSAStartup(WSVERS, &wsadata)) {
        emit addLog("WSAStartup failed\n"); 
    }
#endif

    struct sockaddr_in server_addr;
    (void) memset(&server_addr, 0, sizeof (server_addr));
    //server_addr.sin_len = sizeof(struct sockaddr_in);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(openvpnManagementPort);
    server_addr.sin_addr.s_addr = inet_addr(qstringToChar(openvpnManagementIP));

    managementSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (managementSocket == -1) {
        emit addLog("Create socket failed\n");
    } else {
		emit addLog("Create socket success!\n");
    }
    // bind socket
    int bind_result = bind(managementSocket, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (bind_result == -1) {
        QString error = "Bind socket failed with error code is "+QString::number(errno, 10)+"\n";
        emit addLog(error);
    } else {
        emit addLog("Bind socket success!\n");
    }
    //listen
    if (listen(managementSocket, 5) == -1) {
        QString error = "Listen socket failed with error code is "+QString::number(errno, 10)+"\n";
        emit addLog(error);
    } else {
        emit addLog("Listen socket is ready!\n");
    }

    serverThread = new ServerThread(managementSocket);
    connect(serverThread, SIGNAL(sendStatus(const QString &)), this, SLOT(checkOpenvpnStatus(const QString &)));
	connect(serverThread, SIGNAL(addLog(const QString &)), mainLog, SLOT(addLogEntry(const QString &)));
    serverThread->start();
    statusTimer = new QTimer(this);
    connect(statusTimer, SIGNAL(timeout()), this, SLOT(timerCheckOpenvpn()));
    statusTimer->start(1000*10);
    printf("Finish main window init\n");
}

StatusWindow::~StatusWindow()
{
    serverThread->quit();
    serverThread->wait();
    delete icon;
    delete titleImg;
    delete cStatus;
    delete username;
    delete email;
    delete package;
    delete activeTime;
    delete locationImg;
    delete countryNode;
    delete nodeStatus;
    delete countryImg;
    delete ipImg;
    delete newIp;
    delete newIpInfo;
    delete oldIp;
    delete oldIpInfo;
    delete connectBtn;
    delete jumperBtn;
    delete settingBtn;
}

void StatusWindow::sendCloseOpenvpnSignal()
{
    char signalMsg[32];
    (void) memset(signalMsg, '\0', sizeof (signalMsg));
    strcpy(signalMsg, "signal SIGTERM");
    serverThread->sendAction(signalMsg);
    openvpnStatus = VPN_DISCONNECT;
    QPixmap cstatus(":/images/img_disconnect.png");
    cStatus->setPixmap(cstatus);
    emit openvpnStatusChangedSignal(openvpnStatus);
    emit showBalloon(1, "Safejumper, by proxy.sh", "OpenVPN process exited.");
}

void StatusWindow::beginOpenvpnThread()
{
    //modify 'CONNECT' button to disable
    connectBtn->setEnabled(false);

    // modify the 'CONNECT' status to in-process mode
    QPixmap cstatus(":/images/img_connecting.png");
    cStatus->setPixmap(cstatus);
    openvpnStatus = VPN_CONNECTING;
    emit addLog("Try to get the settings from persistent store\n");
    emit addLog("Try to get the template file from web\n");
    emit addLog("Process the template with the given arguments\n");
    emit addLog("Exec the openvpn system command\n");
}

void StatusWindow::endOpenvpnThread()
{
    connectBtn->setEnabled(true);

    emit addLog("The openvpn system command is in process\n");
}

void doConnect(QString &server_proto, QString &server_port, QString &server_ip, QString &managementSocketIp, int managementSocketPort)
{
    char homedir[64];
    char templateDir[64];
    char cmd[255];
    settings s;
    char settingsFile[128];
    char templateFile[128];
    char caFile[128];
    char pwdFile[128];
    int res = 0;
    char link[255];
    char vpnExec[255];
    FILE *file = NULL;
#if defined(Q_OS_LINUX)
    QString suapp;
#endif

    (void) memset(homedir, '\0', sizeof (homedir));
    (void) memset(templateDir, '\0', sizeof (templateDir));
    (void) memset(cmd, '\0', sizeof (cmd));
    (void) memset(&s, '0', sizeof (s));
    (void) memset(link, '\0', sizeof (link));
    (void) strcpy(link, "https://proxy.sh/ovpn-linux.tpl");
    (void) memset(settingsFile, '\0', sizeof (settingsFile));
    (void) memset(templateFile, '\0', sizeof (templateFile));
    (void) memset(caFile, '\0', sizeof (caFile));
#if defined(Q_OS_WIN)
    (void) strcpy(homedir, getenv("LOCALAPPDATA"));
    sprintf(templateDir, "%s\\safejumper", homedir);
    sprintf(cmd, "mkdir  %s", templateDir);
    sprintf(settingsFile, "%s\\safejumper/settings.xml", homedir);
    sprintf(templateFile, "%s\\safejumper\\ovpn-win.tpl", homedir);
    sprintf(caFile, "%s\\safejumper\\ca-file", homedir);
	sprintf(pwdFile, "%s\\safejumper\\passwd.txt", homedir);
#else
    (void) strcpy(homedir, getenv("HOME"));
    sprintf(templateDir, "%s/.safejumper", homedir);
    sprintf(cmd, "mkdir -p %s", templateDir);
    sprintf(settingsFile, "%s/.safejumper/settings.xml", homedir);
    sprintf(templateFile, "%s/.safejumper/ovpn-linux.tpl", homedir);
    sprintf(caFile, "%s/.safejumper/ca-file", homedir);
	sprintf(pwdFile, "%s/.safejumper/passwd.txt", homedir);
#endif
    system(cmd);
    // Get settings from the store of settings
    res = getSettings(settingsFile, &s);

    // Get template
    // Test whether we have the template locally 
    if ((file = fopen(templateFile, "r")) != NULL) {
        fclose(file);
        remove(templateFile);
    }

    res = getTemplateFile(templateFile, link);
#if defined(Q_OS_WIN)
    if (res < 32) {
#else
    if (res != 0) {
#endif
        printf("Download Error: %d\n", res);
        setTemplateFile(templateFile, res);
    }

    printf("Ready to process template file\n");
    qDebug()<<server_proto<<server_ip<<server_port<<endl;
    processTemplateFile(templateFile, caFile, pwdFile, server_proto, server_ip, server_port);
    // TODO Parse the template file and execute the openvpn command
    // invoke a system command
#if defined(Q_OS_WIN)
    sprintf(vpnExec, "bin\\openvpn.exe --config \"%s\" --management %s %d --management-client --management-query-passwords", templateFile, qstringToChar(managementSocketIp), managementSocketPort);;
	WinExec(vpnExec, SW_HIDE);
#else
#if defined(Q_OS_LINUX)
    if (checkFile("kdesu") == 0) {
        suapp = "kdesu";
    } else if (checkFile("gksu") == 0) {
        suapp = "gksu";
    } else if (checkFile("beesu") == 0) {
        suapp = "beesu";
    } else {
        suapp = "";
    }
    sprintf(vpnExec, "%s openvpn --config %s --management %s %d --management-client --management-query-passwords &", qstringToChar(suapp), templateFile, qstringToChar(managementSocketIp), managementSocketPort);;
#else
    sprintf(vpnExec, "openvpn --config %s --management %s %d --management-client --management-query-passwords &", templateFile, qstringToChar(managementSocketIp), managementSocketPort);;
#endif
    system(vpnExec);
#endif
}

void StatusWindow::openvpnConnect()
{
    if (!locationReady || !protocolReady) {
        emit addLog("Location or protocol is not ready, location dialog shows\n");
        this->hide();
        mainLocation->showLocationAfterLogin();
    } else {
        watcher = new QFutureWatcher<void>();
        connect(watcher, SIGNAL(started()), this, SLOT(beginOpenvpnThread()));
        connect(watcher, SIGNAL(finished()), this, SLOT(endOpenvpnThread()));
        qDebug()<<protocolStr<<portStr<<ipStr<<locationStr<<endl;
        QFuture<void> future = QtConcurrent::run(doConnect, protocolStr, portStr, ipStr, openvpnManagementIP, openvpnManagementPort);
        watcher->setFuture(future);
    }
}

void StatusWindow::sReturnMain()
{
    mainSettings->hide();
    this->show();
    setFocus();
}

void StatusWindow::lReturnMain()
{
    mainLocation->hide();
    this->show();
    setFocus();
}
void StatusWindow::logReturnMain()
{
    mainLog->hide();
    this->show();
    setFocus();
}

void StatusWindow::mainWindowShow()
{
    this->show();
    setFocus();

    // initiate the server list via server.xml
    serverCount = getXmlNodeCount(serverListFile);
    serverList = new Server[serverCount];
    (void) getServerList(serverListFile, serverList);
}

char * qstringToChar(QString &s)
{
    QByteArray t = s.toLatin1();
    return (char *)(t.data());
}

char * qconstStringToChar(const QString &s)
{
    QByteArray t = s.toLatin1();
    return (char *)(t.data());
}

void StatusWindow::tryConnect(QString &p, QString &port, QString &l)
{
    int i = 0;
    char locate[32] = {0};
    QString imgStyle;
    char s1[64] = {0};
    char s2[64] = {0};

    mainLocation->hide();
    this->show();
    setFocus();

    (void) strncpy(locate, qstringToChar(l), 32);
	if (p == "tcp") {
		protocol->setText("OPENVPN TCP "+port);
	} else {
		protocol->setText("OPENVPN UDP "+port);
	}
    countryNode->setText(l);
    for(; i < serverCount; i++) {
        if (!strcmp(serverList[i].location, locate)) {
            break;
        }
    }
    newIp->setText(serverList[i].address);
    
    protocolStr = p;
    portStr = port;
    ipStr = serverList[i].address;
    locationStr = serverList[i].location;
    strncpy(s1, qconstStringToChar(locationStr.toLower()), 64);
    replaceStr(s2, s1, " ", "_");
    imgStyle = "border-image: url(:/images/flag_" + QString(s2) + ".png)";
    countryImg->setStyleSheet(imgStyle);

    protocolReady = true;
    locationReady = true;
    QString errorLog("Note: protocol is "+p + ", location is "+serverList[i].address+" , port is "+serverList[i].location+"\n");
    emit addLog(errorLog);
    qDebug()<<protocolStr<<portStr<<ipStr<<locationStr<<endl;
    this->openvpnConnect();
}

int disconnectViaOpenvpn()
{
    return 0;
}

void StatusWindow::openSettings()
{
    this->hide();
    mainSettings->show();
}

void StatusWindow::openLocation()
{
    this->hide();
    mainLocation->showLocationAfterLogin();
}

void StatusWindow::checkOpenvpnStatus(const QString &status)
{
    qDebug()<<status<<endl;
    if (status.contains("CONNECTING") != 0) {
        emit addLog("OpenVPN''s initial state.\n");
        openvpnStatus = VPN_CONNECTING;
        QPixmap cstatus(":/images/img_connecting.png");
        cStatus->setPixmap(cstatus);
    }
    if (status.contains("WAIT") != 0) {
        emit addLog("Waiting for initial response from server.\n");
        openvpnStatus = VPN_CONNECTING;
        QPixmap cstatus(":/images/img_connecting.png");
        cStatus->setPixmap(cstatus);
    }
    if (status.contains("AUTH") != 0) {
        emit addLog("Authenticating with OpenVPN server.\n");
        openvpnStatus = VPN_CONNECTING;
        QPixmap cstatus(":/images/img_connecting.png");
        cStatus->setPixmap(cstatus);
    }
    if (status.contains("GET_CONFIG") != 0) {
        emit addLog("Downloading configuration options from server.\n");
        emit showBalloon(1, "Safejumper, by proxy.sh", "Downloading configuration options from server.");
        openvpnStatus = VPN_CONNECTING;
        QPixmap cstatus(":/images/img_connecting.png");
        cStatus->setPixmap(cstatus);
    }
    if (status.contains("ASSIGN_IP") != 0) {
        emit addLog("Assigning IP address to virtual network interface.\n");
        emit showBalloon(1, "Safejumper, by proxy.sh", "Assigning IP address to virtual network interface.");
        openvpnStatus = VPN_CONNECTING;
        QPixmap cstatus(":/images/img_connecting.png");
        cStatus->setPixmap(cstatus);
    }
    if (status.contains("ADD_ROUTES") != 0) {
        emit addLog("Adding routes to system.\n");
        openvpnStatus = VPN_CONNECTING;
        QPixmap cstatus(":/images/img_connecting.png");
        cStatus->setPixmap(cstatus);
    }
    if (status.contains("CONNECTED") != 0) {
        emit addLog("OpenVPN Initialization Sequence Completed.\n");
        emit showBalloon(1, "Safejumper, by proxy.sh", "Connected.");
        openvpnStatus = VPN_CONNECTED;
        QPixmap cstatus(":/images/img_connected.png");
        cStatus->setPixmap(cstatus);
        statusTimer->start(1000*60);
        emit openvpnStatusChangedSignal(openvpnStatus);
    }
    if (status.contains("RECONNECTING") != 0) {
        emit addLog("OpenVPN restart has occurred.\n");
        openvpnStatus = VPN_CONNECTING;
        QPixmap cstatus(":/images/img_connecting.png");
        cStatus->setPixmap(cstatus);
    }
    if (status.contains("EXITING") != 0) {
        emit addLog("OpenVPN graceful exit is in progress.\n");
        emit showBalloon(1, "Safejumper, by proxy.sh", "A graceful exit is in progress.");
        openvpnStatus = VPN_DISCONNECT;
        QPixmap cstatus(":/images/img_disconnect.png");
        cStatus->setPixmap(cstatus);
        emit openvpnStatusChangedSignal(openvpnStatus);
    }
    if (status.contains("END") != 0) {
        // end of the a response message
    }
}

void StatusWindow::timerCheckOpenvpn()
{
    char signalMsg[32];
    (void) memset(signalMsg, '\0', sizeof (signalMsg));
#if defined(Q_OS_WIN)
    strcpy(signalMsg, "state on");
	emit addLog("Send 'state on' command to openvpn\n");
    serverThread->sendAction(signalMsg);
	(void) memset(signalMsg, '\0', sizeof (signalMsg));
	strcpy(signalMsg, "state all");
#else
	strcpy(signalMsg, "state");
#endif
	emit addLog("Send 'state' command to openvpn\n");
    serverThread->sendAction(signalMsg);
}



void processTemplateFile(char *templateFile, char *cafile, char *passwdFile, QString &server_proto, QString &server_ip, QString &server_port)
{
    char content[64][128];

    FILE *tFILE = NULL;
    FILE *cFILE = NULL;
    int i = 0;
    int count = 0;
    int ca = 0;
    char *point = NULL;
    (void) memset(content, '\0', sizeof (content));

	remove(cafile);
    cFILE = fopen(cafile, "wb");
	if (cFILE == NULL) {
		return;
	}
    tFILE = fopen(templateFile, "r");
	if (tFILE == NULL) {
		return;
	}
    qDebug()<<server_proto<<server_ip<<server_port<<endl;
    while (fgets(content[count], 128, tFILE)) {
        if ((point = strstr(content[count], "%server-proto%")) != NULL) {
            QString lowerServerProto = server_proto.toLower();
            sprintf(content[count], "proto %s\n", qstringToChar(lowerServerProto));
        } else if ((point = strstr(content[count], "%server-ip%")) != NULL) {
            QString tmp = server_ip + " " + server_port;
            sprintf(content[count], "remote %s\n", qstringToChar(tmp));
        } else if ((point = strstr(content[count], "auth-user-pass")) != NULL) {
#if defined(Q_OS_WIN)
            char new_pwdfile[255] = {0};
			replaceStr(new_pwdfile, passwdFile, "\\", "\\\\");
            sprintf(content[count], "auth-user-pass %s\n", new_pwdfile);
#else
            sprintf(content[count], "auth-user-pass %s\n", passwdFile);
#endif
        }else if ((point = strstr(content[count], "<ca>")) != NULL) {
            ca ++;
#if defined(Q_OS_WIN)
            char new_cafile[255] = {0};
			replaceStr(new_cafile, cafile, "\\", "\\\\");
            sprintf(content[count], "ca %s\n", new_cafile);
#else
            sprintf(content[count], "ca %s\n", cafile);
#endif
        } else if ((point = strstr(content[count], "</ca>")) != NULL) {
            break;
        }

        if (ca > 1) {
            fputs(content[count], cFILE);
        }
        if (ca > 0) {
            ca ++;
        }
        count ++;
    }
    fclose(tFILE);
    fclose(cFILE);

    tFILE = fopen(templateFile, "w");
    for (; i < count - ca + 2; i ++) {
        fputs(content[i], tFILE);
    }

    fclose(tFILE);
}
