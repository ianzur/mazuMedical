#include "processorthread.h"
#include "opencv2/objdetect.hpp"

#include <deque>

#include <QDebug>
#include <sys/timeb.h>
#include <vector>
#include <math.h>
#include <iostream>
#include <iomanip>

using namespace cv;

ProcessorThread::ProcessorThread(QObject *parent) : QThread(parent)
{

}

void ProcessorThread::downsample(cv::Mat &orig, std::vector<cv::Mat> &pyramid)
{
//    qDebug() << "call: downsample()";
    switch (pType) {
    case 0: //Gaussian
        buildGaussian(orig, pyramid, lvls);
        break;
    case 1: //Laplacian of Gaussian
        //buildLaplacian(orig, pyramid, lvls); // TODO
        break;
    default:
        break;
    }
}

void ProcessorThread::upsample(cv::Mat &orig, cv::Mat &out, int lvls)
{
    //qDebug() << "call: upsample()";
    switch (pType)
    {
    case 0:
        upSampleGaussian(orig, out, lvls);
        break;
    case 1:
        //upsampleLaplacian TODO
        break;
    default:
        break;
    }
}

int ProcessorThread::CLOCK()
{
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC,  &t);
    return (t.tv_sec * 1000)+(t.tv_nsec*1e-6);
}

double ProcessorThread::avgfps()
{
    //qDebug() << "call: avgfps()";

    if(CLOCK()-_fpsstart>1000)
    {
        _fpsstart=CLOCK();
        _avgfps=0.8*_avgfps+0.2*_frames1sec; //adjust ratio to change fps variablility a*avg+b*(#framesLastSec) (where a+b=1)
        _frames1sec=0;
    }

    _frames1sec++;
    return _avgfps;
}

//calculate moving average
void ProcessorThread::avgHR(double n, std::deque<double> &A)
{
    double old;
    double avg;

    //not full, test different sizes
    if((int)A.size() < maSize)
    {
//        qDebug() << "first" << avg;
        A.push_back(n);
        Asum = Asum + n;
    }

    //full pop front, push back
    else
    {
        old = A.front();
        A.pop_front();
        A.push_back(n);

        //subtract oldest, add newest
        Asum = Asum - old + n;

        avg = Asum/maSize;

        //emit moving average
        emit outHR(avg);

        if(avg < 35.0)
        {
            emit outINFO("alarm");
        }
    }

}

void ProcessorThread::createIdealBandpassFilter(cv::Mat &mask, float lo, float hi, double fps)
{
    // qDebug() << "call: createIdealBPfilter()" << fps;

    // Create mask for one row
        cv::Mat rowMask(1, mask.cols, CV_32F);

        //cout << rowMask.rows << " " << rowMask.cols;

//        lo = (lo/fps)*rowMask.cols;
//        hi = (hi/fps)*rowMask.cols;

         lo = (2*lo*rowMask.cols)/fps;
         hi = (2*hi*rowMask.cols)/fps;

        float* mptr = rowMask.ptr<float>(0);

        for(int i=0; i<rowMask.cols; i++)
        {
            if(i >= lo && i <= hi)
                mptr[i] = 10.f;
            else
                mptr[i] = 0.f;
        }

       //mask will remain the same for ever row (based on fps) -> copy r many times down
       repeat(rowMask, mask.rows, 1, mask);


    //qDebug() << "Bandpass FINISH!!";
}

void ProcessorThread::concatenate(cv::Mat &frame, cv::Mat &histMat, int i, bool full)
{
    //qDebug() << "call: concatenate()";

    cv::Mat debug;

    cv::Mat temp(frame.rows*frame.cols, 1, CV_32FC3);

    cv::Mat reshaped, column;
    reshaped = frame.reshape(0, frame.rows*frame.cols).clone();
    reshaped.copyTo(temp);

    //qDebug() << histMat.rows << temp.rows;

    if(full)//Shift the histMat image one column to the left and put the new column in the last position
    {
        histMat(cv::Rect(1, 0, histMat.cols-1, histMat.rows)).copyTo(histMat(cv::Rect(0, 0, histMat.cols-1, histMat.rows)));
        temp.copyTo(histMat.col(length-1));
    }
    else//initially fill histMat
    {
        temp.copyTo(histMat.col(i));
    }

    column.release();
    temp.release();
}

