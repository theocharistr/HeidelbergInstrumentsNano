//! @file Importer.cpp

#include "Importer.h"

//-----------------------------------------------------------------------------
Importer::Importer()
{
}

//-----------------------------------------------------------------------------
void Importer::_cleanup()
{
    image.release();
}

//-----------------------------------------------------------------------------
Status Importer::openFile(char *filePath)
{
    _cleanup();

    try
    {
        image = cv::imread(filePath, cv::IMREAD_GRAYSCALE);
    }
    catch (cv::Exception const&)
    {
        return Status::ImporterError;
    }

    if (image.empty())
        return Status::ImporterError;

    return Status::Success;
}

//-----------------------------------------------------------------------------
Status Importer::gridSplit(unsigned int splitDimXMax,
                           unsigned int splitDimYMax)
{
    cells = splitter.gridSplit(image, splitDimXMax, splitDimYMax);

    return Status::Success;
}

//-----------------------------------------------------------------------------
Status Importer::smartSplit(unsigned int splitDimXMax,
                            unsigned int splitDimYMax)
{
    cells = splitter.smartSplit(image, splitDimXMax, splitDimYMax);

    return Status::Success;
}

//-----------------------------------------------------------------------------
Status Importer::getImage(std::vector<unsigned char>& result)
{
    if (image.empty())
        return Status::ImporterError;

    if (image.isContinuous())
    {
        result.assign((unsigned char*)image.datastart,
                      (unsigned char*)image.dataend);
    }
    else 
    {
        for (int i = 0; i < image.rows; ++i)
        {
            result.insert(result.end(),
                          image.ptr<unsigned char>(i),
                          image.ptr<unsigned char>(i) + image.cols);
        }
    }

    return Status::Success;
}

//-----------------------------------------------------------------------------
Status Importer::getCells(std::vector<Rect>& result)
{
    result = cells;

    return Status::Success;
}

//-----------------------------------------------------------------------------
unsigned int Importer::getDimX()
{
    return image.cols;
}

//-----------------------------------------------------------------------------
unsigned int Importer::getDimY()
{
    return image.rows;
}

//-----------------------------------------------------------------------------
unsigned int Importer::getNumCells()
{
    return static_cast<unsigned int>(cells.size());
}
