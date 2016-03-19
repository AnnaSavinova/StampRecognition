#include "StampRecognition.h"

int main( int argc, char* argv[] )
{
    // имя картинки задаётся первым параметром
    char* filename = argc >= 2 ? argv[1] : "Image0.jpg";

    CStampRecognition sr( filename );
    sr.DoHough();

    system( "pause" );
    return 0;
}