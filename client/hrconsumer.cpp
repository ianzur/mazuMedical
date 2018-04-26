#include "hrconsumer.h"
#include <QLCDNumber>
#include <QString>
#include <QDataStream>

HRConsumer::HRConsumer(QObject *parent)
    : Consumer(parent)
{
//    qDebug() << "hr consumer created";
    label = new QLCDNumber(4);
    label->setSmallDecimalPoint(true);
    label->setMinimumSize(125,75);
    label->setMaximumSize(125,75);
    label->setStyleSheet("font: 8pt;");

    serverDisconnected();
}

QWidget *HRConsumer::widget()
{
    return label;
}

void HRConsumer::readDatagram(const QByteArray &ba)
{
    label->display(ba.toDouble());
}

void HRConsumer::serverDisconnected()
{
    label->display(QLatin1String("----"));
}
