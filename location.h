#ifndef LOCATION_H
#define LOCATION_H 

#include <QDialog>
#include "imagelabel.h"
#include "utils.h"

class QLabel;
class QPushButton;
class QIcon;
class QComboBox;

class Location: public QDialog
{
    Q_OBJECT
public:
    Location();
    virtual ~Location();

public slots:
    void showLocationAfterLogin();

signals:
    void returnMain();
    void settingsDialgShow();
    void returnMainWithLocation(QString &protocol, QString &port, QString &country);
    void addLog(const QString &l);
    void showBalloon(const int &type, const QString &t, const QString &m);

protected:

private slots:
    void protocolChanged(const QString &text);
    void countryChanged(const QString &text);
    void returnMainWindow();
    void connectClickAction();
    void settingClickAction();

private:
    ImageLabel *mainImg;
    QLabel *label1;
    QLabel *label2;
    QLabel *mapLabel;
    QComboBox *protocolCom;
    QComboBox *country;
    QPushButton *connectButton;
    QPushButton *settingButton;
    QIcon *icon;
    bool locationReady;
    bool protocolReady;
    QString location;
    QString protocol;
    QString port;
    Server *serverList;
    Server *countryList;
    char serverListFile[255];
};

#endif
