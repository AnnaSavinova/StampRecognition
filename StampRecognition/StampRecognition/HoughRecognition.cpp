#include "HoughRecognition.h"
#include <iostream>

CHoughRecognition::CHoughRecognition( cv::Mat _grayImage ) : grayImage( _grayImage )
{}


CHoughRecognition::~CHoughRecognition()
{}

std::vector<CCircle> CHoughRecognition::FindCircles()
{
    return runHoughWithParams( 2, 10, 100, 170, 5, 120 );
}

std::vector<CCircle> CHoughRecognition::runHoughWithParams( double dp, double minDist, double param1, double param2, double minSize, double maxSize )
{
    std::vector<cv::Vec3f> results;
    cv::HoughCircles(
        grayImage,
        results,
        CV_HOUGH_GRADIENT,
        dp,
        minDist,
        param1,
        param2,
        minSize,
        maxSize
    );

    return cvSeqToVectorCircles( results );
}

std::vector<CCircle> CHoughRecognition::cvSeqToVectorCircles( const std::vector<cv::Vec3f>& seq )
{
    std::vector<CCircle> result;

    for( size_t i = 0; i < seq.size(); ++i ) {
        CCircle circle( cvRound( seq[i][0] ), cvRound( seq[i][1] ), cvRound( seq[i][2] ) );

        result.push_back( circle );
    }

    return result;
}
