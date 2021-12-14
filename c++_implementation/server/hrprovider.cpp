#include "hrprovider.h"

HRProvider::HRProvider(QObject *parent)
    : Provider(parent)
{
    qDebug() << "HRProvider creation";

}

void HRProvider::floatReady(double num)
{

    QByteArray buf = QByteArray::number(num);

    emit writeDatagram(0, buf);
}
