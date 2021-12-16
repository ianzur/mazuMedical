#include "infoprovider.h"

INFOProvider::INFOProvider(QObject *parent)
    : Provider(parent)
{
    qDebug() << "info creation";
}

void INFOProvider::strReady(std::string s)
{
    std::regex e("alarm");

    if(std::regex_search(s, e))
    {
//        system("mpg123 angryCows.mp3 &");
//        system("./alarm &");
    }

    QByteArray buf = QByteArray::fromStdString(s);

    emit writeDatagram(0, buf);
}

void INFOProvider::readDatagram(QSctpSocket &from, const QByteArray &ba)
{
    QString s = QString::fromUtf8(ba);

//    qDebug() << s;

    if(s.contains(QRegularExpression("alarm", QRegularExpression::CaseInsensitiveOption)))
    {
//        qDebug() << s;

        // used to play alarm on Rpi device
//        system("mpg123 angryCows.mp3 &");
//        system("./alarm &");
        emit writeDatagram(0, QString(tr("alarm")).toUtf8());
    }
    else if(s.contains(QRegularExpression("silence", QRegularExpression::CaseInsensitiveOption)))
    {
//        qDebug() << s;

        // kill alarm on Rpi
//        system("killall -q mpg123");
//        system("killall alarm");
//        system("./aK");

        emit writeDatagram(0, QString(tr("silence")).toUtf8());

    }
    else
    {
        qDebug() << "why did I recieve: " << s;
    }
}
