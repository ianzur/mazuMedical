#ifndef PROVIDER_H
#define PROVIDER_H

#include <QDebug>
#include <QObject>

#include <string>
#include <regex>

#include "processorthread.h"

QT_BEGIN_NAMESPACE
class QSctpSocket;
class QByteArray;
QT_END_NAMESPACE

class Provider : public QObject
{
    Q_OBJECT

public:
    explicit inline Provider(QObject *parent = nullptr) : QObject(parent) { }

    virtual void readDatagram(QSctpSocket &, const QByteArray &) { }
    virtual void newConnection(QSctpSocket &) { }
    virtual void clientDisconnected(QSctpSocket &) { }

    const int jpgQual = 85;

signals:
    void writeDatagram(QSctpSocket *to, const QByteArray &ba);

public slots:
    virtual void frameReady(cv::Mat&) { }
    virtual void floatReady(double) { }
    virtual void strReady(std::string) { }

private:

};

#endif // PROVIDER_H
