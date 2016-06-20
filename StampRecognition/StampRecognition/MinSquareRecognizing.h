#pragma once

#include <Eigen\Dense>
#include "Circle.h"

#include <fstream>

class CMinSquareRecognizing {
public:
    CMinSquareRecognizing( cv::Mat _image, std::string _imageSavePath );
    ~CMinSquareRecognizing();

    std::vector<CCircle> FindCircles();

private:
    Eigen::MatrixXd generateScatterMatrix( std::vector<cv::Point> points );

    CCircle getCircleByPoints( std::vector<cv::Point> points );
    std::vector< std::vector<cv::Point> > getContours();

    //TODO ��������� ��������
    const double accuracyThreshold = 70000.0;
    double countCircleAccuracy( double x, double y, double r, std::vector<cv::Point>& points );
    bool checkIfCircle( CCircle& circle, std::vector< cv::Point >& contour );

    //TODO ��������� ��������
    const size_t filterThreshold = 70;
    std::vector< std::vector<cv::Point> > filterContours( std::vector< std::vector<cv::Point> >& contours );

    //TODO ��������� ��������
    const double spectralTolerance = 0.0000000000001;
    void spectralDecomposition( Eigen::MatrixXd& m );

    inline static bool isAbsInf( double a ) { return abs( a ) > 1.e+13; }

    cv::Mat image;
    std::string imageSavePath;

    std::ofstream out;
};

