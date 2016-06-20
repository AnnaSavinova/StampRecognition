#include "MinSquareRecognizing.h"

#include <Eigen\Eigenvalues>
#include "Common.h"

CMinSquareRecognizing::CMinSquareRecognizing( cv::Mat _image, std::string _imageSavePath ) : image( _image ), imageSavePath( _imageSavePath )
{}

CMinSquareRecognizing::~CMinSquareRecognizing()
{}

std::vector<CCircle> CMinSquareRecognizing::FindCircles()
{
    std::vector< std::vector<cv::Point> > contours = getContours();

    out.open( imageSavePath + "accuracy.csv", std::ofstream::out );

    std::vector<CCircle> result;
    for( size_t i = 0; i < contours.size(); ++i ) {
        CCircle circle = getCircleByPoints( contours[i] );
        if( checkIfCircle( circle, contours[i] ) ) {
            result.push_back( circle );
        }
    }

    out.close();

    /*std::vector<cv::Point> points;
    points.push_back( cv::Point( 15, 7 ) );
    points.push_back( cv::Point( 10, 12 ) );
    points.push_back( cv::Point( 5, 7 ) );
    points.push_back( cv::Point( 10, 2 ) );*/
    //points.push_back( cv::Point( 1, 3 ) );
    //points.push_back( cv::Point( 2, 8 ) );
    /*result.push_back( getCircleByPoints( points ) );*/
    return result;
}

Eigen::MatrixXd CMinSquareRecognizing::generateScatterMatrix( std::vector<cv::Point> points )
{
    Eigen::MatrixXd D( points.size(), 4 );
    D.fill( 0 );
    for( int i = 0; i < points.size(); ++i ) {
        D( i, 0 ) = points[i].x * points[i].x + points[i].y * points[i].y;
        D( i, 1 ) = points[i].x;
        D( i, 2 ) = points[i].y;
        D( i, 3 ) = 1.;
    }

    Eigen::MatrixXd DT = D.transpose();

    return DT * D;
}

CCircle CMinSquareRecognizing::getCircleByPoints( std::vector<cv::Point> points )
{
    Eigen::MatrixXd C( 4, 4 );
    C.fill( 0 );
    C( 0, 0 ) = 4.;

    Eigen::MatrixXd S = generateScatterMatrix( points );

    Eigen::MatrixXd InverseS = S.inverse();
    Eigen::MatrixXd engineM = InverseS * C;

    Eigen::EigenSolver<Eigen::MatrixXd> es( engineM, true );

    Eigen::MatrixXd vecs = es.eigenvectors().real();
    Eigen::MatrixXd vals = es.eigenvalues().real();

    for( int i = 0; i < vecs.rows(); ++i ) {
        Eigen::MatrixXd row = vecs.row( i );
        Eigen::MatrixXd rowTranspose = row.transpose();
        double tmp = ( row * S * rowTranspose )( 0, 0 );

        if( ( 1. / vals( i, 0 ) ) > 0
            && !isAbsInf( 1. / vals( i, 0 ) )
            && !isAbsInf( vals( i, 0 ) )
            && tmp > 0 ) {

            //нормируем коэффициент перед квадратами до 1
            double a = vecs( 0, i );
            double b = vecs( 1, i ) / a;
            double c = vecs( 2, i ) / a;
            double d = vecs( 3, i ) / a;


            double x = -b / 2.;
            double y = -c / 2.;
            double r = sqrt( b * b + c * c - 4 * d ) / 2.;

            double accuracy = countCircleAccuracy( x, y, r, points );
            return CCircle( cvRound( x ), cvRound( y ), cvRound( r ), accuracy );
        }
    }

    return CCircle( 0, 0, 0 );
}

double CMinSquareRecognizing::countCircleAccuracy( double x, double y, double r, std::vector<cv::Point>& points )
{
    double result = 0.0;
    for( size_t i = 0; i < points.size(); ++i ) {
        result += ( ( x - points[i].x ) * ( x - points[i].x ) + ( y - points[i].y ) * ( y - points[i].y ) - r * r ) * ( ( x - points[i].x ) * ( x - points[i].x ) + ( y - points[i].y ) * ( y - points[i].y ) - r * r );
    }

    result /= points.size();

    return result;
}

std::vector< std::vector<cv::Point> > CMinSquareRecognizing::getContours()
{
    try {
        cv::Mat gray;

        cv::cvtColor( image, gray, CV_RGB2GRAY );
        cv::threshold( gray, gray, 128, 255, CV_THRESH_BINARY );

        std::vector< std::vector<cv::Point> > contours;
        std::vector<cv::Vec4i> hierarchy;

        cv::Mat contourOutput = gray.clone();
        cv::findContours( contourOutput, contours, CV_RETR_LIST, CV_CHAIN_APPROX_NONE );

        std::cout << contours.size() << std::endl;

        std::vector< std::vector<cv::Point> > filteredContours = filterContours( contours );
        std::cout << filteredContours.size() << std::endl;

        cv::Mat imageWithContours = cv::Mat::zeros( gray.size(), CV_8UC3 );
        for( int i = 0; i < filteredContours.size(); ++i ) {
            cv::drawContours( imageWithContours, filteredContours, i, cv::Scalar( 255, 255, 255 ), 1, 8 );
        }

        cv::bitwise_not( imageWithContours, imageWithContours );

        cv::imwrite( imageSavePath, imageWithContours );

        return filteredContours;

    } catch( cv::Exception e ) {
        std::cout << e.msg;
    }

    return std::vector< std::vector<cv::Point> >();
}

std::vector< std::vector<cv::Point> > CMinSquareRecognizing::filterContours( std::vector< std::vector<cv::Point> >& contours )
{
    std::vector< std::vector<cv::Point> > result;

    for( size_t i = 0; i < contours.size(); ++i ) {
        if( contours[i].size() > filterThreshold ) {
            result.push_back( contours[i] );
        }
    }

    return result;
}

bool CMinSquareRecognizing::checkIfCircle( CCircle& circle, std::vector< cv::Point >& contour )
{
    if( circle.GetAccuracy() < accuracyThreshold && circle.GetRadius() > 50 ) {
        return true;
    }

    return false;
}