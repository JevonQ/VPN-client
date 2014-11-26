#include <QtGui>
#include "login.h"
#include "utils.h"

LoginDlg::LoginDlg(QWidget* parent) :QDialog(parent)
{
    img = new QLabel();
    usrLineEdit = new QLineEdit(this);
    usrLineEdit->setGeometry(QRect(20, 95, 260, 30));
    pwdLineEdit = new QLineEdit(this);
    pwdLineEdit->setGeometry(QRect(20, 153, 260, 30));
    pwdLineEdit->setEchoMode(QLineEdit::Password);
    connectBtn = new QPushButton(this);
    connectBtn->setGeometry(QRect(20, 210, 100, 40));

    remember = new QCheckBox(tr("REMEMBER ME"), this);
    remember->setChecked(true);
    remember->setGeometry(QRect(145, 212, 110, 30));

    QLabel *u_label = new QLabel(this);
    u_label->setGeometry(QRect(252, 103, 15, 15));
    QLabel *p_label = new QLabel(this);
    p_label->setGeometry(QRect(252, 161, 14, 14));

    connect(connectBtn, SIGNAL(clicked()), this, SLOT(accept()));

    (void) memset(accountInfoFile, '\0', sizeof (accountInfoFile));
#if defined(Q_OS_WIN)
    sprintf(accountInfoFile, "%s\\safejumper\\accout.xml", getenv("LOCALAPPDATA"));
#else
    sprintf(accountInfoFile, "%s/.safejumper/accout.xml", getenv("HOME"));
#endif

    FILE *accountfile = NULL;
    char username1[32] = {0};
    char password1[32] = {0};
    int flag = 0;
    printf("Check the account xml file...\n");
    if ((accountfile = fopen(accountInfoFile, "r")) != NULL) {
        printf("Find the account xml file!\n");
        getAccountInfo(accountInfoFile, username1, password1, &flag); 
        printf("Get the account info from xml file!\n");
        fclose(accountfile);
        usrLineEdit->setText(username1);
        pwdLineEdit->setText(password1);
    }

    icon = new QIcon(":/images/vc.ico");
    setWindowIcon(*icon);
    setWindowTitle(tr("Safejumper"));
    resize(300, 260);
    this->setStyleSheet("QDialog{border-image: url(:/images/login-bg.png);}"
        "QPushButton{border-image: url(:/images/login-btn.jpg);}"
        "QLineEdit{border-width:2px;}"
        "QCheckBox::indicator{width: 15px; height: 15px;}"
        "QCheckBox::indicator:unchecked{image: url(:/images/login-checkbox-unselected.jpg);}"
        "QCheckBox::indicator:checked {image: url(:/images/login-checkbox-selected.jpg);}"
        );
    u_label->setStyleSheet("border-image: url(:/images/login-u.jpg);");
    p_label->setStyleSheet("border-image: url(:/images/login-p.jpg);");

    /*
     * set the login status to false
     * modify this to true if the login is successful
     */
    loginStatus = false;
    printf("Finish Login dialog init\n");
}
 
LoginDlg::~LoginDlg()
{
    delete usrLineEdit;
    delete pwdLineEdit;
    delete username;
    delete passwd;
    delete connectBtn;
    delete remember;
    delete icon;
    delete watcher;
}

bool LoginDlg::getLoginStatus()
{
    return loginStatus;
}

void LoginDlg::openDialog()
{
    show();
    usrLineEdit->setFocus();

    if (loginStatus) {
        emit loginSuccess();
    }
}

void downloadServerFile(QString fileString, QString linkString)
{
    QByteArray f = fileString.toLatin1();
    QByteArray l = linkString.toLatin1();

    char file[128] = {0};  
    char link[256] = {0};
	strcpy(file, f.data());
	strcpy(link, l.data());
    
    (void) getServerListFile(file, link);
}

