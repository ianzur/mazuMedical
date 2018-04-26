#ifndef INFOCONSUMER_H
#define INFOCONSUMER_H

#include "consumer.h"

QT_BEGIN_NAMESPACE
class QLabel;
class QPushButton;
class QTextEdit;
QT_END_NAMESPACE

class InfoConsumer : public Consumer
{
    Q_OBJECT
public:
    explicit InfoConsumer(QObject *parent = nullptr);

    QWidget *widget() Q_DECL_OVERRIDE;
    void readDatagram(const QByteArray &ba) Q_DECL_OVERRIDE;
    void serverDisconnected() Q_DECL_OVERRIDE;
    void displayInfo(QString) Q_DECL_OVERRIDE;

private slots:
    void alarmPressed();

private:
    QWidget *frameWidget;
    QTextEdit *textEdit;

    QPushButton *silenceButton;

};

#endif // HRCONSUMER_H
