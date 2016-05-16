#pragma once

#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>

#include <vector>
#include "Circle.h"

class CHoughRecognition {
public:
    CHoughRecognition( IplImage* _grayImage );
    ~CHoughRecognition();

    std::vector<CCircle> FindCircles();
private:
    IplImage* grayImage;

    std::vector<CCircle> runHoughWithParams( double dp, double minDist, double param1, double param2, double minSize, double maxSize );

    static std::vector<CCircle> cvSeqToVectorCircles( CvSeq* seq );
};

