#pragma once

#include <math.h>
#include <exception>
#include <fstream>

#include "HoughRecognition.h"
#include "MinSquareRecognizing.h"

class CStampRecognition {
public:
    CStampRecognition( const char* _imagePath, std::vector< CCircle > _answers, std::string _imageSavePath, std::string _scoresPath );
    ~CStampRecognition();

    void DoHough();
    void DoMinSquare();

private:
    // TODO: подобрать константу
    const float EXTRA_CIRCLES_PENALTY = 0.0f;

    std::string imageSavePath;
    std::string scoresPath;
    IplImage* image;
    cv::Mat matImage;
    cv::Mat matGrayImage;
    IplImage* grayImage;
    std::vector< CCircle > answers;

    void drawResult( std::vector<CCircle> results );
    double scoreResult( std::vector<CCircle> results );
};

