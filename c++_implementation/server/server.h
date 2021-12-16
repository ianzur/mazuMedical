#ifndef SERVER_H
#define SERVER_H

#include <QString>
#include <QObject>
#include <QVector>
#include <QList>
#include <QDebug>

#include "processorthread.h"

QT_BEGIN_NAMESPACE
class QSctpServer;
class QSctpSocket;
QT_END_NAMESPACE

class Provider;

class Server : public QObject
{
    Q_OBJECT
public:
    explicit Server(QString portno, QObject *parent = nullptr);
    virtual ~Server();

signals:

private slots:
    void newConnection();
    void clientDisconnected();
    void readDatagram(int channel);
    void writeDatagram(QSctpSocket *to, const QByteArray &ba);

private:
    QVector<Provider *> providers;
    QSctpServer* sctpServer;
    QList<QSctpSocket *> connections;

    ProcessorThread processor;

};

#endif // SERVER_H
