#ifndef PYRAMIDS_H
#define PYRAMIDS_H

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <stdio.h>
#include <QDebug>
#include <vector>

void buildGaussian(const cv::Mat &orig, std::vector<cv::Mat> &pyramid, const int lvls);

void buildLaplacian(const cv::Mat &orig, std::vector<cv::Mat> &pyramid, const int lvls);

void upSampleGaussian(const cv::Mat &orig, cv::Mat &out, const int lvls);

//TODO
void upSampleLaplacian();


#endif // PYRAMIDS_H
