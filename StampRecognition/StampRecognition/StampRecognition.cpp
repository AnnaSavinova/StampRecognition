#include "StampRecognition.h"

CStampRecognition::CStampRecognition( const char* imagePath, std::vector< CCircle > _answers ) :
    answers( _answers )
{
    image = cvLoadImage( imagePath );
    grayImage = cvLoadImage( imagePath, CV_LOAD_IMAGE_GRAYSCALE );

    std::cout << "image: " << imagePath << std::endl;
    if( image == 0 || grayImage == 0 ) {
        std::string errorMsg = "Невозможно открыть изображение ";
        errorMsg += imagePath;
        throw std::invalid_argument( errorMsg );
    }
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
    std::cout << "score: " << scoreResult( results ) << std::endl;

    cvReleaseMemStorage( &storage );
}

void CStampRecognition::drawResult( CvSeq * results )
{
    IplImage* base = cvCloneImage( image );

    // рисуем правильные ответы зелёным
    for( size_t i = 0; i < answers.size(); ++i ) {
        CvPoint center = cvPoint( answers[i].GetCenterX(), answers[i].GetCenterY() );
        cvCircle( base, center, answers[i].GetRadius(), CV_RGB( 0, 0xff, 0 ), 3 );
    }

    // рисуем результаты работы алгоритма красным
    for( int i = 0; i < results->total; ++i ) {
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

float CStampRecognition::scoreResult( CvSeq * results )
{
    float scoresSum = 0.0f;
    for( size_t i = 0; i < answers.size(); ++i ) {
        float maxScore = 0.0f;
        for( size_t j = 0; j < results->total; ++j ) {
            float* circle = ( float* ) cvGetSeqElem( results, j );
            float score = answers[i].IntersectionSquare( CCircle( circle[0], circle[1], circle[2] ) ) / answers[i].Square();
            if( score > maxScore ) {
                maxScore = score;
            }
        }
        scoresSum += maxScore;
    }

    // штрафуем за лишние найденные круги
    if( answers.size() < results->total ) {
        scoresSum += ( answers.size() - results->total ) * EXTRA_CIRCLES_PENALTY;
    }
    return scoresSum;
}

CStampRecognition::CCircle::CCircle( int _centerX, int _centerY, int _radius ) : centerX( _centerX ), centerY( _centerY ), radius( _radius )
{}

CStampRecognition::CCircle::~CCircle()
{}

float CStampRecognition::CCircle::IntersectionSquare( const CCircle& oth )
{
    float d = sqrt( ( centerX - oth.centerX ) * ( centerX - oth.centerX ) + ( centerY - oth.centerY ) * ( centerY - oth.centerY ) );

    //если окружности не пересекаются
    if( d > ( radius + oth.radius ) ) {
        return 0.0f;
    }

    //если центры окружностей совпадают, пересечение - окружность с меньшим радиусом
    if( !( d > 0 ) ) {
        int minRadius = MIN( radius, oth.radius );
        return CV_PI * minRadius * minRadius;
    }
    float f1 = 2 * acos( ( radius * radius - oth.radius * oth.radius + d * d ) / ( 2 * radius * d ) );
    float f2 = 2 * acos( ( oth.radius * oth.radius - radius * radius + d * d ) / ( 2 * oth.radius * d ) );
    float s1 = ( radius * radius * f1 - sin( f1 ) ) / 2;
    float s2 = ( oth.radius * oth.radius * f2 - sin( f2 ) ) / 2;
    return s1 + s2;
}
