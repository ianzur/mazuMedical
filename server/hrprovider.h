#ifndef HRPROVIDER_H
#define HRPROVIDER_H

#include <QObject>

#include "provider.h"

class HRProvider : public Provider
{
    Q_OBJECT
public:
    explicit HRProvider(QObject *parent = nullptr);

signals:

public slots:
    void floatReady(double) override;
};

#endif // HRPROVIDER_H
