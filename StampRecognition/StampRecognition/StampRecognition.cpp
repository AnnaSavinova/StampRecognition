#include "StampRecognition.h"

CStampRecognition::CStampRecognition( const char* imagePath )
{
    image = cvLoadImage( imagePath );
    grayImage = cvLoadImage( imagePath, CV_LOAD_IMAGE_GRAYSCALE );

    std::cout << "image: " << imagePath << std::endl;
    assert( image != 0 );
    assert( grayImage != 0 );
}

CStampRecognition::~CStampRecognition()
{
    cvDestroyAllWindows();
    cvReleaseImage( &image );
    cvReleaseImage( &grayImage );
}

void CStampRecognition::DoHough()
{
    CvMemStorage* storage = cvCreateMemStorage( 0 );
    cvSmooth( grayImage, grayImage, CV_GAUSSIAN, 5, 5 );
    CvSeq* results = cvHoughCircles(
        grayImage,
        storage,
        CV_HOUGH_GRADIENT,
        2,
        grayImage->width/5
        );

    drawResult( results );

    cvReleaseMemStorage( &storage );
}

void CStampRecognition::drawResult( CvSeq * results )
{
    IplImage* base = cvCloneImage( image );

    // пробегаемся по кругам и рисуем их на оригинальном изображении
    for( int i = 0; i < results->total; i++ ) {
        float* circle = ( float* ) cvGetSeqElem( results, i );
        CvPoint center = cvPoint( cvRound( circle[0] ), cvRound( circle[1] ) );
        cvCircle( base, center, cvRound( circle[2] ), CV_RGB( 0xff, 0xff, 0xff ) );
    }

    cvNamedWindow( "ResultCirclesOnImage", 0 );
    cvShowImage( "ResultCirclesOnImage", base );

    // ждём нажатия клавиши
    cvWaitKey( 0 );
    cvReleaseImage( &base );
}
