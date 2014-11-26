#ifndef LOGDIALOG_H
#define LOGDIALOG_H 

#include <QDialog>
#include "imagelabel.h"

class QLabel;
class QPushButton;
class QIcon;
class QTextEdit;

class LogDialog: public QDialog
{
    Q_OBJECT
public:
    LogDialog();
    virtual ~LogDialog();

public slots:
    void showLogDialog();
    void addLogEntry(const QString &e);

signals:
    void returnMain();
    void addLog(const QString &l);
    void showBalloon(const int &type, const QString &t, const QString &m);

private slots:
    void returnMainWindow();
    void copyButtonAction();
    void contactButtonAction();

private:
    ImageLabel *mainImg;
    QLabel *label1;
    QPushButton *copyButton;
    QPushButton *contactButton;
    QTextEdit *logArea;
    QIcon *icon;

    QString log;
};

#endif