/*removes the most recent frame @ (length-1)
 * reshapes and clone
 */
void ProcessorThread::fetch(cv::Mat &oneDem, cv::Mat &frame)
{
    //qDebug() << "call: fetch()" << frame.rows << frame.cols;
    cv::Mat column = oneDem.col(length-1).clone();
    frame = column.reshape(0, frame.rows).clone();

    column.release();
}

void ProcessorThread::temporalFilter(cv::Mat &histMat, cv::Mat &filtered1d, std::deque<double> &A)
{
    //qDebug() << "call: temporalFilter()";

    switch (fType)    {
    case 0: //Ideal
    {
        cv::Mat channels[3];

            // split into 3 channels
            cv::split(histMat, channels);

            for (int i = 0; i < 3; ++i){

                cv::Mat current = channels[i];  // current channel
                cv::Mat tempImg, temp2;

                int width = cv::getOptimalDFTSize(current.cols);
                int height = cv::getOptimalDFTSize(current.rows);

                cv::copyMakeBorder(current, tempImg, 0, height - current.rows, 0, width - current.cols, cv::BORDER_CONSTANT, cv::Scalar::all(0));

                // do the DFT
                cv::dft(tempImg, tempImg, cv::DFT_ROWS);

                cv::Mat filter = tempImg.clone();

                // construct the filter
                createIdealBandpassFilter(filter, lowHz, highHz, _avgfps);

                // apply filter
                cv::mulSpectrums(tempImg, filter, tempImg, cv::DFT_ROWS);

                cv::Mat reduced(1, tempImg.cols, CV_32F);

                temp2 = abs(tempImg);

                cv::reduce(temp2, reduced, 0, cv::REDUCE_SUM);



                double minVal, maxVal;
                int minIdx[2], maxIdx[2];

                cv::minMaxIdx(reduced, &minVal, &maxVal, minIdx, maxIdx);

                if(i==1)
                {
                    avgHR(((((float)maxIdx[1])*_avgfps)/(2*tempImg.cols)*60), A);
//                    emit outHR(((((float)maxIdx[1])*_avgfps)/(2*tempImg.cols)*60));
                }

//                cout << "channel " << i << " est. HR = " << ((((float)maxIdx[1])*_avgfps)/(2*tempImg.cols)*60) << "   " << maxIdx[1] << endl;

                // do the inverse DFT on filtered image
                cv::idft(tempImg, tempImg, cv::DFT_ROWS | cv::DFT_SCALE);

                // copy back to the current channel
                tempImg(cv::Rect(0, 0, current.cols, current.rows)).copyTo(channels[i]);
            }
            //merge channels
            cv::merge(channels, 3, filtered1d);


            // normalize the filtered image
            cv::normalize(filtered1d, filtered1d, 0, 1, cv::NORM_MINMAX);

        break;
    }
    case 1:
        // TODO Motion maginification (IIR filter)
        break;
    default:
        break;
    }
}

void ProcessorThread::detectFace(cv::Mat image)
{
    double min_face_size=100;
    double max_face_size=300;

   // Load Face cascade (.xml file)
   cv::CascadeClassifier face_cascade( "/usr/local/share/OpenCV/haarcascades/haarcascade_frontalface_alt2.xml" );

   // Detect faces
   std::vector<cv::Rect> faces;
   face_cascade.detectMultiScale( image, faces, 1.2, 2, 0|cv::CASCADE_SCALE_IMAGE, cv::Size(min_face_size, min_face_size), cv::Size(max_face_size, max_face_size));

   //return face (if one is found)
   if(faces.size() < 1)
       this->myROI = cv::Rect(0,0,0,0);
   else
       this->myROI = faces[0];
}

