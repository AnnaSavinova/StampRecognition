#include "StampRecognition.h"

CStampRecognition::CStampRecognition( const char* _imagePath, std::vector< CCircle > _answers, std::string _imageSavePath, std::string _scoresPath ) :
    answers( _answers ), imageSavePath( _imageSavePath ), scoresPath( _scoresPath )
{
    matImage = cv::imread( _imagePath );
    matGrayImage = cv::imread( _imagePath, CV_LOAD_IMAGE_GRAYSCALE );

    image = cvLoadImage( _imagePath );
    grayImage = cvLoadImage( _imagePath, CV_LOAD_IMAGE_GRAYSCALE );

    std::cout << "image: " << _imagePath << std::endl;
    if( image == 0 || grayImage == 0 ) {
        std::string errorMsg = "���������� ������� ����������� ";
        errorMsg += _imagePath;
        throw std::invalid_argument( errorMsg );
    }
}

CStampRecognition::~CStampRecognition()
{
    cvReleaseImage( &image );
    cvReleaseImage( &grayImage );
}

void CStampRecognition::DoHough()
{
    CHoughRecognition hr( matGrayImage );

    std::vector<CCircle> results = hr.FindCircles();
    drawResult( results );
    double score = scoreResult( results );
    std::cout << "score: " << score << std::endl;

    std::fstream out;
    out.open( scoresPath, std::fstream::out );
    out << score << "," << results.size() << "," << answers.size() << std::endl;
    out.close();
}

void CStampRecognition::DoMinSquare()
{
    CMinSquareRecognizing mr( matImage, imageSavePath );
    std::vector<CCircle> results = mr.FindCircles();

    drawResult( results );
    double score = scoreResult( results );
    std::cout << "score: " << score << std::endl;

    std::fstream out;
    out.open( scoresPath, std::fstream::out );
    out << score << "," << results.size() << "," << answers.size() << std::endl;
    out.close();
}

void CStampRecognition::drawResult( std::vector<CCircle> results )
{
    IplImage* base = cvCloneImage( image );

    // ������ ���������� ������ ������
    for( size_t i = 0; i < answers.size(); ++i ) {
        CvPoint center = cvPoint( answers[i].GetCenterX(), answers[i].GetCenterY() );
        cvCircle( base, center, answers[i].GetRadius(), CV_RGB( 0, 0xff, 0 ), 3 );
    }

    // ������ ���������� ������ ��������� �������
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
    //��������� ��� ������ ���������� �������
    if( answers.size() > 1 ) {
        scoresSum /= answers.size();
    }

    // �������� �� ������ ��������� �����
    if( answers.size() < results.size() ) {
        scoresSum += ( answers.size() - results.size() ) * EXTRA_CIRCLES_PENALTY;
    }
    return scoresSum;
}
