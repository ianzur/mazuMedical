#include "fpsprovider.h"

FPSProvider::FPSProvider(QObject *parent)
    : Provider(parent)
{
    qDebug() << "fps creation";
}

void FPSProvider::floatReady(double num)
{
    QByteArray buf = QByteArray::number(num);

    emit writeDatagram(0, buf);
}
