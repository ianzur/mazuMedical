#include "pyramids.h"

void buildGaussian(const cv::Mat &orig, std::vector<cv::Mat> &pyramid, const int lvls)
{
    if(lvls < 1)
    {
        qDebug() << "ERROR LVLS MUST BE > 1";
    }

    pyramid.clear();

    cv::Mat curr = orig;
    for(int l=0; l<lvls; l++)
    {
        cv::Mat dn;
        cv::pyrDown(curr, dn);
        pyramid.push_back(dn);
        curr = dn;
    }
}

void upSampleGaussian(const cv::Mat &orig, cv::Mat &out, const int lvls)
{
    cv::Mat curr = orig.clone();
    for(int l=0; l < lvls; l++)
    {
        cv::Mat up;
        cv::pyrUp(curr, up);
        curr = up;
        up.release();
    }
    curr.copyTo(out);

    curr.release();
}


void buildLaplacian(const cv::Mat &orig, std::vector<cv::Mat> &pyramid, const int lvls)
{
    if(lvls < 1)
    {
        qDebug() << "ERROR LVLS MUST BE > 1";
    }

    pyramid.clear();

    cv::Mat curr = orig;
    for(int l=0; l<lvls; l++)
    {
        cv::Mat dn, up;
        cv::pyrDown(curr, dn);
        cv::pyrUp(dn, up, curr.size());
        cv::Mat lap = curr-up;
        pyramid.push_back(lap);
        curr = dn;
    }
    pyramid.push_back(curr);
}

//TODO
//void upSampleLaplacian(
