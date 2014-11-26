#ifndef ABOUT_H
#define ABOUT_H

#include <QDialog>
#include <QLabel>

class QAction;
class QPushButton;
class QIcon;

class Label: public QLabel
{
    Q_OBJECT
public:
    explicit Label(QWidget *parent=0): QLabel(parent)
    {
    }

protected:   
    void mouseReleaseEvent( QMouseEvent* ); 
    void enterEvent(QEvent *e);

signals:
    void clicked();
};

class About: public QDialog
{
    Q_OBJECT
public:
    About();

public slots:
    void showAboutDialog();
    void openProxyLink();
    void openQTLink();
    void openOpenvpnLink();

protected:
    void closeEvent(QCloseEvent *event);

private slots:

private:
    Label *label1;
    Label *label2;
    Label *label3;
    Label *bodyLabel;
    QPushButton *closeButton;
    QIcon *icon; 
};

#endif
