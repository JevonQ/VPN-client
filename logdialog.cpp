#include <QtGui>
#include "logdialog.h"
#include "utils.h"

LogDialog::LogDialog()
{
    mainImg = new ImageLabel(":/images/main1.png", ":/images/main2.png", this);
    connect(mainImg, SIGNAL(clicked()), this, SLOT(returnMainWindow()));
    copyButton = new QPushButton(this);
    contactButton= new QPushButton(this);
    logArea = new QTextEdit(this);

    // set UI
    mainImg->setGeometry(QRect(19, 10, 80, 80));
    copyButton->setGeometry(QRect(201, 50, 189, 30));
    copyButton->setStyleSheet("border-image: url(:/images/log-copybutton.jpg)");
    logArea->setGeometry(QRect(10, 105, 380, 160));
    logArea->setReadOnly(true);
    contactButton->setGeometry(QRect(78, 290, 245, 45));
    contactButton->setStyleSheet("border-image: url(:/images/log-contactbutton.jpg)");
    this->setStyleSheet("QDialog{border-image: url(:/images/log-bg.png)}");

    connect(copyButton, SIGNAL(clicked()), this, SLOT(copyButtonAction()));
    connect(contactButton, SIGNAL(clicked()), this, SLOT(contactButtonAction()));

    icon = new QIcon(":/images/vc.ico");
    setWindowIcon(*icon);
    resize(400, 350);
    setMinimumSize(QSize(400, 350));
    setMaximumSize(QSize(400, 350));
    setWindowTitle(tr("Safejumper"));
}

LogDialog::~LogDialog()
{
    delete label1;
    delete copyButton;
    delete contactButton;
    delete logArea;
    delete mainImg;
    delete icon;
}

void LogDialog::showLogDialog()
{
    logArea->setText(log);
    this->show();
}

void LogDialog::addLogEntry(const QString &e)
{
    log += e;
}

void LogDialog::returnMainWindow()
{
    emit returnMain();
}

void LogDialog::copyButtonAction()
{
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(logArea->toPlainText());
}

void LogDialog::contactButtonAction()
{
    QDesktopServices::openUrl(QUrl(QLatin1String("http://blog.const.net.cn")));
}

