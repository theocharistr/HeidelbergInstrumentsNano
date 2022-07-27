//! @file Importer.h

#ifndef IMPORTER_H_
#define IMPORTER_H_

#include "IImporter.h"
#include "Splitter.h"

#include <opencv2/opencv.hpp>

//-----------------------------------------------------------------------------
class Importer : public IImporter
{
    private:

        Splitter splitter;

        cv::Mat image;
        std::vector<Rect> cells;

    private:

        void _cleanup();

    public:

        Importer();

        Status openFile(char *filePath) override;

        Status gridSplit(unsigned int splitDimXMax,
                         unsigned int splitDimYMax) override;

        Status smartSplit(unsigned int splitDimXMax,
                          unsigned int splitDimYMax) override;

        Status getImage(std::vector<unsigned char> &image) override;
        Status getCells(std::vector<Rect> &result) override;

        unsigned int getDimX() override;
        unsigned int getDimY() override;
        unsigned int getNumCells() override;
};

#endif
