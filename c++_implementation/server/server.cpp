#include <QtNetwork>
//#include "/home/raphy/mazupi2/sysroot/usr/include/pigpiod_if2.h"

#include "server.h"
#include "frameprovider.h"
#include "hrprovider.h"
#include "fpsprovider.h"
#include "infoprovider.h"

enum SctpChannels {
    Frames = 0, //frames
    FPS = 1,
    HR = 2, //hr
    INFO = 3, //send string describing current action

    NumberOfChannels = 4 //TODO add info channel
};

Server::Server(char* portno, QObject *parent)
    : QObject(parent)
    , providers(SctpChannels::NumberOfChannels)
{
    sctpServer = new QSctpServer(this);
    sctpServer->setMaximumChannelCount(NumberOfChannels);

    //sender = processor
    //reciever = providers
    providers[SctpChannels::Frames] = new FrameProvider(this);
    providers[SctpChannels::FPS] = new FPSProvider(this);
    providers[SctpChannels::HR] = new HRProvider(this);
    providers[SctpChannels::INFO] = new INFOProvider(this);

    connect(sctpServer, &QSctpServer::newConnection, this, &Server::newConnection);

    qRegisterMetaType< cv::Mat >("cv::Mat&");
    qRegisterMetaType<std::string>("std::string");
    connect(&processor, SIGNAL(outDisplay(cv::Mat&)), providers[SctpChannels::Frames], SLOT(frameReady(cv::Mat&)));
    connect(&processor, SIGNAL(outFPS(double)), providers[SctpChannels::FPS], SLOT(floatReady(double)));
    connect(&processor, SIGNAL(outHR(double)), providers[SctpChannels::HR], SLOT(floatReady(double)));
    connect(&processor, SIGNAL(outINFO(std::string)), providers[SctpChannels::INFO], SLOT(strReady(std::string)));

    
//    connect(&roi, SIGNAL(outROI), providers[SctpChannels::INFO], slot(strReady(string))));
    
    connect(providers[SctpChannels::Frames], &Provider::writeDatagram, this, &Server::writeDatagram);
    connect(providers[SctpChannels::FPS], &Provider::writeDatagram, this, &Server::writeDatagram);
    connect(providers[SctpChannels::HR], &Provider::writeDatagram, this, &Server::writeDatagram);
    connect(providers[SctpChannels::INFO], &Provider::writeDatagram, this, &Server::writeDatagram);


    if(!sctpServer->listen(QHostAddress::Any,atoi(portno)))
    {
        qDebug() << "ERROR: " << sctpServer->serverError();
    }
    else
    {
        qDebug() << "Server started, " << sctpServer->serverAddress() << " listening to port" << sctpServer->serverPort();
    }
}

Server::~Server()
{
    //delete all connections
    qDeleteAll(connections.begin(), connections.end());

    //kill processor thread
    processor.requestInterruption();
    processor.quit();
}

void Server::newConnection()
{
    qDebug() << "connected to client";

    QSctpSocket *connection = sctpServer->nextPendingDatagramConnection();

    connections.append(connection);
    connect(connection, &QSctpSocket::channelReadyRead, this, &Server::readDatagram);
    connect(connection, &QSctpSocket::disconnected, this, &Server::clientDisconnected);


    //roi.start();
    

    //processor.update(roi);


    processor.start();

    for (Provider *provider : providers)
        provider->newConnection(*connection);
}

void Server::clientDisconnected()
{
    QSctpSocket *connection = static_cast<QSctpSocket *>(sender());

    connections.removeOne(connection);
    connection->disconnect();

    for (Provider *provider : providers)
        provider->clientDisconnected(*connection);

    connection->deleteLater();

    processor.requestInterruption();
    processor.quit();
}

void Server::readDatagram(int channel)
{
    QSctpSocket *connection = static_cast<QSctpSocket *>(sender());

    connection->setCurrentReadChannel(channel);
    providers[channel]->readDatagram(*connection, connection->readDatagram().data());
}

void Server::writeDatagram(QSctpSocket *to, const QByteArray &ba)
{
    int channel = providers.indexOf(static_cast<Provider *>(sender()));

    if (to) {
        to->setCurrentWriteChannel(channel);
        to->writeDatagram(ba);
        return;
    }

    for (QSctpSocket *connection : connections) {
//        qDebug() << connection->readChannelCount();
        connection->setCurrentWriteChannel(channel);
        connection->writeDatagram(ba);
    }
}
