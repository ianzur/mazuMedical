#ifndef INFOPROVIDER_H
#define INFOPROVIDER_H

#include <QObject>
#include <QDebug>
#include <QRegularExpression>

#include "provider.h"

class INFOProvider : public Provider
{
    Q_OBJECT
public:
    explicit INFOProvider(QObject *parent = nullptr);

    void readDatagram(QSctpSocket &from, const QByteArray &ba) Q_DECL_OVERRIDE;

signals:

public slots:
    void strReady(std::string) override;

};

#endif // INFOPROVIDER_H
