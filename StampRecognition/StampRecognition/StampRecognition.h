#pragma once

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <math.h>

class CStampRecognition {
public:
    struct CCircle {
    public:
        CCircle( int _centerX, int _centerY, int _radius );
        ~CCircle();

        int GetCenterX() { return centerX; }
        int GetCenterY() { return centerY; }
        int GetRadius() { return radius; }

    private:
        int centerX;
        int centerY;
        int radius;
    };

    CStampRecognition( const char* _imagePath, std::vector< CCircle > _answers );
    ~CStampRecognition();

    void DoHough();

private:
    IplImage* image;
    IplImage* grayImage;
    std::vector< CCircle > answers;
    
    void drawResult( CvSeq* results );
};

