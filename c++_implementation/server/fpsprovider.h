#ifndef FPSPROVIDER_H
#define FPSPROVIDER_H

#include <QObject>
#include <QDebug>

#include "provider.h"

class FPSProvider : public Provider
{
    Q_OBJECT
public:
    explicit FPSProvider(QObject *parent = nullptr);

signals:

public slots:
    void floatReady(double) override;

};

#endif // FPSPROVIDER_H
