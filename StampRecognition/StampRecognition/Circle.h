#pragma once

#include "Common.h"

class CCircle {
public:
    CCircle( int _centerX, int _centerY, int _radius );
    ~CCircle();

    int GetCenterX() { return centerX; }
    int GetCenterY() { return centerY; }
    int GetRadius() { return radius; }

    double Square() { return CV_PI * radius * radius; }
    double IntersectionSquare( const CCircle& oth );
private:
    int centerX;
    int centerY;
    int radius;
};

