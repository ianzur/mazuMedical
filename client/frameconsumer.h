#ifndef FRAMECONSUMER_H
#define FRAMECONSUMER_H

#include "consumer.h"

QT_BEGIN_NAMESPACE
class QLabel;
class QMovie;
QT_END_NAMESPACE

class FrameConsumer : public Consumer
{
    Q_OBJECT
public:
    explicit FrameConsumer(QObject *parent = nullptr);

    QWidget *widget() Q_DECL_OVERRIDE;
    void readDatagram(const QByteArray &ba) Q_DECL_OVERRIDE;
    void serverDisconnected() Q_DECL_OVERRIDE;
    void connectionMade() Q_DECL_OVERRIDE;

private:
    QLabel *label;
    QMovie *movie;

};

#endif // FRAMECONSUMER_H
