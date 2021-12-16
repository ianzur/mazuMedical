#ifndef FRAMEPROVIDER_H
#define FRAMEPROVIDER_H

#include <QDebug>
#include <QDataStream>
#include <QIODevice>

#include "provider.h"

class FrameProvider : public Provider
{
    Q_OBJECT
public:
    explicit FrameProvider(QObject *parent = nullptr);

signals:

private:

public slots:
    void frameReady(cv::Mat &) override;

};

#endif // FRAMEPROVIDER_H
