#include "StampRecognition.h"
#include <fstream>

const std::string IMAGES_PATH_PREFIX ="C:\\Users\\Anna\\Diplom\\Images\\";

int main( int argc, char* argv[] )
{
    // имя файла с информацией о картинках задается первым аргументом
    char* imagesInfo = argc >= 2 ? argv[1] : "";
    std::ifstream in;
    in.open( imagesInfo, std::ifstream::in );

    while( !in.eof() ) {
        std::string imageName;
        in >> imageName;
        std::string imagePath = IMAGES_PATH_PREFIX + imageName;

        int count, centerX, centerY, radius;
        in >> count;
        std::vector< CStampRecognition::CCircle > answers;

        for( int i = 0; i < count; ++i ) {
            in >> centerX;
            in >> centerY;
            in >> radius;
            std::cout << imagePath << " " << centerX << " " << centerY << " " << radius << std::endl;
            answers.push_back( CStampRecognition::CCircle( centerX, centerY, radius ) );
        }
        CStampRecognition sr( imagePath.c_str(), answers );
        sr.DoHough();
    }
    in.close();

    system( "pause" );
    return 0;
}