void ProcessorThread::run()
{
    emit outINFO(std::string("Processor Start"));

    VideoCapture c(0);

    c.set(CAP_PROP_FRAME_WIDTH, 480);
    c.set(CAP_PROP_FRAME_HEIGHT, 360);
    c.set(CAP_PROP_FPS, 25);

    //iterator
    int i = 0;

    bool full = false;

    //holds orignial frames
    cv::Mat roiFrames[length];

    //holds downsampled frames
    cv::Mat dFrames[length];

    /*calculating size of histMatThe magnified version makes it possible to see the chang
     * Based on capture size c.get() and number of downsampling levels (lvls)
     * each downsample reduces image height and width by factor of 2 (pow(2,lvls))
     *
     * frameHeight/(2^lvls)*frameWidth/(2^lvls) = # of rows
     * length                                   = # of cols
     *
     * EDIT: added ceil, division was giving wrong size.
     * Round up, same as downsample function.
     */

     int oneDemensionH = ceil((c.get(CAP_PROP_FRAME_HEIGHT)/(pow(2,lvls)))*(ceil((c.get(CAP_PROP_FRAME_WIDTH)/(pow(2, lvls))))));

     cv::Mat histMat(oneDemensionH,length,CV_32FC3);

   //  cv::Mat histMat;

    /* histMat (history Mat) holds concated frames Mat(rows,cols,type)
     * each frame is transformed into a single column of this frame:
     *
     *  one frame:
     *          1a, 1b;
     *          1c, 1d;   histMat.convertTo(debug, CV_8U);
//            cvtColor(debug,debug, cv::COLOR_Lab2RGB);
     *
     *  all combined:
     *   1a, 2a, 3a, 4a;
     *   1b, 2b, 3b, 4b;
     *   1c, 2c, 3c, 4c;
     *   1d, 2d, 3d, 4d;
     *
     *  or*:
     *   1a, 2a, 3a, 4a;
     *   1c, 2c, 3c, 4c;
     *   1b, 2b, 3b, 4b;
     *   1d, 2d, 3d, 4d;
     *
     * Using cv::reshape() unsure of the exact algorithm in this function
     * as long as it does it the same everytime
     *
     * This should simplify temporal filtering
     */

    //for displaying intermediate steps
    cv::Mat debug, d2;

    //holds currant filtered frame
    cv::Mat filtered;

    //holds amplified frame
    //    cv::Mat amplified(oneDemensionH,1, CV_32FC3);
    //    cv::Mat amplified;

    //holds
    cv::Mat motion;

    cv::Mat final;

    cv::Mat output;

    //holds intial frame read until converted and placed in oFrames[]
    cv::Mat temp;

    cv::Mat reshaped;

    //Play with the size of the moving average
    std::deque<double> movingAvg;

    movingAvg.erase(movingAvg.begin(),movingAvg.end());

    // find roi
    do{
        emit outINFO(std::string("Finding ROI"));

        c >> temp;

        if(temp.empty())
            continue;

        detectFace(temp);

    }while(myROI.size() == cv::Size(0,0));

//    this->myROI = cv::Rect(0,0,480, 360);

    emit outINFO(std::string("ROI found!\nIf ROI is not covering face disconnect & reconnect"));

    emit outINFO(std::string("Filling Frame History\nPlease wait ~15 seconds"));

    //main loop
    while(c.isOpened() && !isInterruptionRequested())
    {
        temp.release();
        roiFrames[i].release();
        dFrames[i].release();

        c >> temp;

//        qDebug() << temp.rows << temp.cols;

        if(temp.empty())//no frame? try again.
            continue;

        //Convert color space then
        cv::cvtColor(temp, temp, cv::COLOR_BGR2Lab);

        // extract roi
        cv::Mat roi = temp(myROI);

//        qDebug() << roi.empty();

       //Convert type
        roi.convertTo(roiFrames[i], CV_32F);

        //holds current frames full pyramid
        std::vector<cv::Mat> pyramid;

        //DOWNSAMPLE
        downsample(roiFrames[i], pyramid);
        dFrames[i] = pyramid.at(lvls-1);

        cv::resize(histMat, histMat, cv::Size(length,dFrames[i].rows*dFrames[i].cols), 0, 0, cv::INTER_CUBIC);

        //CONCATENATE
        concatenate(dFrames[i], histMat, i, full);

        //temporal filtering
        if(full == true)
        {
            //TEMPORAL FILTER
            temporalFilter(histMat, filtered, movingAvg);

            // amplified = column image from last row of temporally filtered histMat
            cv::Mat amplified(dFrames[i].rows*dFrames[i].cols, 1, CV_32FC3);

            //AMPLIFY
            //Only Amplify current frame?? (.col(last)??)
            amplified = filtered.col(length-1).clone();

//            amplified.convertTo(debug, CV_8U);

            //qDebug() << amplified.rows << amplified.cols << amplified.isContinuous();
//            qDebug() << filtered.rows << filtered.cols;

            //RESHAPE most recent frame
            dFrames[i] = amplified.reshape(0, dFrames[i].rows).clone();

            //fetch(amplified, dFrames[i]);

//            qDebug() << dFrames[i].rows << "  " << dFrames[i].cols;


            //cv::resize(motion, motion, cv::Size(1, dFrames[i].rows*dFrames[i].cols), 0, 0, INTER_CUBIC);

            //UPSAMPLE
            upsample(dFrames[i], motion, lvls);

            //final = roiFrames[i].clone();

            //qDebug() << motion.type() << dFrames[i].type() << final.type() << roiFrames[i].type();

//            qDebug() << ROI.isSubmatrix();

            //just incase (the more pyramid lvls things might get wierd after upsample)
            cv::resize(motion, motion, cv::Size(roi.cols, roi.rows));

//            qDebug() << "final: " << final.rows << final.cols << "\nroiFrames" << roiFrames[i].rows << roiFrames[i].cols << "\nmotion: " << motion.rows << motion.cols;

            output = roiFrames[i].clone();

            //motion = ROI + motion;

            output = roiFrames[i] + motion;

            //final(myROI) = roiFrames[i](myROI)+motion(myROI);
//            qDebug() << "finish addition";

            double minVal, maxVal;
            cv::minMaxLoc(output, &minVal, &maxVal); //find minimum and maximum intensities
            //convert back to unsign int 0-255 RGB
            //qDebug() << output.type() << roiFrames[i].type();

            output.convertTo(output, CV_8UC3, 255.0/(maxVal - minVal),-minVal * 255.0/(maxVal - minVal));

            output.copyTo(temp(myROI));

            cv::cvtColor(temp, temp, cv::COLOR_Lab2BGR);



            cv::rectangle(temp, myROI, cv::Scalar(5,144,51), 2);

            emit outDisplay(temp);

            emit outFPS(avgfps());
        }
        else
        {
            // convert format back to 8-bit for display
            temp.convertTo(temp, CV_8U);

            // convert to from lab to BGR
            cv::cvtColor(temp, temp, cv::COLOR_Lab2BGR);

            cv::rectangle(temp, myROI, cv::Scalar(5,144,51), 2);

            emit outDisplay(temp);
        }

        // frame history full?
        if(i==length-1)
        {
            i = -1;
            full = true; //run Temporal filter + amplification
        }

        i++;
    }
}

void ProcessorThread::update(cv::Rect r, float low, float high)
{
    //qDebug() << "call: update()";
   // this->myROI = cv::Rect(r.x(),r.y(),r.width(),r.height());
    this->myROI = r;

    this->lowHz = low;
    this->highHz = high;
}
