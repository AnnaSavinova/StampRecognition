#pragma once

#include "Circle.h"

class CHoughRecognition {
public:
    CHoughRecognition( cv::Mat _grayImage );
    ~CHoughRecognition();

    std::vector<CCircle> FindCircles();
private:
    cv::Mat grayImage;

    std::vector<CCircle> runHoughWithParams( double dp, double minDist, double param1, double param2, double minSize, double maxSize );

    static std::vector<CCircle> cvSeqToVectorCircles( const std::vector<cv::Vec3f>& seq );
};

