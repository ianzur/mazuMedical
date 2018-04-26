#include "infoconsumer.h"

#include <QWidget>
#include <QPushButton>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QString>

InfoConsumer::InfoConsumer(QObject *parent)
    : Consumer(parent)
{
    frameWidget = new QWidget;

    textEdit = new QTextEdit;
    textEdit->setMinimumSize(260,150);
    textEdit->setStyleSheet("font: 5pt;");


    silenceButton = new QPushButton(tr("Alarm"));
    silenceButton->setMinimumSize(260,50);
    silenceButton->setStyleSheet("font: 8pt;");
    silenceButton->setAutoDefault(false);

    connect(silenceButton, &QPushButton::clicked, this, &InfoConsumer::alarmPressed);

    QVBoxLayout *layout = new QVBoxLayout(frameWidget);
    layout->setContentsMargins( 0, 0, 0, 0);
    layout->addWidget(textEdit);
    layout->addWidget(silenceButton, 1, Qt::AlignJustify);
}

QWidget *InfoConsumer::widget()
{
    return frameWidget;
}

void InfoConsumer::displayInfo(QString s)
{
    textEdit->append(s);
}

void InfoConsumer::readDatagram(const QByteArray &ba)
{
    QString s = QString::fromUtf8(ba);

    qDebug() << s;


    if(s.contains(QRegExp("alarm", Qt::CaseInsensitive)))
    {
//        system("mpg123 angryCows.mp3 &");
//        system("./alarm &");

        silenceButton->setText("Silence Alarm");
    }
    else if(s.contains(QRegExp("silence", Qt::CaseInsensitive)))
    {
//        system("killall -q mpg123");
//        system("killall alarm");
//        system("./aK");

        silenceButton->setText("Alarm");
    }

    textEdit->append(s);
}

void InfoConsumer::alarmPressed()
{
    if( silenceButton->text() == "Alarm")
    {
        //tell camera unit to alarm
        emit writeDatagram(QString(tr("alarm")).toUtf8());
    }
    else
    {
        //tell camera unit to silence
        emit writeDatagram(QString(tr("silence")).toUtf8());
    }
}

void InfoConsumer::serverDisconnected()
{

}

