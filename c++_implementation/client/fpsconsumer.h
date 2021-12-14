#ifndef FPSCONSUMER_H
#define FPSCONSUMER_H

#include "consumer.h"

QT_BEGIN_NAMESPACE
class QLCDNumber;
QT_END_NAMESPACE

class FPSConsumer : public Consumer
{
    Q_OBJECT
public:
    explicit FPSConsumer(QObject *parent = nullptr);

    QWidget *widget() Q_DECL_OVERRIDE;
    void readDatagram(const QByteArray &ba) Q_DECL_OVERRIDE;
    void serverDisconnected() Q_DECL_OVERRIDE;

private:
    QLCDNumber *label;
    double fps;

};

#endif // FPSCONSUMER_H
