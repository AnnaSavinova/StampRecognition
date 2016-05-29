#pragma once

#include <Eigen\Dense>
#include "Circle.h"

class CMinSquareRecognizing {
public:
    CMinSquareRecognizing( cv::Mat _image, std::string _imageSavePath );
    ~CMinSquareRecognizing();

    std::vector<CCircle> FindCircles();

private:
    Eigen::MatrixXd generateScatterMatrix( std::vector<CvPoint> points );

    CCircle getCircleByPoints( std::vector<CvPoint> points );
    std::vector<cv::Point> getContours();

    //TODO подобрать параметр
    const size_t filterThreshold = 70;
    std::vector< std::vector<cv::Point> > filterContours( std::vector< std::vector<cv::Point> >& contours );

    //TODO подобрать параметр
    const double spectralTolerance = 0.0000000000001;
    void spectralDecomposition( Eigen::MatrixXd& m );

    inline static bool isAbsInf( double a ) { return abs( a ) > 1.e+13; }

    cv::Mat image;
    std::string imageSavePath;
};

