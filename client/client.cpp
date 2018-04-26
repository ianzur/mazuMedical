
#include <QtWidgets>

#include "client.h"
#include "frameconsumer.h"
#include "hrconsumer.h"
#include "fpsconsumer.h"
#include "infoconsumer.h"


enum SctpChannels {
    Frames = 0, //frames
    FPS = 1, //frames per second
    HR = 2, //estimated HR
    INFO = 3, //what is currently happening

    NumberOfChannels = 4
};

Client::Client(QWidget *parent)
    : QDialog(parent)
    , consumers(SctpChannels::NumberOfChannels)
{

    setWindowTitle(tr("Vigilant Monitor"));

    sctpSocket = new QSctpSocket(this);

    //host address
    QLabel *hostLabel = new QLabel(tr("Addr:"));
    hostLabel->setStyleSheet("font: 8pt;");
    QRegExp rx("\\d{1,3}.\\d{1,3}.\\d{1,3}.\\d{1,3}");
    hostComboBox = new QComboBox;
    hostComboBox->setEditable(true);
    hostComboBox->addItem("192.168.1.10");

    hostComboBox->addItem("10.125.221.190");
    hostComboBox->addItem("10.0.0.113");
    hostComboBox->addItem("127.0.0.1");
    hostComboBox->setStyleSheet("font: 8pt;");
    hostLabel->setBuddy(hostComboBox);

    //port number
    QLabel *portLabel = new QLabel(tr("Port:"));
    portLabel->setStyleSheet("font: 8pt;");
    portLineEdit = new QLineEdit;
    portLineEdit->setMaximumWidth(100);
    portLineEdit->setValidator(new QIntValidator(1, 65535, this));
    portLineEdit->setText("8989");
    portLineEdit->setStyleSheet("font: 8pt;");
    portLabel->setBuddy(portLineEdit);

    connectButton = new QPushButton(tr("Connect"));
    connectButton->setFixedSize(260,50);
    connectButton->setCheckable(true);
    connectButton->setStyleSheet("font: 8pt;");

    consumers[SctpChannels::Frames] = new FrameConsumer(this);
    QLabel *fpsLabel = new QLabel(tr("FPS:"));
    fpsLabel->setStyleSheet("font: 8pt;");
    consumers[SctpChannels::FPS] = new FPSConsumer(this);
    QLabel *hrLabel = new QLabel(tr("HR:"));
    hrLabel->setStyleSheet("font: 8pt;");
    consumers[SctpChannels::HR] = new HRConsumer(this);
    consumers[SctpChannels::INFO] = new InfoConsumer(this);

    connect(portLineEdit, &QLineEdit::textChanged, this, &Client::enableConnectButton);
    connect(connectButton, &QPushButton::clicked, this, &Client::connectPressed);
    connect(sctpSocket, &QSctpSocket::connected, this, &Client::connected);
    connect(sctpSocket, &QSctpSocket::disconnected, this, &Client::disconnected);
    connect(sctpSocket, &QSctpSocket::channelReadyRead, this, &Client::readDatagram);
    connect(consumers[SctpChannels::HR], &Consumer::writeDatagram, this, &Client::writeDatagram);
    connect(consumers[SctpChannels::FPS], &Consumer::writeDatagram, this, &Client::writeDatagram);
    connect(consumers[SctpChannels::INFO], &Consumer::writeDatagram, this, &Client::writeDatagram);


    QGridLayout *mainLay = new QGridLayout;
    QGridLayout *topLay = new QGridLayout;
    topLay->addWidget(hostLabel, 0, 0, 1, 1, Qt::AlignJustify);
    topLay->addWidget(hostComboBox, 0, 1, 1, 1, Qt::AlignJustify);
    topLay->addWidget(portLabel, 0, 2, 1, 1, Qt::AlignJustify);
    topLay->addWidget(portLineEdit, 0, 4, 1, 1, Qt::AlignJustify);
    mainLay->addWidget(connectButton, 0, 3, 1, 1, Qt::AlignJustify);
    mainLay->addLayout(topLay, 0, 0, 1, 2, Qt::AlignJustify);
    mainLay->addWidget(consumers[SctpChannels::Frames]->widget(), 1, 0, 4, 2, Qt::AlignJustify);

    QGridLayout *infoLay = new QGridLayout;
    infoLay->addWidget(hrLabel, 0, 0, Qt::AlignRight);
    infoLay->addWidget(consumers[SctpChannels::HR]->widget(), 0, 1, Qt::AlignJustify);
    infoLay->addWidget(fpsLabel, 1, 0, Qt::AlignRight);
    infoLay->addWidget(consumers[SctpChannels::FPS]->widget(), 1, 1, Qt::AlignJustify);
//    mainLay->setColumnMinimumWidth(2,260);
    mainLay->setRowMinimumHeight(240, 2);
    mainLay->addLayout(infoLay, 1, 3, Qt::AlignJustify);
    mainLay->addWidget(consumers[SctpChannels::INFO]->widget(), 2, 3, Qt::AlignJustify);


    setLayout(mainLay);

}

Client::~Client()
{

    //kill alarm stuff
    system("killall -v mpg123");
    system("killall alarm");
    system("./aK");

    delete sctpSocket;
}

void Client::connected()
{
    consumers[SctpChannels::INFO]->displayInfo("Connection Successful!");


    for (Consumer *consumer : consumers)
        consumer->connectionMade();

    connectButton->setText("Disconnect");
    connectButton->setEnabled(true);
}

void Client::disconnected()
{
    for (Consumer *consumer : consumers)
        consumer->serverDisconnected();

    sctpSocket->close();

    connectButton->setText("Connect");
    connectButton->setEnabled(true);

}

void Client::connectPressed(bool c)
{
    if(c)
    {
        sctpSocket->abort();

        QString s("connecting to " + hostComboBox->currentText().toLatin1() + "\nport: " + portLineEdit->text());

        consumers[SctpChannels::INFO]->displayInfo(s);

        sctpSocket->connectToHost(hostComboBox->currentText(), portLineEdit->text().toInt());

        if(!sctpSocket->waitForConnected(5000))
        {
            s.clear();
            s = "ERROR: " + sctpSocket->errorString();

            consumers[SctpChannels::INFO]->displayInfo(s);

            connectButton->toggle();
        }
    }
    else
    {
        disconnected();
    }


}

void Client::readDatagram(int channel)
{
    sctpSocket->setCurrentReadChannel(channel);
    consumers[channel]->readDatagram(sctpSocket->readDatagram().data());
}

void Client::enableConnectButton()
{
    connectButton->setEnabled(!hostComboBox->currentText().isEmpty() && !portLineEdit->text().isEmpty());
}

void Client::writeDatagram(const QByteArray &ba)
{
    if (sctpSocket->isValid() && sctpSocket->state() == QAbstractSocket::ConnectedState) {
        sctpSocket->setCurrentWriteChannel(consumers.indexOf(static_cast<Consumer *>(sender())));
        sctpSocket->writeDatagram(ba);
    }
}
