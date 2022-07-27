//! @file Splitter.cpp

#include "Splitter.h"
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <math.h>
#include <vector>

 
 

//-----------------------------------------------------------------------------
Splitter::Splitter()
{
}

//-----------------------------------------------------------------------------
std::vector<Rect> Splitter::gridSplit(cv::Mat image,
                                      unsigned int splitDimXMax,
                                      unsigned int splitDimYMax)
{
    std::vector<Rect> result = {};

    unsigned int imageDimX = image.cols;
    unsigned int imageDimY = image.rows;

    unsigned int gridDimX = static_cast<unsigned int>(ceil(float(imageDimX) / splitDimXMax));
    unsigned int gridDimY = static_cast<unsigned int>(ceil(float(imageDimY) / splitDimYMax));

    for (unsigned int i = 0; i < gridDimX; ++i)
    {
        for (unsigned int j = 0; j < gridDimY; ++j)
        {
            Rect cell = {};
            cell.topLeft.x = i * splitDimXMax;
            cell.topLeft.y = j * splitDimYMax;
            cell.bottomRight.x = std::min((i + 1) * splitDimXMax, imageDimX);
            cell.bottomRight.y = std::min((j + 1) * splitDimYMax, imageDimY);

            result.push_back(cell);
        }
    }

    return result;
}

//-----------------------------------------------------------------------------
 /* //   1st Task
std::vector<Rect> Splitter::smartSplit(cv::Mat image,
    unsigned int splitDimXMax,
    unsigned int splitDimYMax)

{
     std::vector<Rect> result = {};
    unsigned int imageDimX = image.cols;
    unsigned int imageDimY = image.rows;

    unsigned int gridDimX = static_cast<unsigned int>(ceil(float(imageDimX) / splitDimXMax));
    unsigned int gridDimY = static_cast<unsigned int>(ceil(float(imageDimY) / splitDimYMax));
    for (unsigned int i = 0; i < gridDimX; ++i)
    {
        for (unsigned int j = 0; j < gridDimY; ++j)
        {
            Rect cell = {};
            cell.topLeft.x = i * splitDimXMax;
            cell.topLeft.y = j * splitDimYMax;
            cell.bottomRight.x = std::min((i + 1) * splitDimXMax, imageDimX);
            cell.bottomRight.y = std::min((j + 1) * splitDimYMax, imageDimY);
            cv::Rect myROI(cell.topLeft.x, cell.topLeft.y, cell.bottomRight.x - cell.topLeft.x, cell.bottomRight.y - cell.topLeft.y);//Rect(x,y,width,height)
            int TotalNumberOfPixels = (cell.bottomRight.x - cell.topLeft.x) * (cell.bottomRight.y - cell.topLeft.y);
            int ZeroPixels = TotalNumberOfPixels - cv::countNonZero(image(myROI)); //Dark pixels
             if (ZeroPixels > 0)
            {
                result.push_back(cell);
            }
        }
    }
           return result;  
} */

/*// 2nd Task

std::vector<Rect> Splitter::smartSplit(cv::Mat image,
    unsigned int splitDimXMax,
    unsigned int splitDimYMax)

{
    std::vector<Rect> result = {};
    Rect cell = {};
    std ::vector<std ::vector<cv::Point>> contours;

    //To store hierarchy(nestedness)
    std ::vector<cv::Vec4i> hierarchy;
    std::vector <double> distance;
 
    //Find contours
    findContours(image, contours, hierarchy, cv::RETR_TREE,cv:: CHAIN_APPROX_SIMPLE);
    for (int i = 1; i < contours.size(); i++)
    {
        auto rect = cv::boundingRect(contours[i]); 
        cell.topLeft.x = rect.tl().x;
        cell.topLeft.y = rect.tl().y;
        cell.bottomRight.x = rect.br().x;
        cell.bottomRight.y = rect.br().y;
        result.push_back(cell);
    }
     
    return result; 
}
*/


/*
 // 3d Task

std::vector<Rect> Splitter::smartSplit(cv::Mat image,
    unsigned int splitDimXMax,
    unsigned int splitDimYMax)
{
    std::vector<Rect> result = {};
    Rect cell = {};
    std ::vector<std ::vector<cv::Point>> contours;
    //To store hierarchy(nestedness)
    std ::vector<cv::Vec4i> hierarchy;
    //Find contours
   

    findContours(image, contours, hierarchy, cv::RETR_TREE,cv:: CHAIN_APPROX_SIMPLE);
    std::vector <double> distance(contours.size());
    std::vector <int> indeces(contours.size());
    for (int i = 1; i < contours.size(); i++)
    {
        auto rect = cv::boundingRect(contours[i]);
        distance[i] = sqrt(pow((rect.tl().x + rect.br().x)/2, 2) + pow((rect.tl().y + rect.br().y)/2, 2));
        indeces[i] = i;
     }

    sort(indeces.begin(), indeces.end(), [&](int A, int B) -> bool {  
        if (distance[A] != distance[B])
            return distance[A] < distance[B];
        else
            return distance[A];
        });

    for (int i = 1; i < indeces.size(); i++)
    {
        std::cout << distance[indeces[i]] << "dist" << indeces[i] << "ind"<<"\n";
        auto rect = cv::boundingRect(contours[indeces[i]]);
        cell.topLeft.x = rect.tl().x;
        cell.topLeft.y = rect.tl().y;
        cell.bottomRight.x = rect.br().x;
        cell.bottomRight.y = rect.br().y;
        result.push_back(cell);
    }

    return result;
} 
*/

std::vector<Rect> Splitter::smartSplit(cv::Mat image,
    unsigned int splitDimXMax,
    unsigned int splitDimYMax)
{
    std::vector<Rect> result = {};
    Rect cell = {};
    std::vector<std::vector<cv::Point>> contours;
     std::vector<cv::Vec4i> hierarchy;
     cv::Mat dilate_img, thres_img;
    //--- performing Otsu threshold ---
    cv::threshold(image, thres_img, 0,255, cv::THRESH_OTSU | cv::THRESH_BINARY_INV);
    //cv::imshow("thresh1", thres_img);
    cv::Mat elementKernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(150,100), cv::Point(-1, -1));
    cv::dilate(thres_img, dilate_img, elementKernel,cv::Point(150,100),1);
 
     findContours(dilate_img, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);
    std::vector <double> distance(contours.size());
    std::vector <int> indeces(contours.size());
    for (int i = 0; i < contours.size(); i++)
    {
        auto rect = cv::boundingRect(contours[i]);
        distance[i] = sqrt(pow((rect.tl().x + rect.br().x) / 2, 2) + pow((rect.tl().y + rect.br().y) / 2, 2));
        indeces[i] = i;
    }

    sort(indeces.begin(), indeces.end(), [&](int A, int B) -> bool {
        if (distance[A] != distance[B])
            return distance[A] < distance[B];});

    for (int i = 0; i < indeces.size(); i++)
    {
        std::cout << distance[indeces[i]] << "dist" << indeces[i] << "ind" << "\n";
        auto rect = cv::boundingRect(contours[indeces[i]]);
        cell.topLeft.x = rect.tl().x;
        cell.topLeft.y = rect.tl().y;
        cell.bottomRight.x = rect.br().x;
        cell.bottomRight.y = rect.br().y;
        result.push_back(cell);
    }

    return result;
}
