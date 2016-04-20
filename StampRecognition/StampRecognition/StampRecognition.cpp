#include "StampRecognition.h"

CStampRecognition::CStampRecognition( const char* _imagePath, std::vector< CCircle > _answers, std::string _imageSavePath, std::string scoresPath ) :
    answers( _answers ), imageSavePath ( _imageSavePath )
{
    image = cvLoadImage( _imagePath );
    grayImage = cvLoadImage( _imagePath, CV_LOAD_IMAGE_GRAYSCALE );

    std::cout << "image: " << _imagePath << std::endl;
    if( image == 0 || grayImage == 0 ) {
        std::string errorMsg = "Невозможно открыть изображение ";
        errorMsg += _imagePath;
        throw std::invalid_argument( errorMsg );
    }

    scoresOut.open( scoresPath, std::ofstream::out );
}

CStampRecognition::~CStampRecognition()
{
    cvDestroyAllWindows();
    cvReleaseImage( &image );
    cvReleaseImage( &grayImage );

    scoresOut.close();
}

void CStampRecognition::DoHough()
{
    //std::cout << "DoHough!" << std::endl;
    CvMemStorage* storage = cvCreateMemStorage( 0 );
    cvSmooth( grayImage, grayImage, CV_GAUSSIAN, 5, 5 );
    CvSeq* results = cvHoughCircles(
        grayImage,
        storage,
        CV_HOUGH_GRADIENT,
        1,
        grayImage->width/8,
        200,
        100,
        0,
        0
        );

    drawResult( results );
    std::cout << "score: " << scoreResult( results ) << std::endl;
    scoresOut << imageSavePath << "," << scoreResult( results ) << std::endl;

    cvReleaseMemStorage( &storage );
}

void CStampRecognition::drawResult( CvSeq * results )
{
    //std::cout << "Draw!" << std::endl;
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
        cvCircle( base, center, cvRound( circle[2] ), CV_RGB( 0xff, 0, 0 ), 3 );
    }

    /*cvNamedWindow( "ResultCirclesOnImage", 0 );
    cvShowImage( "ResultCirclesOnImage", base );*/
    cvSaveImage( imageSavePath.c_str(), base );

    // ждём нажатия клавиши
    //cvWaitKey( 0 );
    cvReleaseImage( &base );
}

float CStampRecognition::scoreResult( CvSeq * results )
{
    //std::cout << "Score!" << std::endl;
    float scoresSum = 0.0f;
    for( size_t i = 0; i < answers.size(); ++i ) {
        float maxScore = 0.0f;
        for( size_t j = 0; j < results->total; ++j ) {
            float* circle = ( float* ) cvGetSeqElem( results, j );
            CCircle result( circle[0], circle[1], circle[2] );
            float score = answers[i].IntersectionSquare( result ) * 2.;
            score = score / ( answers[i].Square() + result.Square() );
            if( score > maxScore ) {
                maxScore = score;
            }
        }
        scoresSum += maxScore;
    }
    //нормируем для случая нескольких печатей
    if( answers.size() > 1 ) {
        scoresSum /= answers.size();
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

    //если окружность вложена во вторую, то пересечение - площадь меньшей
    if( ( ( centerX + radius ) <= ( oth.centerX + oth.radius ) ) &&
        ( ( centerX - radius ) >= ( oth.centerX - oth.radius ) ) &&
        ( ( centerY + radius ) <= ( oth.centerY + oth.radius ) ) &&
        ( ( centerY - radius ) >= ( oth.centerY - oth.radius ) ) ||
        ( ( centerX + radius ) >= ( oth.centerX + oth.radius ) ) &&
        ( ( centerX - radius ) <= ( oth.centerX - oth.radius ) ) &&
        ( ( centerY + radius ) >= ( oth.centerY + oth.radius ) ) &&
        ( ( centerY - radius ) <= ( oth.centerY - oth.radius ) )
        ) {
        float minRadius = MIN( radius, oth.radius );
        return CV_PI * minRadius * minRadius;
    }
    float f1 = 2 * acos( ( radius * radius - oth.radius * oth.radius + d * d ) / ( 2 * radius * d ) );
    float f2 = 2 * acos( ( oth.radius * oth.radius - radius * radius + d * d ) / ( 2 * oth.radius * d ) );
    float s1 = ( radius * radius * f1 - sin( f1 ) ) / 2;
    float s2 = ( oth.radius * oth.radius * f2 - sin( f2 ) ) / 2;
    return s1 + s2;
}
