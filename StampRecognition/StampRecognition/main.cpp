#include "StampRecognition.h"

int main( int argc, char* argv[] )
{
    // ��� ����� � ����������� � ��������� �������� ������ ����������
    char* imagesInfo = argc >= 2 ? argv[1] : "";
    // ������ ���������� �������� ���� �� ����� � �������������
    char* imagesPathPrefix = argc >= 3 ? argv[2] : "";
    std::string imageSavePathPrefix( imagesPathPrefix );
    imageSavePathPrefix += "result\\";
    std::ifstream in;
    in.open( imagesInfo, std::ifstream::in );
    if( !in.is_open() ) {
        throw std::invalid_argument( "���������� ������� ���� � ����������� � ������������" );
    }

    std::string scoresPathPrefix( imageSavePathPrefix );

    while( !in.eof() ) {
        std::string imageName;
        in >> imageName;
        std::string imagePath = imagesPathPrefix + imageName;
        std::string imageSavePath = imageSavePathPrefix + imageName;

        int count, centerX, centerY, radius;
        in >> count;
        std::vector< CCircle > answers;

        for( int i = 0; i < count; ++i ) {
            in >> centerX;
            in >> centerY;
            in >> radius;
            std::cout << imagePath << " " << centerX << " " << centerY << " " << radius << std::endl;
            answers.push_back( CCircle( centerX, centerY, radius ) );
        }
        std::string scoresPath = scoresPathPrefix + imageName;
        scoresPath += "_scores.csv";
        
        CStampRecognition sr( imagePath.c_str(), answers, imageSavePath, scoresPath );
        sr.DoHough();
        //sr.DoMinSquare();
    }
    in.close();

    system( "pause" );
    return 0;
}