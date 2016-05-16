#include "Circle.h"


CCircle::CCircle( int _centerX, int _centerY, int _radius ) : centerX( _centerX ), centerY( _centerY ), radius( _radius )
{}

CCircle::~CCircle()
{}

double CCircle::IntersectionSquare( const CCircle& oth )
{
    double d = sqrt( ( centerX - oth.centerX ) * ( centerX - oth.centerX ) + ( centerY - oth.centerY ) * ( centerY - oth.centerY ) );

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
        double minRadius = MIN( radius, oth.radius );
        return CV_PI * minRadius * minRadius;
    }
    double f1 = 2 * acos( ( radius * radius - oth.radius * oth.radius + d * d ) / ( 2 * radius * d ) );
    double f2 = 2 * acos( ( oth.radius * oth.radius - radius * radius + d * d ) / ( 2 * oth.radius * d ) );
    double s1 = ( radius * radius * f1 - sin( f1 ) ) / 2;
    double s2 = ( oth.radius * oth.radius * f2 - sin( f2 ) ) / 2;
    return s1 + s2;
}