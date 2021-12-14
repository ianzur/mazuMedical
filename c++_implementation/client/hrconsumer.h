#ifndef HRCONSUMER_H
#define HRCONSUMER_H

#include "consumer.h"

QT_BEGIN_NAMESPACE
class QLCDNumber;
QT_END_NAMESPACE

class HRConsumer : public Consumer
{
    Q_OBJECT
public:
    explicit HRConsumer(QObject *parent = nullptr);

    QWidget *widget() Q_DECL_OVERRIDE;
    void readDatagram(const QByteArray &ba) Q_DECL_OVERRIDE;
    void serverDisconnected() Q_DECL_OVERRIDE;

private:
    QLCDNumber *label;
    double hr;

};

#endif // HRCONSUMER_H
