#include <QtGui>
#include "about.h"

void Label::mouseReleaseEvent(QMouseEvent *e)
{
    emit clicked();
    QLabel::mouseReleaseEvent(e);
}

void Label::enterEvent(QEvent *e)
{
    setCursor(Qt::PointingHandCursor);
    QLabel::enterEvent(e);
}


About::About()
{
    label1 = new Label(this);
    label2 = new Label(this);
    label3 = new Label(this);
    bodyLabel = new Label(this);
    closeButton = new QPushButton(this);
    bodyLabel->setTextFormat(Qt::PlainText);
    bodyLabel->setText("Safejumper, by proxy.sh - version 2.0\nMore infomation @ https://proxy.sh");
    closeButton->setText(QString::fromUtf8("OK"));

    label1->setGeometry(QRect(20, 20, 80, 80));
    label1->setStyleSheet("border-image:url(:/images/img_safejumper.jpg)");
    label2->setGeometry(QRect(120, 20, 80, 80));
    label2->setStyleSheet("border-image:url(:/images/img_openvpn.jpg)");
    label3->setGeometry(QRect(220, 20, 80, 80));
    label3->setStyleSheet("border-image:url(:/images/img_qt.jpg)");
    bodyLabel->setStyleSheet("color:blue");

    bodyLabel->setGeometry(QRect(20, 150, 280, 30));
    closeButton->setGeometry(QRect(220, 200, 80, 20));

    connect(label1, SIGNAL(clicked()), this, SLOT(openProxyLink()));
    connect(label2, SIGNAL(clicked()), this, SLOT(openOpenvpnLink()));
    connect(label3, SIGNAL(clicked()), this, SLOT(openQTLink()));
    connect(bodyLabel, SIGNAL(clicked()), this, SLOT(openProxyLink()));
    connect(closeButton, SIGNAL(clicked()), this, SLOT(hide()));

    icon = new QIcon(":/images/vc.ico");
    setWindowIcon(*icon);
    setWindowTitle(tr("Safejumper"));
    resize(320, 250);
    setMinimumSize(QSize(320, 250));
    setMaximumSize(QSize(320, 250));
    printf("make 'about' dialog\n");
}

void About::showAboutDialog()
{
    printf("show about dialog\n");
}

void About::openProxyLink()
{
    QDesktopServices::openUrl(QUrl(QLatin1String("https://proxy.sh/")));
}

void About::openOpenvpnLink()
{
    QDesktopServices::openUrl(QUrl(QLatin1String("http://openvpn.net/")));
}

void About::openQTLink()
{
    QDesktopServices::openUrl(QUrl(QLatin1String("http://qt-project.org/")));
}

void About::closeEvent(QCloseEvent *event)
{
    (void) event;
}
