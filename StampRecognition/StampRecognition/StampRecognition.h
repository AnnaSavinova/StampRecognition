#pragma once

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <math.h>

class CStampRecognition {
public:
    CStampRecognition( const char* imagePath );
    ~CStampRecognition();

    void DoHough();

private:
    IplImage* image;
    IplImage* grayImage;

    void drawResult( CvSeq* results );
};

