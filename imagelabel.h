#ifndef IMAGELABEL_H
#define IMAGELABEL_H

#include <QLabel>
class ImageLabel :public QLabel
{
    Q_OBJECT
public:
    explicit ImageLabel(const QString &s1, const QString &s2, QWidget *parent = 0);
    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);

signals:
    void clicked();

protected:
    void mouseReleaseEvent( QMouseEvent* );

private:
    QString img1;
    QString img2;
};
#endif
