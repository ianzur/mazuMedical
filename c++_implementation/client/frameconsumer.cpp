#include "frameconsumer.h"
#include <QMovie>
#include <QLabel>
#include <QDataStream>
#include <QImage>
#include <QPixmap>


FrameConsumer::FrameConsumer(QObject *parent)
    : Consumer(parent)
{
//    qDebug() << "frame consumer created";
    label = new QLabel;
    label->setFrameStyle(QFrame::Box | QFrame::Raised);
    label->setFixedSize(480 + label->frameWidth() * 2, 360 + label->frameWidth() * 2);
    label->setAlignment(Qt::AlignCenter);

    movie = new QMovie(this);
    movie->setCacheMode(QMovie::CacheAll);
    movie->setFileName(QLatin1String("NoCameraGif.gif"));
    connect(movie, &QMovie::frameChanged, this, &FrameConsumer::serverDisconnected);


//    qDebug() << movie->isValid() << movie->lastErrorString();

    serverDisconnected();
}

QWidget *FrameConsumer::widget()
{
    return label;
}

void FrameConsumer::connectionMade()
{
//    qDebug() << "connection made (stop movie)";

    movie->stop();

}

void FrameConsumer::readDatagram(const QByteArray &ba)
{
    QDataStream ds(ba);
    QPixmap pix;

//    qDebug() << ba.size();

    pix.loadFromData(ba, "JPG");

    label->setPixmap(pix);
}

void FrameConsumer::serverDisconnected()
{
    movie->start();
    label->setPixmap(QPixmap::fromImage(movie->currentImage()));
}
