#include <QLabel>
#include "imagelabel.h"

ImageLabel::ImageLabel(const QString &s1, const QString &s2, QWidget *parent) :QLabel(parent) 
{
    img1 = s1;
    img2 = s2;
    this->setStyleSheet("border-image: url("+img1+")");
}

void ImageLabel::enterEvent(QEvent * e)
{
    this->setStyleSheet("border-image: url("+img2+")");
    this->setGeometry(QRect(10, 10, 99, 80));
    QLabel::enterEvent(e);
}

void ImageLabel::leaveEvent(QEvent *e)
{
    this->setStyleSheet("border-image: url("+img1+")");
    this->setGeometry(QRect(19, 10, 80, 80));
    QLabel::enterEvent(e);
}

void ImageLabel::mouseReleaseEvent(QMouseEvent *e)
{
    emit clicked();
    QLabel::mouseReleaseEvent(e);
}
