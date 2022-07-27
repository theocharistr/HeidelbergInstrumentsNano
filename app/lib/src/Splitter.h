//! @file Splitter.h

#ifndef SPLITTER_H_
#define SPLITTER_H_

#include "IImporter.h"

#include <opencv2/opencv.hpp>
#include <vector>

//-----------------------------------------------------------------------------
class Splitter
{
    public:

        Splitter();

        std::vector<Rect> gridSplit(cv::Mat image,
                                    unsigned int splitDimXMax,
                                    unsigned int splitDimYMax);

        std::vector<Rect> smartSplit(cv::Mat image,
                                     unsigned int splitDimXMax,
                                     unsigned int splitDimYMax);
};

#endif
