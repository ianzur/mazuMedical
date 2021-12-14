#include "fpsconsumer.h"
#include <QLCDNumber>
#include <QString>
#include <QDataStream>

FPSConsumer::FPSConsumer(QObject *parent)
    : Consumer(parent)
{
//    qDebug() << "fps created";
    label = new QLCDNumber(4);
    label->setSmallDecimalPoint(true);
    label->setMinimumSize(125,75);
    label->setMaximumSize(125,75);
    label->setStyleSheet("font: 8pt;");

    serverDisconnected();
}

QWidget *FPSConsumer::widget()
{
    return label;
}

void FPSConsumer::readDatagram(const QByteArray &ba)
{
    label->display(ba.toDouble());
}

void FPSConsumer::serverDisconnected()
{
    label->display(QLatin1String("----"));
}
