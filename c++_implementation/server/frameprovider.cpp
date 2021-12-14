#include "frameprovider.h"

#include <vector>
#include "opencv2/imgcodecs.hpp"

FrameProvider::FrameProvider(QObject *parent)
    : Provider(parent)
{
    qDebug() << "frameProvider creation";
}

void FrameProvider::frameReady(cv::Mat &img)
{
//    qDebug() << "frameReady slot filled";

//    qDebug() << img.empty();

    std::vector<uchar> encoded;

    //convert Mat to jpg
    std::vector <int> compression_params;
    compression_params.push_back(cv::IMWRITE_JPEG_QUALITY);
    compression_params.push_back(jpgQual);

    cv::imencode(".jpg", img, encoded, compression_params);

    QByteArray buf; // = reinterpret_cast<const char*>(encoded.data()), encoded.size());
    QDataStream ds(&buf, QIODevice::WriteOnly);

    ds.writeRawData((const char*) encoded.data(), encoded.size());

//    qDebug() << buf.size();

    emit writeDatagram(0, buf);

//    qDebug() << "frame sent to client";
}
