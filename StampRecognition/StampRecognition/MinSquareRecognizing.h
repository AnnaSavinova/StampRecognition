#pragma once

#include <Eigen\Dense>
#include "Circle.h"

class CMinSquareRecognizing {
public:
    CMinSquareRecognizing();
    ~CMinSquareRecognizing();

    std::vector<CCircle> FindCircles();

private:
    Eigen::MatrixXd generateScatterMatrix( std::vector<CvPoint> points );

    CCircle getCircleByPoints( std::vector<CvPoint> points );

    //TODO подобрать параметр
    const double spectralTolerance = 0.0000000000001;
    void spectralDecomposition( Eigen::MatrixXd& m );

    inline static bool isAbsInf( double a ) { return abs( a ) > 1.e+13; }
};

