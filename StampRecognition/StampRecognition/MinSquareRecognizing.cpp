#include "MinSquareRecognizing.h"

#include <Eigen\Eigenvalues>
#include <opencv2\core\core.hpp>
#include <opencv2\core\eigen.hpp>
#include <opencv2\highgui\highgui.hpp>

#include <iostream>

CMinSquareRecognizing::CMinSquareRecognizing()
{}

CMinSquareRecognizing::~CMinSquareRecognizing()
{}

std::vector<CCircle> CMinSquareRecognizing::FindCircles()
{
    std::vector<CvPoint> points;
    points.push_back( CvPoint( 15, 7 ) );
    points.push_back( CvPoint( 10, 12 ) );
    points.push_back( CvPoint( 5, 7 ) );
    points.push_back( CvPoint( 10, 2 ) );
    //points.push_back( CvPoint( 1, 3 ) );
    //points.push_back( CvPoint( 2, 8 ) );

    std::vector<CCircle> result;
    result.push_back( getCircleByPoints( points ) );
    return result;
}

Eigen::MatrixXd CMinSquareRecognizing::generateScatterMatrix( std::vector<CvPoint> points )
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

CCircle CMinSquareRecognizing::getCircleByPoints( std::vector<CvPoint> points )
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
            return CCircle( x, y, r );
        }
    }

    return CCircle( 0, 0, 0 );
}