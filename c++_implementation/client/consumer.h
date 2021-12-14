#ifndef CONSUMER_H
#define CONSUMER_H

#include <QDebug>
#include <QObject>
#include <QByteArray>

QT_BEGIN_NAMESPACE
class QWidget;
QT_END_NAMESPACE

class Consumer : public QObject
{
    Q_OBJECT
public:
    explicit inline Consumer(QObject *parent = nullptr) : QObject(parent) { }

    virtual QWidget *widget() = 0;
    virtual void readDatagram(const QByteArray &ba) = 0;
    virtual void serverDisconnected() { }
    virtual void connectionMade() { }
    virtual void displayInfo(QString) { }


signals:
    void writeDatagram(const QByteArray &ba);
};

#endif // CONSUMER_H
