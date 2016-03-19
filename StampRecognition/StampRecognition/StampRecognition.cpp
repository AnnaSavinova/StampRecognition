#include "StampRecognition.h"

CStampRecognition::CStampRecognition( const char* imagePath, std::vector< CCircle > _answers ) :
    answers( _answers )
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
        10,
        grayImage->width/5
        );

    drawResult( results );

    cvReleaseMemStorage( &storage );
}

void CStampRecognition::drawResult( CvSeq * results )
{
    IplImage* base = cvCloneImage( image );

    // пробегаемся по кругам и рисуем их на оригинальном изображении
    for( size_t i = 0; i < answers.size(); ++i ) {
        CvPoint center = cvPoint( answers[i].GetCenterX(), answers[i].GetCenterY() );
        cvCircle( base, center, answers[i].GetRadius(), CV_RGB( 0, 0xff, 0 ), 3 );
    }

    for( int i = 0; i < results->total; i++ ) {
        float* circle = ( float* ) cvGetSeqElem( results, i );
        CvPoint center = cvPoint( cvRound( circle[0] ), cvRound( circle[1] ) );
        cvCircle( base, center, cvRound( circle[2] ), CV_RGB( 0xff, 0, 0 ) );
    }

    cvNamedWindow( "ResultCirclesOnImage", 0 );
    cvShowImage( "ResultCirclesOnImage", base );

    // ждём нажатия клавиши
    cvWaitKey( 0 );
    cvReleaseImage( &base );
}

CStampRecognition::CCircle::CCircle( int _centerX, int _centerY, int _radius ) : centerX( _centerX ), centerY( _centerY ), radius( _radius )
{}

CStampRecognition::CCircle::~CCircle()
{}
