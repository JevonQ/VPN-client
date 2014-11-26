#ifndef STATUSWINDOW_H
#define STATUSWINDOW_H

#include <QMainWindow>
#include <QtGui>
#include <QFutureWatcher>
#include <QLabel>
#include "utils.h"
#include "settings.h"
#include "login.h"
#include "location.h"
#include "logdialog.h"
#include "server.h"

class QLabel;
class QPushButton;
class QIcon;

typedef enum vpn_status_e {
    VPN_DISCONNECT = 0,
    VPN_CONNECTED,
    VPN_CONNECTING,
} vpnstatus;

class LLabel: public QLabel
{
    Q_OBJECT
public:
    explicit LLabel(QWidget *parent=0): QLabel(parent)
    {
    }

protected:   
    void mouseReleaseEvent( QMouseEvent* ); 
    void enterEvent(QEvent *e);

signals:
    void clicked();
};

class StatusWindow: public QMainWindow
{
    Q_OBJECT

    public:
        StatusWindow(Settings *s, LoginDlg *d, Location *l, LogDialog *o, QWidget* parent = 0);
        virtual ~StatusWindow();
        void sendCloseOpenvpnSignal();

    public slots:
        void mainWindowShow();
        void openvpnConnect();
        void beginOpenvpnThread();
        void endOpenvpnThread();
        void sReturnMain();
        void lReturnMain();
        void logReturnMain();
        void checkOpenvpnStatus(const QString &s);

    private slots:
        void tryConnect(QString &p, QString &o, QString &l);
        void openSettings();
        void openLocation();
        void timerCheckOpenvpn();

    signals:
        void addLog(const QString &log);
        void openvpnStatusChangedSignal(const int &);
        void showBalloon(const int &type, const QString &t, const QString &m);

    private:
        QIcon       *icon;
        QLabel      *titleImg;
        QLabel      *cStatus;
        QLabel      *username;
        QLabel      *email;
        QLabel      *package;
        QLabel      *activeTime;
        LLabel      *locationImg;
        QLabel      *countryNode;
        QLabel      *nodeStatus;
        QLabel      *countryImg;
        LLabel      *ipImg;
        QLabel      *newIp;
        QLabel      *oldIp;
        QLabel      *newIpInfo;
        QLabel      *oldIpInfo;
        LLabel      *protocolImg;
        QLabel      *protocolInfo;
        QLabel      *protocol;
        QLabel      *encryptionInfo;
        QLabel      *encryption;
        QPushButton *connectBtn;
        QPushButton *jumperBtn;
        QPushButton *settingBtn;

        Settings *mainSettings;
        LoginDlg *mainLoginDlg;
        Location *mainLocation;
        LogDialog *mainLog;
        QFutureWatcher<void> *watcher;
        ServerThread *serverThread;

        bool locationReady;
        bool protocolReady;
        QString protocolStr;
        QString portStr;
        QString locationStr;
        QString ipStr;
        int serverCount;
        Server *serverList;
        char serverListFile[255];

        // VPN connection related
        vpnstatus openvpnStatus;
        
        // VPN management socket related
        int     managementSocket;
        int     clientSocket[10]; // max client fds
        int     openvpnManagementPort;
        QString openvpnManagementIP;

        // VPN status check timer;
        QTimer *statusTimer;
};

void runServer(int socket, char *clientFD);
char * qstringToChar(QString &s);
void doConnect(QString &server_proto, QString &server_port, QString &server_ip, QString &managementSocketIp, int managementSocketPort);
void processTemplateFile(char *templateFile, char *cafile, char *passwdFile, QString &server_proto, QString &server_ip, QString &server_port);
#endif //STATUSWINDOW_H
