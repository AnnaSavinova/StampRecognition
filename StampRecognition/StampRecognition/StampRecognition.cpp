#include "StampRecognition.h"


CStampRecognition::CStampRecognition( const char* _imagePath, std::vector< CCircle > _answers, std::string _imageSavePath ) :
    answers( _answers ), imageSavePath( _imageSavePath )
{
    image = cvLoadImage( _imagePath );
    grayImage = cvLoadImage( _imagePath, CV_LOAD_IMAGE_GRAYSCALE );

    std::cout << "image: " << _imagePath << std::endl;
    if( image == 0 || grayImage == 0 ) {
        std::string errorMsg = "Невозможно открыть изображение ";
        errorMsg += _imagePath;
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
    CHoughRecognition hr( grayImage );

    std::vector<CCircle> results = hr.FindCircles();
    drawResult( results );
    std::cout << "score: " << scoreResult( results ) << std::endl;
}

void CStampRecognition::DoMinSquare()
{
    CMinSquareRecognizing mr;
    mr.FindCircles();
}

void CStampRecognition::drawResult( std::vector<CCircle> results )
{
    IplImage* base = cvCloneImage( image );

    // рисуем правильные ответы зелёным
    for( size_t i = 0; i < answers.size(); ++i ) {
        CvPoint center = cvPoint( answers[i].GetCenterX(), answers[i].GetCenterY() );
        cvCircle( base, center, answers[i].GetRadius(), CV_RGB( 0, 0xff, 0 ), 3 );
    }

    // рисуем результаты работы алгоритма красным
    for( size_t i = 0; i < results.size(); ++i ) {
        CvPoint center = cvPoint( results[i].GetCenterX(), results[i].GetCenterY() );
        cvCircle( base, center, results[i].GetRadius(), CV_RGB( 0xff, 0, 0 ), 3 );
    }

    cvSaveImage( imageSavePath.c_str(), base );

    cvReleaseImage( &base );
}

double CStampRecognition::scoreResult( std::vector<CCircle> results )
{
    double scoresSum = 0.0f;
    for( size_t i = 0; i < answers.size(); ++i ) {
        double maxScore = 0.0f;
        for( size_t j = 0; j < results.size(); ++j ) {
            double score = answers[i].IntersectionSquare( results[i] ) * 2.;
            score = score / ( answers[i].Square() + results[i].Square() );
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
    if( answers.size() < results.size() ) {
        scoresSum += ( answers.size() - results.size() ) * EXTRA_CIRCLES_PENALTY;
    }
    return scoresSum;
}
