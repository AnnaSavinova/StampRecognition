#include "HoughRecognition.h"
#include <iostream>

CHoughRecognition::CHoughRecognition( IplImage* _grayImage ) : grayImage( _grayImage )
{}


CHoughRecognition::~CHoughRecognition()
{}

std::vector<CCircle> CHoughRecognition::FindCircles()
{
    return runHoughWithParams( 2, 5, 100, 170, 5, 120 );
}

std::vector<CCircle> CHoughRecognition::runHoughWithParams( double dp, double minDist, double param1, double param2, double minSize, double maxSize )
{
    CvMemStorage* storage = cvCreateMemStorage( 0 );
    CvSeq* results = cvHoughCircles(
        grayImage,
        storage,
        CV_HOUGH_GRADIENT,
        dp,
        minDist,
        param1,
        param2,
        minSize,
        maxSize
    );

    cvReleaseMemStorage( &storage );

    return cvSeqToVectorCircles( results );
}

std::vector<CCircle> CHoughRecognition::cvSeqToVectorCircles( CvSeq* seq )
{
    std::vector<CCircle> result;

    for( size_t i = 0; i < seq->total; ++i ) {
        float* element = ( float* ) cvGetSeqElem( seq, i );
        CCircle circle( cvRound( element[0] ), cvRound( element[1] ), cvRound( element[2] ) );

        result.push_back( circle );
    }

    return result;
}
