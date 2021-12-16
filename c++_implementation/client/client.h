#ifndef CLIENT_H
#define CLIENT_H

#include <QDebug>

#include <QDialog>
#include <QVector>
#include <QSctpSocket>
#include <QRegularExpression>

QT_BEGIN_NAMESPACE
class QComboBox;
class QLineEdit;
class QPushButton;
class QByteArray;
QT_END_NAMESPACE


class Consumer;

class Client : public QDialog
{
    Q_OBJECT

public:
    explicit Client(QWidget *parent = 0);
    ~Client();

private slots:
    void connected();
    void disconnected();
    void connectPressed(bool c);
    void readDatagram(int channel);
    void writeDatagram(const QByteArray &ba);
    void enableConnectButton();

private:
    QVector<Consumer *> consumers;
    QSctpSocket *sctpSocket;

    QComboBox *hostComboBox;
    QLineEdit *portLineEdit;
    QPushButton *connectButton;
};

#endif // CLIENT_H