void LoginDlg::verifyAccountInfo(QString username, QString password)
{
    QString serverListFile;
    QString link;
#if defined(Q_OS_WIN)
    QString homeDir = QString(QLatin1String(getenv("LOCALAPPDATA")));
    serverListFile = homeDir + "\\safejumper\\server.xml";
#else
    QString homeDir = QString(QLatin1String(getenv("HOME")));
    serverListFile = homeDir + "/.safejumper/server.xml";
#endif

    link = "https://proxy.sh/access.php?u=" + username + "&p=" + password + "&hub=ture";

    watcher = new QFutureWatcher<void>();
    qDebug()<<serverListFile<<endl;
    qDebug()<<link<<endl;
    connect(watcher, SIGNAL(started()), this, SLOT(beginVerify()));
    connect(watcher, SIGNAL(finished()), this, SLOT(endVerify()));
    QFuture<void> future =
        QtConcurrent::run(downloadServerFile, serverListFile, link);
    watcher->setFuture(future);
}
 
/*
 * rewrite accept() function
 */
void LoginDlg::accept()
{
    printf("Function: accept\n");
    verifyAccountInfo(usrLineEdit->text().trimmed(), pwdLineEdit->text());
}

void LoginDlg::beginVerify()
{
    printf("begin to verify the account\n");
    qDebug()<<"begin to verify"<<endl;
    usrLineEdit->setEnabled(0);
    pwdLineEdit->setEnabled(0);
    connectBtn->setEnabled(0);
    remember->setEnabled(0);
}

char * qstringToChar(const QString &s)
{
    QByteArray t = s.toLatin1();
    return (char *)(t.data());
}

void LoginDlg::endVerify()
{
    char serverListFile[255];
    char homeDir[64];
	char username[64] = {0};
	char passwd[64] = {0};
    (void) memset(serverListFile, '\0', sizeof(serverListFile));
    (void) memset(homeDir, '\0', sizeof(homeDir));

#if defined(Q_OS_WIN)
    (void) strcpy(homeDir, getenv("LOCALAPPDATA"));
    (void) sprintf(serverListFile, "%s\\safejumper\\server.xml", homeDir);
#else
    (void) strcpy(homeDir, getenv("HOME"));
    (void) sprintf(serverListFile, "%s/.safejumper/server.xml", homeDir);
#endif

    if (verifyAccount(serverListFile) == true) {
        remove(accountInfoFile);
        if (remember->isChecked()) {
            if ((usrLineEdit->text().trimmed().length() != 0) && (pwdLineEdit->text().trimmed().length() != 0)) {
				strcpy(username, qstringToChar(usrLineEdit->text().trimmed()));
				strcpy(passwd, qstringToChar(pwdLineEdit->text().trimmed()));
                saveAccountInfo(accountInfoFile, username, passwd, 1);
            }
        }

        // XXX for easy handle
        char passwdFile[255] = {0};
#if defined(Q_OS_WIN)
		sprintf(passwdFile, "%s\\safejumper\\passwd.txt", homeDir);
#else
        sprintf(passwdFile, "%s/.safejumper/passwd.txt", homeDir);
#endif
        FILE *f = fopen(passwdFile, "wb");
        if (f != NULL) {
            fputs(qstringToChar(usrLineEdit->text().trimmed()), f);
            fputs("\n", f);
            fputs(qstringToChar(pwdLineEdit->text().trimmed()), f);
            fclose(f);
        }

        loginStatus = true;
        QDialog::accept();
        emit addLog("Success to login the vpn client\n");
        emit loginSuccess();
    } else {
        loginStatus = false;
        emit addLog("Fail to login the vpn client, username or password is not correct\n");
        emit showBalloon(2, "Proxy.sh vpn client", "Password verification Failed.");
        QMessageBox::warning( this, "Warning!" , "<font size = 5 color = black> Username or Password is not correct!</font> " );
        usrLineEdit->setFocus();
    }
    printf("end to verify the account\n");
    usrLineEdit->setEnabled(1);
    pwdLineEdit->setEnabled(1);
    connectBtn->setEnabled(1);
    remember->setEnabled(1);
}
