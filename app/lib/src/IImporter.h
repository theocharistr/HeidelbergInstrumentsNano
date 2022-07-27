//! @file IImporter.h

#ifndef API_IMPORTER_H_
#define API_IMPORTER_H_

#include "Status.h"

#include <vector>

//-----------------------------------------------------------------------------
struct Coord
{
    unsigned int x;
    unsigned int y;
};

struct Rect
{
    Coord topLeft;
    Coord bottomRight;
};

//-----------------------------------------------------------------------------
struct IImporter
{
    virtual Status openFile(char *filePath) = 0;

    virtual Status gridSplit(unsigned int splitDimXMax,
                             unsigned int splitDimYMax) = 0;

    virtual Status smartSplit(unsigned int splitDimXMax,
                              unsigned int splitDimYMax) = 0;

    virtual Status getImage(std::vector<unsigned char> &result) = 0;
    virtual Status getCells(std::vector<Rect> &result) = 0;

    virtual unsigned int getDimX(void) = 0;
    virtual unsigned int getDimY(void) = 0;
    virtual unsigned int getNumCells(void) = 0;
};

#endif
