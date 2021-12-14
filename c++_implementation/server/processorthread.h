#ifndef PROCESSORTHREAD_H
#define PROCESSORTHREAD_H

#include <QThread>
#include <deque>

#include "opencv2/opencv.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/objdetect.hpp"


// my libraries
#include "pyramids.h"
//#include "filters.h"

class ProcessorThread : public QThread
{
      Q_OBJECT


  public:
      explicit ProcessorThread(QObject *parent = 0);
      void update(cv::Rect, float low, float high);
      int CLOCK();
      double avgfps();
      void avgHR(double d, std::deque<double> &A);

  signals:
      void outDisplay(cv::Mat &img);
      void outFPS(double fps);
      void outHR(double hr);
      void outINFO(std::string);

  protected:
      void run() override;

  private:
      void setSize (int r, int c);
      void setDSize (int r, int c);

      void concatenate(cv::Mat &frame, cv::Mat &histMat, int i, bool full);
      void fetch(cv::Mat &histMat, cv::Mat &frame);

      void downsample(cv::Mat &o, std::vector<cv::Mat> &pyramid);
      void upsample(cv::Mat &orig, cv::Mat &out, int lvls);

      void temporalFilter(cv::Mat &histMat, cv::Mat &filtered1d, std::deque<double> &A);

      void createIdealBandpassFilter(cv::Mat &mask, float low, float high, double fps);

      void detectFace(cv::Mat);

      //Region of interest
      cv::Rect myROI;

      //number of pyramid levels
      int lvls = 3;

      int length = 250;

      //frequencies to filer for
      float lowHz = 30.0/60.0;
      float highHz = 150.0/60.0;

      int _fpsstart=0;

      //initial avgfps change to match what expected fps is
      double _avgfps=25.0;

      double _frames1sec=0;

      //0 = Gaussian, 1 = Laplacian
      int pType = 0;

      //0 = Ideal, 1 = IIR
      int fType = 0;

      int alpha = 75;

      int maSize = 64;
      double Asum;
};

#endif // PROCESSORTHREAD_H
