#include "StampRecognition.h"
#include <fstream>

int main( int argc, char* argv[] )
{
    // ��� ����� � ����������� � ��������� �������� ������ ����������
    char* imagesInfo = argc >= 2 ? argv[1] : "";
    // ������ ���������� �������� ���� �� ����� � �������������
    char* imagesPathPrefix = argc >= 3 ? argv[2] : "";
    std::ifstream in;
    in.open( imagesInfo, std::ifstream::in );
    if( !in.is_open() ) {
        throw std::invalid_argument( "���������� ������� ���� � ����������� � ������������" );
    }

    while( !in.eof() ) {
        std::string imageName;
        in >> imageName;
        std::string imagePath = imagesPathPrefix + imageName;

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